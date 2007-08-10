/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "SceneObject.h"
using namespace ft;

SceneObject::SceneObject(void)
{
	m_position.set(0,0,0);
	m_orientation.set(0,0,1);
	m_color.set(1.0,1.0,1.0); //white
	m_alpha = 1;
	m_visible = true;
	m_vRotation = QuatToCalQuat(Quat(degToRad(0.0f), Vec(0,1,0))); // according to Z axis
}

bool SceneObject::Render()
{
	return true;
}

SceneObject& SceneObject::setPosition(const FTVect& pos)
{
	m_position = pos;
	return *this;
}


SceneObject& SceneObject::setOrientation(const FTVect& orientation)
{
	m_orientation = orientation;
	return *this;
}

SceneObject& SceneObject::setColor(const FTVect& col)
{
	m_color = col;
	return *this;
}

const FTVect& SceneObject::getPosition() const
{
	return m_position;
}

const FTVect& SceneObject::getOrientation() const
{
	return m_orientation;
}

const FTVect& SceneObject::getColor() const
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

