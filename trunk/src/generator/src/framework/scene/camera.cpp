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
	float _x, _y, _z;
	_x = sinf(DegToRad(yaw))*dist;
	_y = sinf(DegToRad(pitch))*dist;
	_z = cosf(DegToRad(yaw))*dist;
	//m_pitchAngle = pitch;
	//m_yawAngle = yaw;
	//m_rollAngle = roll;
	//m_distance = dist;
	//m_camLeftRight = leftRight;
	//m_camUpDown = upDown;

	m_cameraMode = ft_FlyCamera;
	m_zoom = 0;
	m_camPos.set(_x, _y, _z);
	m_camUp.set(0.0, 1.0, 0.0);
	m_camAt.set(0.0, 0.0, 0.0);
	m_viewMtx = LookAtMatrix44(m_camPos, m_camAt, m_camUp);


	DefineFlyCam( m_camPos, DegToRad(yaw), DegToRad(pitch), DegToRad(roll) );
	//m_viewMtx = getFlyCameraViewMatrix();

	//tmp
	cameraSpline = new Spline("data\\spline\\curve.sdf");

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
	static int first_time = 1;

	// Distance & Speed
	static float _cameraDistance = 0.0;
	static float _targetDistance = 0.0;
	static float _cameraSpeed = 40.0;
	static float _targetSpeed = 40.0;
	// Temp
	int _index;
	// Spring 
	static vector3 _prevTrgPos(0.0, 0.0, 0.0), _currTrgPos;
	matrix44 _splineViewMtx;
	matrix44 _springViewMtx;
	float _springConstants[3] = {0.5, 2.5, 4.5};


	if (m_scObj!=NULL) //update translation from sceneobject
	{
		_currTrgPos = CalVecToVector3(m_scObj->getPosition());
	}

	// Update Distances
	_cameraDistance += (deltaTime*5) * _cameraSpeed;
    
	if(_currTrgPos!=_prevTrgPos)
	{
		_targetDistance += (deltaTime) * _targetSpeed; //if (movementMode) jesli sie nie rusza to trzeba zmienic speed na 0
		_prevTrgPos = _currTrgPos;
	}


	if (first_time) {
	//	// Ranomly Initialize Splines
	//	//for(int i = 0; i < 4; i++) {
	//	//	cameraSpline->RAND_FUNC_CAMERA(cameraSpline->controlData[i].pos);
	//	//	//RAND_FUNC_TARGET(targetSpline.controlData[i].pos);
	//	//}
		cameraSpline->Build();
	//	//targetSpline.Build();
	}

	// *** Update Spring Cmamera

	//currTrgPos = targetObjP->position + (-40.0F * targetSpline.curveData[index].tan);
	//springCameraObjP->position = 
	//	SpringDamp(springCameraObjP->position, currTrgPos, prevTrgPos, 
	//		deltaSecs, springConstants[springMode], 0.25F, 1.0F);
	//// Initialization
	//if (first_time) springCameraObjP->position = currTrgPos;
	
	_springViewMtx = LookAtMatrix44(_currTrgPos + vector3(0.0, 20.0, 400.0),
		_currTrgPos , //(-10.0F * targetSpline.curveData[index].tan),
		vector3(0.0, 1.0, 0.0));
	
	//springCameraObjP->orientation = LookAtMatrix44(vector3(0.0, 0.0, 0.0),
	//	(targetObjP->position + (-10.0F * targetSpline.curveData[index].tan)
	//		- springCameraObjP->position).normalize(),
	//	vector3(0.0, 1.0, 0.0)).invert();
	//springCameraObjP->normalMtx = InvertMatrix44(springCameraObjP->orientation);



	// *** Update Spline Camera
	_index = cameraSpline->GET_SPLINE_INDEX(cameraSpline, &_cameraDistance); 
	_splineViewMtx = LookAtMatrix44(cameraSpline->curveData[_index].pos+_currTrgPos,
		//targetObjP->position, 
		_currTrgPos,
		//vector3(0.0, 0.0, 0.0),
		vector3(0.0, 1.0, 0.0));

//	splineCameraObjP->orientation = LookAtMatrix44(vector3(0.0, 0.0, 0.0),
		//(targetObjP->position - splineCameraObjP->position).normalize(),
//		(vector3(0.0, 2.0, 0.0) - splineCameraObjP->position).normalize(),
//		vector3(0.0, 1.0, 0.0)).invert();
//	splineCameraObjP->normalMtx = InvertMatrix44(splineCameraObjP->orientation);


	// for testing purposes std::cout << deltaTime <<" elapsed time \n";

	// *** Set Proper View Matrix
	switch (m_cameraMode) {
		case ft_StaticCamera:
		if (m_scObj!=NULL) 
			m_viewMtx = LookAtMatrix44(m_camPos, _currTrgPos, m_camUp);
		else
			m_viewMtx = LookAtMatrix44(m_camPos, m_camAt, m_camUp);
		break;
		case ft_FlyCamera:
			m_viewMtx = getFlyCameraViewMatrix();
			UpdateFlyCamera(deltaTime);
		break;
		case ft_TracingCamera:
			m_viewMtx = _splineViewMtx;
		break;
		case ft_SpringCamera:
			m_viewMtx = _springViewMtx;
		break;
	}

	// No more
	first_time = 0;
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
const std::string Camera::getCameraInfo()
{
	std::string _s = "Camera <" + m_id +">: mode [" + CameraModeId[m_cameraMode] +"]";
	return _s;
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

	YT = RANGE(x, 0.0F, 1.0F, -3.141592F, 3.141592F);
	PT = RANGE(y, 0.0F, 1.0F, -1.5707F/2, 1.5707F/2);

	m_angTrg.set(YT, PT, 0.0);	
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
	if (m_key!= 255)
	{
		if (m_key=='w') m_eyeTrg += deltaTime * m_fwdS * m_fwd*40;
		if (m_key=='s') m_eyeTrg -= deltaTime * m_fwdS * m_fwd*40;
		if (m_key=='d') m_eyeTrg += deltaTime * m_fwdS * m_side*40;
		if (m_key=='a') m_eyeTrg -= deltaTime * m_fwdS * m_side*40;
		if (m_key=='r') m_eyeTrg += deltaTime * m_fwdS * m_up*40;
		if (m_key=='f') m_eyeTrg -= deltaTime * m_fwdS * m_up*40;
		m_key=255;
	}
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