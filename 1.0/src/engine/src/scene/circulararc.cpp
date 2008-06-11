/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "circulararc.h"
#include "../utility/mathutil.h"

using namespace ft;

CircularArc::CircularArc(const std::string& name)
{
	setName(name);
	setRenderingOrder(ft_Rendering_Trace_Level);
	setColor(CalVector(1,1,1));

	setRadius(50);
	setDisk(false);
	setSegmentsNumber(20);
}

CircularArc::~CircularArc(void)
{
	_dbg << getID() << " circle deleted\n";
}

bool CircularArc::Render()
{

		CalVector _ox = CalVector(1,0,0);
		m_start.normalize();
		m_end.normalize();
		float _startAngValue = UTIL_GetVectorsAngle(m_start, _ox);
		float sign = UTIL_GetSignForDirChange(_ox, m_start);
		if (sign==-1)
			_startAngValue = 2*Pi - _startAngValue;

		float _angleStartEndValue = UTIL_GetVectorsAngle(m_start,m_end);
		sign = UTIL_GetSignForDirChange(m_start,m_end);
		// determine the angular step per segment
		if (sign==-1)
			_angleStartEndValue = 2*Pi - _angleStartEndValue;

		const float radius = m_radius;


		const float step = _angleStartEndValue / m_segments;
		const int vertexCount = m_filled ? m_segments+1 : m_segments;
		float _x, _z;
		
		// set drawing color

	glPushMatrix();
		if (m_filled){
			glPushAttrib (GL_ENABLE_BIT);
			glDisable (GL_CULL_FACE);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);		
		// begin drawing a triangle fan (for disk) or line loop (for circle)
		glColor4f (m_color.x, m_color.y, m_color.z, getAlpha());
		glBegin (m_filled ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
		glVertex3f(m_position.x,1,m_position.z);
		for (int i = 0; i < vertexCount; i++)
		{
			_x = cos(_startAngValue+step*i)*m_radius + m_position.x;
			_z = sin(_startAngValue+step*i)*m_radius + m_position.z;
			glVertex3f(_x,1,_z);
		}
		glEnd();
		glDisable(GL_BLEND);	
		if (m_filled) glPopAttrib();
	glPopMatrix();
	return true;
}

CircularArc&  CircularArc::setRadius(const float radius)
{
	m_radius = radius;
	return *this;
}

CircularArc&  CircularArc::setDisk(const bool filled)
{
	m_filled = filled;
	return *this;
}

CircularArc&  CircularArc::setSegmentsNumber(const int segments)
{
	m_segments = segments;
	return *this;
}


CircularArc& CircularArc::setStart(const CalVector& start)
{
	m_start = start;
	return *this;
}


CircularArc& CircularArc::setEnd(const CalVector& end)
{
	m_end = end;
	return *this;
}

void CircularArc::setParameters(const float radius, const CalVector& start, const CalVector& end, const CalVector& center, const CalVector& color, 
				   const int segments, const bool filled)
{
	setStart(start);
	setEnd(end);
	setRadius(radius);
	setPosition(center);
	setColor(color);
	setSegmentsNumber(segments);
	setDisk(filled);
}
