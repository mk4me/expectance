/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CAMERA_H
#define _GEN_CAMERA_H

#include <iostream>
#include "../utility/debug.h"
#include "../core/global.h"
#include "sceneobject.h"

namespace ft
{
	//! A Camera class
	/*!
	 *	This class is responsible for global camera operations. 
	 */
    class Camera
    {
    public:
        Camera(void) { /*empty*/}
		Camera(SceneObject *pScObj);
		Camera(std::string id);
		~Camera(void) { /*empty*/}

		//! get unique ID of camera object
		const std::string& getID() const;

		/// \brief Initializes basic camera parameters
	    void Init(float pitch=20.0f, float yaw=0.0f, float roll=0.0f, float dist=800.0f, float leftRight=0.0f, float upDown=0.0f);
		void Update();
		void setPitchAngle(float angle);
		void setYawAngle(float angle);
		void setRollAngle(float angle);
		void setDistance(float distance);
		void setCamUpDown(float updown);
		void setCamLeftRight(float leftright);
		
		//! get the value of distance camera from middle of the scene
		float getDistance();
		//! get the value of Pitch Angle
		float getPitchAngle();
		//! get the value of Yaw Angle
		float getYawAngle();
		//! get the value of Roll Angle
		float getRollAngle();
		//! get the value of Camera Up-Down factor
		float getCamUpDown();
		//! get the value of Camera Left-Right factor
		float getCamLeftRight();

		void ChangeVelocity(GLfloat vel);
		void ChangeHeading(GLfloat degrees);
		void ChangePitch(GLfloat degrees);
		void SetPrespective(void);

    private:
		//! set unique ID from sceneobject instance that owns this camera
		void setID(const std::string name);
		
		std::string m_id;
		SceneObject *m_scObj; // camera for scene object

		GLfloat m_MaxPitchRate;         // max OX
		GLfloat m_MaxHeadingRate;       // max OY
		GLfloat m_HeadingDegrees;       // OY
		GLfloat m_PitchDegrees;         // OX
		GLfloat m_MaxForwardVelocity;	// max Vel
		GLfloat m_ForwardVelocity;
		//glQuaternion m_qHeading;
		//glQuaternion m_qPitch;
		//glPoint m_Position;
		//glVector m_DirectionVector;





		float m_pitchAngle; // OX
		float m_yawAngle;   // OY
		float m_rollAngle;  // OZ
		float m_distance;
		float m_camLeftRight, m_camUpDown;
    };
};

#endif //_GEN_CAMERA_H