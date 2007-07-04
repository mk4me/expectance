/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "traceline.h"
using namespace ft;

TraceLine::TraceLine(const std::string& name)
{
	setName(name);
	setColor(FTVect(1,1,1));
	m_colorMix = false;
	m_marker = true;
}

bool TraceLine::Render()
{
	if(m_traces.size() > 1) //minimum two elements
	{
		for (unsigned int i = 1; i < m_traces.size(); i++)
		{
			FTVect p1 = static_cast<FTVect>(m_traces[i-1]);
			FTVect p2 = static_cast<FTVect>(m_traces[i]);
			
			
			glPushMatrix();
			if (!m_colorMix)
				glColor3f(m_color.x,m_color.y,m_color.z);
			else
			{
				glColor3f(i/15.0,i/10.0,i/50.0);
			}

			glBegin(GL_LINES);
				glVertex3f(p1.x,p1.y,p1.z);
				glVertex3f(p2.x,p2.y,p2.z);
			glEnd();

			if (m_marker)
			{
				glPointSize(8.0f);
				glColor3f(0.9f,0.5f, 0.8f);
				glBegin(GL_POINTS);
					glVertex3f(p1.x, p1.y, p1.z);
				glEnd();

					glPointSize(1.0f);
			}

			glPopMatrix();
		}
	}
	return true;
}

void TraceLine::AddPoint(const FTVect& point)
{
	m_traces.push_back(point);
}

void TraceLine::ClearTrace()
{
	m_traces.clear();
}

void TraceLine::setColorMix(bool mix)
{
	m_colorMix = mix;
}

void TraceLine::ShowMarker()
{
	m_marker = true;
}

void TraceLine::HideMarker()
{
	m_marker = false;
}


