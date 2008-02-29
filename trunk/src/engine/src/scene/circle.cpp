/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "circle.h"

using namespace ft;

Circle::Circle(const std::string& name)
{
	setName(name);
	setRenderingOrder(ft_Rendering_Trace_Level);
	setColor(CalVector(1,1,1));

	setRadius(50);
	setDisk(false);
	setSegmentsNumber(20);
	setStippleParameters(); // false but initialize pattern for eventual stipple
}

Circle::~Circle(void)
{
	_dbg << getID() << " circle deleted\n";
}

bool Circle::Render()
{
	const float step = (2 * Pi) / m_segments;
	const int vertexCount = m_filled ? m_segments+1 : m_segments;
	float _x, _z;

	glPushMatrix();
		if (m_filled){
			glPushAttrib (GL_ENABLE_BIT);
			glDisable (GL_CULL_FACE);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		if(m_lineStipple)
		{
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, 0x00ff);
		}
		// begin drawing a triangle fan (for disk) or line loop (for circle)
		glColor4f (m_color.x, m_color.y, m_color.z, getAlpha());
		glBegin (m_filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
		for (int i = 0; i < vertexCount; i++)
		{
			_x = cos(step*i)*m_radius + m_position.x;
			_z = sin(step*i)*m_radius + m_position.z;
			glVertex3f(_x,1,_z);
		}
		glEnd();
		glDisable(GL_BLEND);	
		if(m_lineStipple) glDisable(GL_LINE_STIPPLE);
		if (m_filled) glPopAttrib();
	glPopMatrix();
	return true;
}

Circle&  Circle::setRadius(const float radius)
{
	m_radius = radius;
	return *this;
}

Circle&  Circle::setDisk(const bool filled)
{
	m_filled = filled;
	return *this;
}

Circle&  Circle::setSegmentsNumber(const int segments)
{
	m_segments = segments;
	return *this;
}

Circle& Circle::setStippleParameters(bool const lineStipple, const GLint factor, const GLshort pattern)
{
	m_lineStipple = lineStipple;
	m_lineStippleFactor = factor;
	m_lineStipplePattern = pattern;
	return *this;
}

void Circle::setParameters(const float radius, const CalVector& center, const CalVector& color, 
				   const int segments, const bool filled)
{
	setRadius(radius);
	setPosition(center);
	setColor(color);
	setSegmentsNumber(segments);
	setDisk(filled);
}
