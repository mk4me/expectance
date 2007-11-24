/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "SceneObject.h"
using namespace ft;

SceneObject::SceneObject(void)
{
	m_position.set(0,0,0);
	m_direction.set(0,0,1); // each scene object start direction according to positive Z axis (zero angle)
                            // ABAK's note: doesn't true for avatar - avatar is originally oriented along positive X axis
	m_orientation = QuatToCalQuat( Quat(degToRad(0.0f), Vec(0,1,0)) ); //global orientation (zero angle around Y axis for initial global rotation)
    m_globalRotationOffset = CalQuaternion(); // no rotation
	m_color.set(1.0,1.0,1.0); //white
	m_alpha = 1;
	m_visible = true;
}

byte SceneObject::getRenderingOrder()
{
	return 1; //by default
}

bool SceneObject::Render()
{
	return true;
}

bool SceneObject::RenderShadow()
{
	return true;
}


SceneObject& SceneObject::setPosition(const CalVector &position)
{
	m_position = position;
	return *this;
}
SceneObject& SceneObject::changePosition(const CalVector &deltaPosition)
{
	m_position += deltaPosition;
	return *this;
}

SceneObject& SceneObject::setOrientation(const CalQuaternion &rotation)
{
	m_orientation = rotation;
	return *this;
}

SceneObject& SceneObject::changeOrientation(const CalQuaternion &deltaRotation)
{
	m_orientation *= deltaRotation;
	return *this;
}

SceneObject& SceneObject::setGlobalRotationOffset(const CalQuaternion &rotation)
{
	m_globalRotationOffset = rotation;
	return *this;
}

SceneObject& SceneObject::changeGlobalRotationOffset(const CalQuaternion &deltaRotation)
{
	m_globalRotationOffset *= deltaRotation;
	return *this;
}

SceneObject& SceneObject::setDirection(const CalVector &direction)
{
	m_direction = direction;
	return *this;
}

SceneObject& SceneObject::setDirection(const CalQuaternion &direction)
{
	// TODO	 verify practical sense of that
	CalVector _originDirection = CalVector(0,0,1);
	_originDirection*=direction;
	m_direction = _originDirection;
	return *this;
}
SceneObject& SceneObject::changeDirection(const CalVector &deltaDirection)
{
	m_direction += deltaDirection;
	return *this;
}

SceneObject& SceneObject::changeDirection(const CalQuaternion &deltaDirection)
{
	m_direction *= deltaDirection;
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

const CalQuaternion& SceneObject::getGlobalRotationOffset() const
{
	return m_globalRotationOffset;
}


const CalVector& SceneObject::getDirection() const
{
	return m_direction;
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
	_dbg << "ID " << getID() <<", location = [" << m_position.x <<", " << m_position.y << ", " << m_position.z 
	<<"], m_color = [" << m_color.x << ", "<<m_color.y<<", "<< m_color.z <<"] \n";
}

