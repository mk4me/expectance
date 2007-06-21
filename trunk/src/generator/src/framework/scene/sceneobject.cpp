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
}

SceneObject::~SceneObject(void)
{
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


SceneObject& SceneObject::setID(const std::string& id)
{
	if (!id.empty())
	{
		m_id = id;
	}
	else
	{
		///char * id =	itoa(static_cast<int>(this),m_id,10); not good idea what about referencing on the same instance of object?
	}
	//TODOMKA
	//void SceneObject::setID(std::string id)
	//{
	//itoa(static_cast<int>(this),m_id,10);
	//strcpy(m_id,id);
	//}
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



const std::string& SceneObject::getID() const
{
	return m_id;
}

const bool SceneObject::isVisible() const
{
	return m_visible;
}

void SceneObject::PrintInfo(void) const
{
	std::cout << "ID " << m_id <<", location = [" << m_position.x <<", " << m_position.y << ", " << m_position.z 
	<<"], m_color = [" << m_color.x << ", "<<m_color.y<<", "<< m_color.z <<"] \n";
}

