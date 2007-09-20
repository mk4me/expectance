/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "SceneObject.h"
using namespace ft;

SceneObject::SceneObject(void)
{
	m_position.set(0,0,0);
	m_orientation = QuatToCalQuat( Quat(degToRad(0.0f), Vec(0,1,0)) ); //accordig to positive Z axis (zero angle)
	m_color.set(1.0,1.0,1.0); //white
	m_alpha = 1;
	m_visible = true;
}

bool SceneObject::Render()
{
	return true;
}

bool SceneObject::RenderShadow()
{
	return true;
}


SceneObject& SceneObject::setPosition(const CalVector& pos)
{
	m_position = pos;
	return *this;
}



SceneObject& SceneObject::setOrientation(const CalQuaternion &orientation)
{
	m_orientation = orientation;
	return *this;
}

SceneObject& SceneObject::setColor(const CalVector& col)
{
	m_color = col;
	return *this;
}

const CalVector& SceneObject::getPosition() const
{
	return m_position;
}

const CalQuaternion& SceneObject::getOrientation() const
{
	return m_orientation;
}

const CalVector& SceneObject::getColor() const
{
	return m_color;
}

void SceneObject::Show()
{
	m_visible = true;
}

void SceneObject::Hide()
{
	m_visible = false;
}

const bool SceneObject::isVisible() const
{
	return m_visible;
}

const void SceneObject::PrintInfo(void) const
{
	std::cout << "ID " << getID() <<", location = [" << m_position.x <<", " << m_position.y << ", " << m_position.z 
	<<"], m_color = [" << m_color.x << ", "<<m_color.y<<", "<< m_color.z <<"] \n";
}

