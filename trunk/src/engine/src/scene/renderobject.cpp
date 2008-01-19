/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "renderobject.h"
using namespace ft;

RenderObject::RenderObject(void):
m_renderingOrder(ft_Rendering_StaticShadow_Level),
m_visible(true)
{
	m_color.set(1.0,1.0,1.0); //white
}

bool RenderObject::Render()
{
	return true;
}

bool RenderObject::RenderShadow()
{
	return true;
}

const byte RenderObject::getRenderingOrder()
{
	return m_renderingOrder; 
}

RenderObject& RenderObject::setRenderingOrder(RenderingOrder ro)
{
	m_renderingOrder = ro;
	return *this;
}

const CalVector& RenderObject::getColor() const
{
	return m_color;
}

RenderObject& RenderObject::setColor(const CalVector& col, const float alpha)
{
	setAlpha(alpha);
	m_color = col;
	return *this;
}

RenderObject& RenderObject::setVisible( bool vis)
{
	m_visible =  vis;
	return *this;
}

const bool RenderObject::isVisible() const
{
	return m_visible;
}
