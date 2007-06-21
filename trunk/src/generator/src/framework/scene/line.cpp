/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "line.h"

using namespace ft;

Line::Line(FTVect &position, FTVect &orientation, float length, std::string id)
{
	m_start = position;
	m_position = position;
	m_orientation = orientation;
	m_length = length;
	orientation.normalize();
	m_end = (m_start + orientation*length);
	setID(id);
	std::cout << id << " simple line created \n";	
}


Line::Line(FTVect &start, FTVect &end, std::string id)
{
	m_start = start;
	m_end = end;
	setID(id);
	std::cout << id << " line created\n";	
}

Line::Line(FTVect &start, FTVect &end, std::string id, FTVect &color)
{
	m_start = start;
	m_end = end;
	setColor(color);
	setID(id);
	std::cout << id << " color line created\n";
}

Line::~Line(void)
{
	std::cout << m_id << " line deleted\n";
}

bool Line::Render()
{
	glPushMatrix();
		glEnable(GL_BLEND);
		//set position, orientation
		glTranslatef(m_start.x, m_start.y, m_start.z);
		//set object parameters
		//draw it	
		glColor4f(m_color.x, m_color.y, m_color.z, m_alpha);
		glBegin(GL_LINES);
			glVertex3f(m_start.x, m_start.y, m_start.z);
			glVertex3f(m_end.x, m_end.y, m_end.z);
		glEnd();
		glDisable(GL_BLEND);
	glPopMatrix();
	return true;
}

bool Line::isActive()
{
	return true;
}