/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CAMERA_H
#define _GEN_CAMERA_H

#include <iostream>
#include "../utility/debug.h"
#include "../core/global.h"


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
	    ~Camera(void) { /*empty*/}

		//! get unique ID of camera object
		const std::string& getID() const;
		//! set unique ID from sceneobject instance that owns this camera
		void setID(const std::string name);

		/// \brief Initializes basic camera parameters
		void Init();
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
		//float c_debug;

		void ChangeVelocity(GLfloat vel);
		void ChangeHeading(GLfloat degrees);
		void ChangePitch(GLfloat degrees);
		void SetPrespective(void);

    private:
		
		std::string m_id;
		
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