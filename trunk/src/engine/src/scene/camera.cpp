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

void Camera::Init(float yaw, float pitch, float roll, float dist, CameraMode mode)
{
	float _x, _y, _z;
	_x = sinf(DegToRad(yaw))*dist;
	_y = sinf(DegToRad(pitch))*dist;
	_z = cosf(DegToRad(yaw))*dist;

	setYaw(yaw); setPitch(pitch); setRoll(roll); setDistance(dist); setCameraMode(mode);

	m_tracingRadius = 500.0f;
	m_zoom = 0;
	m_camPos.set(_x, _y, _z);
	m_camUp.set(0.0, 1.0, 0.0);
	m_camAt.set(0.0, 0.0, 0.0);

	DefineFlyCam( m_camPos, DegToRad(yaw), DegToRad(pitch), DegToRad(roll) );
	m_viewMtx = getFlyCameraViewMatrix();

	m_viewMtx = LookAtMatrix44(m_camPos, m_camAt, m_camUp);

	//tmp
    std::string splinePath = FT_DATA_PATH + "spline/curve.sdf";
    cameraSpline = new Spline(splinePath.c_str());
}

void Camera::UpdateView()
{

	glLoadMatrixf((float *)&m_viewMtx);

}

void  Camera::Render()
{
	static vector3 _pos(0.0,0.0,0.0); //default position definition
	static float _tempAng = -1.57079633f;
	float _alpha, _r, _g, _b;
    _alpha = _r =_g = _b = 0.0f;

	if ( (m_scObj!=NULL) || (m_id=="mainCamera") ) //update translation from sceneobject
	{
		if (_tempAng <= 1.57079633f)
			_tempAng += 0.02f;
		else
			_tempAng = -1.57079633f;

		_alpha = cos(_tempAng);

		// draw grid for indicating camera target
		int _sc=40, _st=10;
		if (m_id == "mainCamera")
		{
			_sc = 1280; _st = 420;
			_pos.set(0.0,0.0,0.0);
			_r=0.6f;
		}
		else
		{
			_pos = CalVecToVector3(m_scObj->getPosition());
			_r=0.7f;
		}

		switch(m_cameraMode)
		{
			case ft_FlyCamera: _r=0.0f; _g=1.0f; _b=1.0f; //blue
				break;
			case ft_StaticCamera: _r=0.0f; _g=1.0f; _b=0.0f; //green
				break;
			case ft_OrbitCamera: _r=1.0f; _g=0.0f; _b=0.5f; //red
				break;
			case ft_ThirdPersonCamera: _g=_r/2; _b=0.0f; //leave it orange
				break;
		}

		_pos.y = 0;
		vector3 _p1 = _pos+vector3(_sc,0,_sc), _p2 = _pos+vector3(_sc,0,-_sc),_p3= _pos+vector3(-_sc,0,-_sc),_p4=_pos+vector3(-_sc,0,_sc);
		glPushMatrix();
			glDisable(GL_CULL_FACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glLineWidth(1.0f);
			glColor4d(_r, _g, _b, _alpha);
			glBegin(GL_LINES);
				glVertex3f(_p1.x-_st, _p1.y, _p1.z);
				glVertex3f(_p1.x, _p1.y, _p1.z);
				glVertex3f(_p1.x, _p1.y, _p1.z-_st);
				glVertex3f(_p1.x, _p1.y, _p1.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(_p2.x, _p2.y, _p2.z+_st);
				glVertex3f(_p2.x, _p2.y, _p2.z);
				glVertex3f(_p2.x-_st, _p2.y, _p2.z);
				glVertex3f(_p2.x, _p2.y, _p2.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(_p3.x+_st, _p3.y, _p3.z);
				glVertex3f(_p3.x, _p3.y, _p3.z);
				glVertex3f(_p3.x, _p3.y, _p3.z+_st);
				glVertex3f(_p3.x, _p3.y, _p3.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(_p4.x, _p4.y, _p4.z-_st);
				glVertex3f(_p4.x, _p4.y, _p4.z);
				glVertex3f(_p4.x+_st, _p4.y, _p4.z);
				glVertex3f(_p4.x, _p4.y, _p4.z);
			glEnd();
			glLineWidth(1.0f);
			glDisable(GL_BLEND);
			glEnable(GL_CULL_FACE);
		glPopMatrix();
	}
}

void Camera::OnUpdate(const double deltaTime)
{
	static int first_time = 1;

	// Distance & Speed
	static float _cameraDistance = 0.0;
	//static float _targetDistance = 0.0;
	static float _cameraSpeed = 40.0;
	static float _targetSpeed = 40.0;
	// Temp
	int _index;
	// Spring
	static vector3 _prevTrgPos(0.0, 0.0, 0.0), _currTrgPos;
	matrix44 _orbitViewMtx;
	matrix44 _thirdPersonViewMtx;


	if (m_scObj!=NULL) //update translation from sceneobject
	{
		_currTrgPos = CalVecToVector3(m_scObj->getPosition());
		_currTrgPos.y = 100; //(glutGet (GLUT_WINDOW_HEIGHT))/4; // against waving camera according to y avatars coordinate waving y=100 by default is enough too
	}

	// Update Distances
	_cameraDistance += (deltaTime*5) * _cameraSpeed;
	//_targetDistance += (deltaTime) * _targetSpeed; //if (movementMode) jesli sie nie rusza to trzeba zmienic speed na 0

	if(_currTrgPos!=_prevTrgPos)
	{
		_prevTrgPos = _currTrgPos;
	}


	// *** Update Spring Cmamera
	_thirdPersonViewMtx = LookAtMatrix44(_currTrgPos + vector3(0.0, 100.0, 400.0), _currTrgPos , vector3(0.0, 1.0, 0.0));


	// *** Update Spline Camera
	_index = cameraSpline->getSplineIndex(cameraSpline, &_cameraDistance, m_tracingRadius);
	_orbitViewMtx = LookAtMatrix44(cameraSpline->curveData[_index].pos+_currTrgPos, _currTrgPos, vector3(0.0, 1.0, 0.0));

	// for testing purposes _dbg << deltaTime <<" elapsed time \n";

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
		case ft_OrbitCamera:
			m_viewMtx = _orbitViewMtx;
		break;
		case ft_ThirdPersonCamera:
			m_viewMtx = _thirdPersonViewMtx;
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
	if ((m_scObj==NULL)&&(m_cameraMode == ft_ThirdPersonCamera))  //choose once more for camera without object to trace
		m_cameraMode = static_cast<ft::CameraMode>( (static_cast<unsigned short>(m_cameraMode) +1) % 4 );
}

const void Camera::PrintInfo() const
{
    if (Debug::CAMERA>0)
	    _dbg << "Camera <" << m_id <<">: mode [" << CameraModeId[m_cameraMode] <<"] \n";

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

void Camera::setOrbitCameraRadius(float radius)
{
	m_tracingRadius = radius;
}


const float Camera::getYaw() const
{
	return m_Yaw;
}

const float Camera::getPitch() const
{
	return m_Pitch;
}

const float Camera::getRoll() const
{
	return m_Roll;
}

const float Camera::getDistance() const
{
	return m_distance;
}

void Camera::setYaw(const float yaw)
{
	m_Yaw = yaw;
}

void Camera::setPitch(const float pitch)
{
	m_Pitch = pitch;
}

void Camera::setRoll(const float roll)
{
	m_Roll = roll;
}

void Camera::setDistance(const float distance)
{
	m_distance = distance;
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
