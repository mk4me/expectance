/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "traceline.h"
using namespace ft;


TraceLine::TraceLine(const std::string& name)
{
	setName(name);
	setColor(CalVector(1,1,1));
	setColorMix(false);
	setMarkerShape(ft_Point);
	setMarkerColor(CalVector(0.5f, 0.5f, 0.8f));
	setBufferSize(20);
	setMarkerScale(4);
	setBlending(true);
	ShowMarker();
	ShowLine();
	setRenderingOrder(ft_Rendering_Trace_Level);
}

bool TraceLine::Render()
{
	unsigned long _size = m_traces.size();
	if(_size > 1) //minimum two elements
	{
		glPushMatrix();
		glDisable(GL_CULL_FACE);
		if (m_blending)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
		}
		float _div = 1.0f/_size;
		for (unsigned long i = 1; i < _size; i++)
		{
			CalVector p1 = static_cast<CalVector>(m_traces[i-1]);
			CalVector p2 = static_cast<CalVector>(m_traces[i]);
			
			

			if (!m_colorMix) //one color line
				glColor4f(m_color.x,m_color.y,m_color.z, _div);
			else //multicolor line
			{
				glColor4f(i/15.0,i/10.0,i/50.0, _div);
			}

			if (m_line)
			{
			glBegin(GL_LINES);
				glVertex3f(p1.x,p1.y,p1.z);
				glVertex3f(p2.x,p2.y,p2.z);
			glEnd();
			}
			// point at the end
			if (m_marker)
			{

				glColor4f(m_markerColor.x,m_markerColor.y,m_markerColor.z,_div);
				switch (m_shape)
				{
				case ft_Point:
					{
						glPointSize(3.0f);
						//glColor3f(0.9f,0.5f, 0.8f);
						glBegin(GL_POINTS);
							glVertex3f(p1.x, p1.y, p1.z);
						glEnd();
						break;
					};
				case ft_Diamond:
					{
						glLineWidth(2.0f);
						//glColor3f(0.5f,0.5f, 0.8f);
						glBegin(GL_QUADS);
							glVertex3f(p1.x+2*m_markerScale, p1.y, p1.z);
							glVertex3f(p1.x, p1.y, p1.z-2*m_markerScale);
							glVertex3f(p1.x-2*m_markerScale, p1.y, p1.z);
							glVertex3f(p1.x, p1.y, p1.z+2*m_markerScale);
						glEnd();
						glBegin(GL_TRIANGLE_FAN);
							glVertex3f(p1.x, p1.y-2*m_markerScale, p1.z);
							glVertex3f(p1.x+2*m_markerScale, p1.y, p1.z);
							glVertex3f(p1.x, p1.y, p1.z-2*m_markerScale);
							glVertex3f(p1.x-2*m_markerScale, p1.y, p1.z);
							glVertex3f(p1.x, p1.y, p1.z+2*m_markerScale);
						glEnd();
						break;
					};
				case ft_Cross:
					{
						glBegin(GL_TRIANGLE_FAN);
							glVertex3f(p1.x+m_markerScale, p1.y, p1.z-2*m_markerScale);
							glVertex3f(p1.x,			   p1.y, p1.z-m_markerScale);
							glVertex3f(p1.x-m_markerScale, p1.y, p1.z-2*m_markerScale);
						glEnd();
						glBegin(GL_TRIANGLE_FAN);
							glVertex3f(p1.x+2*m_markerScale, p1.y, p1.z-m_markerScale);
							glVertex3f(p1.x+2*m_markerScale, p1.y, p1.z+m_markerScale);
							glVertex3f(p1.x+m_markerScale, p1.y, p1.z);
						glEnd();
						glBegin(GL_TRIANGLE_FAN);
							glVertex3f(p1.x,			   p1.y, p1.z+m_markerScale);
							glVertex3f(p1.x+m_markerScale, p1.y, p1.z+2*m_markerScale);
							glVertex3f(p1.x-m_markerScale, p1.y, p1.z+2*m_markerScale);
						glEnd();
						glBegin(GL_TRIANGLE_FAN);
							glVertex3f(p1.x-m_markerScale, p1.y, p1.z);
							glVertex3f(p1.x-2*m_markerScale, p1.y, p1.z+m_markerScale);
							glVertex3f(p1.x-2*m_markerScale, p1.y, p1.z-m_markerScale);
						glEnd();
						break;
					};
				default: break;
				}
			}
			_div+=1.0f/_size;
		}
		glLineWidth(1.0f);
		if (m_blending)
			glDisable(GL_BLEND);
		
		glEnable(GL_CULL_FACE);
		glPopMatrix();
		_div = 0;
		if ((m_bufferSize != 0)&&(_size > m_bufferSize))
		m_traces.erase(m_traces.begin());
	}
	return true;
}


void TraceLine::AddPoint(const CalVector& point)
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

void TraceLine::ShowLine()
{
	m_line = true;
}

void TraceLine::HideLine()
{
	m_line = false;
}

void TraceLine::setMarkerShape(const ft::MarkerShape shape)
{
	m_shape = shape;
}


void TraceLine::setMarkerColor(const CalVector color)
{
	m_markerColor = color;
}

void TraceLine::setMarkerScale(const int scale)
{
	m_markerScale = scale;
}

void TraceLine::setBlending(bool blending)
{
	m_blending = blending;
}

void TraceLine::setBufferSize(const long bufferSize)
{
	m_bufferSize = bufferSize;
}
