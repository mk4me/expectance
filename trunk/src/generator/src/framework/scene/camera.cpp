/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka on the base of Dante Treglia II code from GPG, 2000
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
	m_camPos.set(0.0, 100.0, 800.0);
	m_camUp.set(0.0, 1.0, 0.0);
	m_camAt.set(0.0, 0.0, 0.0);
	
	DefineFlyCam(m_camPos, DegToRad(-45), DegToRad(-40), 0);
	m_viewMtx = getFlyCameraViewMatrix();

}

void Camera::UpdateView()
{
	//glTranslatef(0.0f, m_camUpDown, -m_distance);
	//glRotatef(m_pitchAngle, 1.0f, 0.0f, 0.0f);
	//glRotatef(m_yawAngle, 0.0f, 1.0f, 0.0f);
	glLoadMatrixf((float *)&m_viewMtx);

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

	// Update Distances
	_cameraDistance += (deltaTime) * _cameraSpeed;
    _targetDistance += (deltaTime) * _targetSpeed; //if (movementMode) jesli sie nie rusza to trzeba zmienic speed na 0


	// for testing purposes std::cout << deltaTime <<" elapsed time \n";

	// *** Set Proper View Matrix
	switch (m_cameraMode) {
		case ft_StaticCamera:
			m_viewMtx = LookAtMatrix44(m_camPos, m_camAt, m_camUp);
		break;
		case ft_FlyCamera:
			UpdateFlyCamera(deltaTime);
			m_viewMtx = getFlyCameraViewMatrix();
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
	std::cout << "Camera <" << m_id <<">: mode [" << CameraModeId[m_cameraMode] <<"] \n";
	//	", location = [" << m_position.x <<", " << m_position.y << ", " << m_position.z 
	//<<"], m_color = [" << m_color.x << ", "<<m_color.y<<", "<< m_color.z <<"] \n";
}

void Camera::ChangeZoom()
{
	m_zoom ^=1;
}

void Camera::DefineFlyCam(vector3 & eyeIn, float yIn, float pIn, float rIn)
{
	// Flying Cam
	m_eye = eyeIn;
	m_Yaw = yIn;
	m_Pitch = pIn;
	m_Roll = rIn;

	// Init
	m_fwdS = 100;
	m_eyeTrg = eyeIn + vector3(0.0, 0.0, -1.0);
	m_eyeTrgPrev = eyeIn + vector3(0.0, 0.0, -1.0);
	m_angTrg.set(0.0, 0.0, 0.0);
	m_angTrgPrev.set(0.0, 0.0, 1.0);

}


const matrix44 Camera::getFlyCameraViewMatrix()
{
	float cosY, cosP, cosR;
	float sinY, sinP, sinR;

	// Compute Current Info

	// Only Want to Calc these once
	cosY = cosf(m_Yaw);
	cosP = cosf(m_Pitch);
	cosR = cosf(m_Roll);
	sinY = sinf(m_Yaw);
	sinP = sinf(m_Pitch);
	sinR = sinf(m_Roll);

	// Fwd Vector
	m_fwd.x = sinY * cosP;
	m_fwd.y = sinP;
	m_fwd.z = cosP * -cosY;

	// Look At Point
	m_at = m_fwd + m_eye;

	// Up Vector
	m_up.x = -cosY * sinR - sinY * sinP * cosR;
	m_up.y = cosP * cosR;
	m_up.z = -sinY * sinR - sinP * cosR * -cosY;

	// Side Vector (right)
	m_side = CrossProduct(m_fwd, m_up);

	// View matrix
	return LookAtMatrix44(m_eye, m_at, m_up);
}

void Camera::OnMouseMove(float x, float y)
{
	float YT, PT;

	YT = RANGE(x, 0.0F, 1.0F, -3.1415F, 3.1415F);
	PT = RANGE(y, 0.0F, 1.0F, 1.5707F, -1.5707F);

	m_angTrg.set(YT, -PT, 0.0);	
}

void Camera::OnKey(unsigned char key)
{
	m_key = key;
}

void Camera::UpdateFlyCamera(const double deltaTime)
{
	// Damp Angles
	vector3 _currAng = SpringDamp(vector3(m_Yaw, m_Pitch, 0.0), m_angTrg, m_angTrgPrev, deltaTime, 4.5F, 0.5F, 0.0001F);
	m_Yaw = _currAng.x;
	m_Pitch = _currAng.y;
	m_angTrgPrev = m_angTrg;

	// Store Info
	m_eyeTrgPrev = m_eyeTrg;

	// Keyboard Data
		if (GetKeyState('W') & 0x80) m_eyeTrg += deltaTime * m_fwdS * m_fwd;
		if (GetKeyState('S') & 0x80) m_eyeTrg -= deltaTime * m_fwdS * m_fwd;
		if (GetKeyState('D') & 0x80) m_eyeTrg += deltaTime * m_fwdS * m_side;
		if (GetKeyState('A') & 0x80) m_eyeTrg -= deltaTime * m_fwdS * m_side;
		//if (GetKeyState(VK_UP) & 0x80) m_eyeTrg += deltaTime * m_fwdS * m_fwd;
		//if (GetKeyState(VK_DOWN) & 0x80) m_eyeTrg -= deltaTime * m_fwdS * m_fwd;
		//if (GetKeyState(VK_RIGHT) & 0x80) m_eyeTrg += deltaTime * m_fwdS * m_side;
		//if (GetKeyState(VK_LEFT) & 0x80) m_eyeTrg -= deltaTime * m_fwdS * m_side;
		if (GetKeyState('R') & 0x80) m_eyeTrg += deltaTime * m_fwdS * m_up;
		if (GetKeyState('F') & 0x80) m_eyeTrg -= deltaTime * m_fwdS * m_up;
		////if (m_key!= 255)
		////{
		////if (m_key=='w') m_eyeTrg += deltaTime * m_fwdS * m_fwd;
		////if (m_key=='s') m_eyeTrg -= deltaTime * m_fwdS * m_fwd;
		////if (m_key=='d') m_eyeTrg += deltaTime * m_fwdS * m_side;
		////if (m_key=='a') m_eyeTrg -= deltaTime * m_fwdS * m_side;
		////if (m_key=='r') m_eyeTrg += deltaTime * m_fwdS * m_up;
		////if (m_key=='f') m_eyeTrg -= deltaTime * m_fwdS * m_up;
		////m_key=255;
		////}
	m_eye = SpringDamp(m_eye, m_eyeTrg, m_eyeTrgPrev, deltaTime, 2.5F, 0.5F, 1.0F);	
}

//-------------------------------------------------------------------------
// An asymptotic effect returns a float that approaches targetX from currX 
//-------------------------------------------------------------------------
float Camera::Damp(float currX, float targetX) {
	return currX + ((targetX - currX) / 16.0F);
}

//---------------------------------------------------------------------------
// Spring Damp Function
//---------------------------------------------------------------------------
vector3 Camera::SpringDamp(
		vector3 currPos,    // Current Position
		vector3 trgPos,     // Target Position
		vector3 prevTrgPos, // Previous Target Position
		const double deltaTime,    // Change in Time
		float springConst,  // Hooke's Constant
		float dampConst,    // Damp Constant
		float springLen) 
{
	vector3 disp;           // Displacement
	vector3 velocity;       // Velocity   
	float forceMag;         // Force Magnitude

	// Calculate Spring Force
	disp = currPos - trgPos;
	velocity = (prevTrgPos - trgPos) * (float)deltaTime;
	forceMag = springConst * (springLen - disp.length()) + 
		dampConst * (DotProduct(disp, velocity) / disp.length());

	// Apply Spring Force
	disp.normalize();
	disp *= forceMag * (float)deltaTime;
	return currPos += disp;
}