/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "line.h"

using namespace ft;

/**
 * Line: <describe the responsibilities and behavior of this method>
 *
 * \param const std::string & name <argument description>
 **/
Line::Line(const std::string& name):m_arrow(false)
{
	setName(name);
	
}

/**
 * Line: <describe the responsibilities and behavior of this method>
 *
 * \param const FTVect & position <argument description>
 * \param const FTVect & orientation <argument description>
 * \param float length <argument description>
 * \param const std::string & name <argument description>
 **/
Line::Line(const FTVect& position, const FTVect& orientation, float lenght, const std::string& name):m_arrow(false)
{
	FTVect tmpv = orientation;
	tmpv.normalize();
	tmpv = (m_start + tmpv*lenght);

	setLenght(lenght).setStart(position).setEnd(tmpv);
	setPosition(position).setOrientation(orientation);
	setName(name);
	

	//Line(position, tmpv, name); 
}


/**
 * Line: <describe the responsibilities and behavior of this method>
 *
 * \param const FTVect & start <argument description>
 * \param const FTVect & end <argument description>
 * \param const std::string & name <argument description>
 **/
Line::Line(const FTVect& start, const FTVect& end, const std::string& name):m_arrow(false)
{
	m_start = start;
	m_end = end;

	setName(name);
}


/**
 * ~Line: <describe the responsibilities and behavior of this method>
 *
 **/
Line::~Line(void)
{
	std::cout << getID() << " line deleted\n";
}

/**
 * Render: <describe the responsibilities and behavior of this method>
 *
 * \return bool <describe what is returned if appropriate>
 **/
bool Line::Render()
{
	glPushMatrix();

		//set position, orientation
		glTranslatef(m_start.x, m_start.y, m_start.z);
		//set object parameters
		//draw it	
		glColor4f(m_color.x, m_color.y, m_color.z, m_alpha);
		glBegin(GL_LINES);
			glVertex3f(m_start.x, m_start.y, m_start.z);
			glVertex3f(m_end.x, m_end.y, m_end.z);
		glEnd();
		glEnable(GL_BLEND);
		glShadeModel(GL_SMOOTH);
		glDisable(GL_CULL_FACE);

		if(m_arrow)
		{
			GLUquadricObj* pQuadric = gluNewQuadric();

			FTVect v = m_end-m_start;
			float height = v.normalize();
			float angle = 0.0f;
			if(sqrt(v.x*v.x+v.y*v.y) > 1)
				angle = 90.0f;
			else
				angle = (float)asin(sqrt(v.x*v.x+v.y*v.y))/3.14159f*180.0f;
			if(v.z < 0.0f)
				angle = 180.0f-angle;

           // the rotation vector
           FTVect rot(v.y,-v.x,0.0f);

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

/**
 * setStart: <describe the responsibilities and behavior of this method>
 *
 * \param const FTVect & start <argument description>
 * \return ft::Line & <describe what is returned if appropriate>
 **/
Line& Line::setStart(const FTVect& start)
{
	m_start = start;
	return *this;
}

/**
 * setEnd: <describe the responsibilities and behavior of this method>
 *
 * \param const FTVect & end <argument description>
 * \return ft::Line & <describe what is returned if appropriate>
 **/
Line& Line::setEnd(const FTVect& end)
{
	m_end = end;
	return *this;
}


/**
 * setLenght: <describe the responsibilities and behavior of this method>
 *
 * \param const float lenght <argument description>
 * \return ft::Line & <describe what is returned if appropriate>
 **/
Line& Line::setLenght(const float lenght)
{
	m_lenght = lenght;
	return *this;
}


/**
 * setArrow: <describe the responsibilities and behavior of this method>
 *
 * \param const bool arrow <argument description>
 * \return ft::Line & <describe what is returned if appropriate>
 **/
Line& Line::setArrow(const bool arrow)
{
	m_arrow = arrow;
	return *this;
}
