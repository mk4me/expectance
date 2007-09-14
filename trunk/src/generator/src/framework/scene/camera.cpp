/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "camera.h"

using namespace ft;

Camera::Camera(ft::SceneObject *pScObj)
{
	if (pScObj != NULL)
	{
		m_scObj = pScObj;
		setID(pScObj->getID());
		Init();
	}
	else
	{
		Camera("NULL_CAM");
	}
}

Camera::Camera(std::string id)
{
	Init();
	setID(id);
	m_scObj = NULL;
}

void Camera::Init(float pitch, float yaw, float roll, float dist, float leftRight, float upDown)
{
	m_pitchAngle = pitch;
	m_yawAngle = yaw;
	m_rollAngle = roll;
	m_distance = dist;
	m_camLeftRight = leftRight;
	m_camUpDown = upDown;
	m_cameraMode = ft_FlyCamera;
	m_zoom = 0;
}

void Camera::UpdateView()
{
	glTranslatef(0.0f, m_camUpDown, -m_distance);
	glRotatef(m_pitchAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(m_yawAngle, 0.0f, 1.0f, 0.0f);	
}

void Camera::OnUpdate(const double deltaTime)
{
	// Distance & Speed
	static float _cameraDistance = 0.0;
	static float _targetDistance = 0.0;
	static float _cameraSpeed = 40.0;
	static float _targetSpeed = 40.0;
	// Time
	float _deltaSecs;
	// Temp
	int _index;
	// Spring 
	static vector3 _prevTrgPos(0.0, 0.0, 0.0);
	matrix44 _splineViewMtx;
	matrix44 _springViewMtx;
	float _springConstants[3] = {0.5, 2.5, 4.5};



	// for testing purposes std::cout << deltaTime <<" elapsed time \n";

	// *** Set Proper View Matrix
	switch (m_cameraMode) {
		case ft_StaticCamera:
			//m_viewMtx = LookAtMatrix44(camPos, camAt, camUp);
		break;
		case ft_FlyCamera:
			//cameraP->Tick(deltaSecs);
			//m_viewMtx = cameraP->GetViewMtx();
		break;
		case ft_TracingCamera:
			//m_viewMtx = m_splineViewMtx;
		break;
		case ft_SpringCamera:
			//m_viewMtx = m_springViewMtx;
		break;
	}

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

float Camera::getCamLeftRight()
{
	return m_camLeftRight;
}


void Camera::setCameraMode(const ft::CameraMode mode)
{
	m_cameraMode = mode;
}

const ft::CameraMode Camera::getCameraMode()
{
	return m_cameraMode;
}

void Camera::ChangeCameraMode()
{
	m_cameraMode = static_cast<ft::CameraMode>( (static_cast<unsigned short>(m_cameraMode) +1) % 4 );
}

const void Camera::PrintInfo() const
{
	std::cout << "Camera <" << m_id <<">: mode [" << m_cameraMode <<"] \n";
	//	", location = [" << m_position.x <<", " << m_position.y << ", " << m_position.z 
	//<<"], m_color = [" << m_color.x << ", "<<m_color.y<<", "<< m_color.z <<"] \n";
}

void Camera::ChangeZoom()
{
	m_zoom ^=1;
}