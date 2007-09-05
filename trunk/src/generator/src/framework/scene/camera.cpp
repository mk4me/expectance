/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "camera.h"

using namespace ft;




void Camera::Init()
{
	m_pitchAngle = 20.0f;
	m_yawAngle = 0; 
	m_distance = 800.0f;
	m_camLeftRight = 0.0f;
	m_camUpDown = 0.0f;
}

void Camera::Update()
{
		//usun to stad lub zrob jak we francuskim kodzie w draw (Camera.Update, Camera.LookAt)
	// set camera position
	glTranslatef(0.0f, m_camUpDown, -m_distance);
	glRotatef(m_pitchAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(m_yawAngle, 0.0f, 1.0f, 0.0f);

}
const std::string& Camera::getID() const
{
	return m_id;
}

void Camera::setID(const std::string name)
{
	m_id = name;
}

void Camera::setPitchAngle(float angle)
{
	m_pitchAngle = angle;
}
void Camera::setYawAngle(float angle)
{
	m_yawAngle = angle;
}
void Camera::setRollAngle(float angle)
{
	m_rollAngle = angle;
}

void Camera::setDistance(float distance)
{
	m_distance = distance;
}
void Camera::setCamUpDown(float updown)
{
	m_camUpDown = updown;
}
void Camera::setCamLeftRight(float leftright)
{
	m_camLeftRight = leftright;
}


float Camera::getDistance()
{
	return m_distance;
}

float Camera::getPitchAngle()
{
	return m_pitchAngle;
}

float Camera::getYawAngle()
{
	return m_yawAngle;
}

float Camera::getRollAngle()
{
	return m_rollAngle;
}

float Camera::getCamUpDown()
{
	return m_camUpDown;
}