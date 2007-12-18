/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "DataCollector.h"
#include "oglcontext.h"
using namespace ft;


DataCollector::DataCollector(const std::string& name)
{
	setName(name);
	setColor(CalVector(1,1,1));
	setBufferSize(OGLContext::getInstance()->getWidth()/2);
	setBlending(true);
	m_min=m_max = 0.0f;
    m_drawScale = 1.0f;
    m_drawOffset = 0.0f;
}


bool DataCollector::Render()
{
	unsigned long _size = m_DataList.size();
	if(_size > 1) //minimum two elements
	{
		float dvpHeight = OGLContext::getInstance()->getDataViewportHeight();

        glPushMatrix();
		glDisable(GL_CULL_FACE);
		if (m_blending)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
		}
		float _div = 1.0f/_size;
		std::list<float>::iterator it = m_DataList.begin();

		int i=0;
        float p1;
		for (; it!=m_DataList.end(); ++it)
		{
			float p2 = *it;
            p2 += getDrawOffset();
            p2 *= getDrawScale();
            p2 = dvpHeight/2 + p2;

            if (i>0) // avoid first step to be ensured that p1 is defined
            {
			    glColor4f(m_color.x,m_color.y,m_color.z, 1.0);
			    glBegin(GL_LINES);
				    glVertex2f(i-1,p1);
				    glVertex2f(i,p2);
			    glEnd();
			    // point at the end
			    if (m_points)
			    {
				    glPointSize(2.0f);
				    glColor4f(1.0f,1.0f,1.0f, 0.3f);
				    glBegin(GL_POINTS);
					    glVertex2f(i,p1);
				    glEnd();
			    }
            }

			p1=p2;
			i++;
		}

        glLineWidth(1.0f);
		if (m_blending)
			glDisable(GL_BLEND);
		
		glEnable(GL_CULL_FACE);
		glPopMatrix();

		if ((m_bufferSize != 0)&&(_size > m_bufferSize))
        {
		    m_DataList.pop_front();
        }
	}
	return true;
}


void DataCollector::AddValue(const float value)
{
	unsigned long _size = m_DataList.size();
	if ((m_bufferSize != 0)&&(_size > m_bufferSize))
    {
	    m_DataList.pop_front();
    }
	m_DataList.push_back(value);
	if(_size > 1) //minimum two elements
	{
		if ( value > m_max ) m_max = value;
		if ( value < m_min ) m_min = value;
		if (m_max != m_min)
			m_scope = m_max - m_min;
		else m_scope = 1;
	}
	else
	{
		m_min=m_max=value;
	}
}

void DataCollector::Clear()
{
	m_DataList.clear();
}


void DataCollector::ShowPoints()
{
	m_points = true;
}

void DataCollector::HidePoints()
{
	m_points = false;
}


void DataCollector::setBlending(bool blending)
{
	m_blending = blending;
}

void DataCollector::setBufferSize(const long bufferSize)
{
	m_bufferSize = bufferSize;
}
