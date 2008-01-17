/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "line.h"

using namespace ft;

Line::Line(const std::string& name):m_arrow(false)
{
	setName(name);
	setRenderingOrder(ft_Rendering_Trace_Level);
	setLenght(1);
	
}

Line::Line(const CalVector& position, const Quat& orientation, float lenght, const std::string& name):m_arrow(false)
{
	// axis angle unit vector + angle
	//1. unit vector*lenght
	//orientation.normalise(); //is it necessary???
	float _angle;
	Vec _axis;
	orientation.getAngleAxis(_angle,_axis);
	_axis.normalize(); //is it necessary???
	m_direction = VecToCalVec(_axis);
	m_end = (m_position + m_direction*lenght);

	setLenght(lenght);
	setPosition(position).setOrientation( QuatToCalQuat(orientation) );
	setName(name);
	setRenderingOrder(ft_Rendering_Trace_Level);
	//Line(position, tmpv, name); 
}

Line::Line(const CalVector& start, const CalVector& end, const std::string& name):m_arrow(false)
{
	m_position = start;
	m_end = end;

	setName(name);
	setRenderingOrder(ft_Rendering_Trace_Level);
}

Line::~Line(void)
{
	_dbg << getID() << " line deleted\n";
}

bool Line::Render()
{
	glPushMatrix();

		//set position, orientation
		//glTranslatef(m_start.x, m_start.y, m_start.z);
		//set object parameters
		//draw it	
		glColor4f(m_color.x, m_color.y, m_color.z, 1.0);
		glBegin(GL_LINES);
			glVertex3f(m_position.x, m_position.y, m_position.z);
			glVertex3f(m_end.x, m_end.y, m_end.z);
		glEnd();
		glEnable(GL_BLEND);
		glShadeModel(GL_SMOOTH);
		glDisable(GL_CULL_FACE);

		if(m_arrow)
		{
			GLUquadricObj* pQuadric = gluNewQuadric();

			CalVector v = m_end-m_position;
			float height = v.normalize();
			float angle = 0.0f;
			double _lenXY = sqrt(v.x*v.x+v.y*v.y); 
			if( _lenXY > 1)
				angle = 90.0f;
			else
				angle = (float)asin(_lenXY)/3.14159f*180.0f;
			if(v.z < 0.0f)
				angle = 180.0f-angle;

           // the rotation vector
           CalVector rot(v.y,-v.x,0.0f);

			GLdouble radius = 5;
			// the arrow (cone)
			glPushMatrix();
			 glTranslatef(m_end.x, m_end.y, m_end.z);
			 glPushMatrix();
			   glRotatef(-angle,rot.x,rot.y,rot.z);
			   gluCylinder(pQuadric,radius,0,2*radius,10,1);
			 glPopMatrix();
			glPopMatrix();
		  gluDeleteQuadric(pQuadric);

		}
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	glPopMatrix();
	return true;
}




Line& Line::setEnd(const CalVector& end)
{
	m_end = end;
	CalVector _diff = m_position- m_end;
	// correction for lenght
	m_lenght = _diff.length(); 
	// correction for direction
	m_direction = _diff;
	m_direction.normalize();

	return *this;
}

Line& Line::setLenght(const float lenght)
{
	m_end = (m_position + m_direction*lenght);
	m_lenght = lenght;
	
	return *this;
}


Line& Line::setDirection(const CalVector &direction)
{
	m_direction = direction;
	m_direction.normalize();
	m_end = m_position + m_direction*m_lenght;

	return *this;
}

Line& Line::setArrow(const bool arrow)
{
	m_arrow = arrow;
	return *this;
}
