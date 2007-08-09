/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "SceneObject.h"
using namespace ft;

/**
 * SceneObject: <describe the responsibilities and behavior of this method>
 *
 **/
SceneObject::SceneObject(void)
{
	m_position.set(0,0,0);
	m_orientation.set(0,0,1);
	m_color.set(1.0,1.0,1.0); //white
	m_alpha = 1;
	m_visible = true;
	m_vRotation = QuatToCalQuat(Quat(degToRad(0.0f), Vec(0,1,0))); // according to Z axis
}

/**
 * Render: <describe the responsibilities and behavior of this method>
 *
 * \return bool <describe what is returned if appropriate>
 **/
bool SceneObject::Render()
{
	return true;
}

/**
 * setPosition: <describe the responsibilities and behavior of this method>
 *
 * \param const FTVect & position <argument description>
 * \return ft::SceneObject & <describe what is returned if appropriate>
 **/
SceneObject& SceneObject::setPosition(const FTVect& pos)
{
	m_position = pos;
	return *this;
}

/**
 * setOrientation: <describe the responsibilities and behavior of this method>
 *
 * \param const FTVect & orientation <argument description>
 * \return ft::SceneObject & <describe what is returned if appropriate>
 **/
SceneObject& SceneObject::setOrientation(const FTVect& orientation)
{
	m_orientation = orientation;
	return *this;
}
/**
 * setColor: <describe the responsibilities and behavior of this method>
 *
 * \param const FTVect & color <argument description>
 * \return ft::SceneObject & <describe what is returned if appropriate>
 **/
SceneObject& SceneObject::setColor(const FTVect& col)
{
	m_color = col;
	return *this;
}



/**
 * getPosition: <describe the responsibilities and behavior of this method>
 *
 * \return const FTVect & <describe what is returned if appropriate>
 **/
const FTVect& SceneObject::getPosition() const
{
	return m_position;
}

/**
 * getOrientation: <describe the responsibilities and behavior of this method>
 *
 * \return const FTVect & <describe what is returned if appropriate>
 **/
const FTVect& SceneObject::getOrientation() const
{
	return m_orientation;
}

/**
 * getColor: <describe the responsibilities and behavior of this method>
 *
 * \return const FTVect & <describe what is returned if appropriate>
 **/
const FTVect& SceneObject::getColor() const
{
	return m_color;
}

/**
 * Show: <describe the responsibilities and behavior of this method>
 *
 **/
void SceneObject::Show()
{
	m_visible = true;
}

/**
 * Hide: <describe the responsibilities and behavior of this method>
 *
 **/
void SceneObject::Hide()
{
	m_visible = false;
}

/**
 * isVisible: <describe the responsibilities and behavior of this method>
 *
 * \return const bool <describe what is returned if appropriate>
 **/
const bool SceneObject::isVisible() const
{
	return m_visible;
}

/**
 * PrintInfo: <describe the responsibilities and behavior of this method>
 *
 * \return const void <describe what is returned if appropriate>
 **/
const void SceneObject::PrintInfo(void) const
{
	std::cout << "ID " << getID() <<", location = [" << m_position.x <<", " << m_position.y << ", " << m_position.z 
	<<"], m_color = [" << m_color.x << ", "<<m_color.y<<", "<< m_color.z <<"] \n";
}

