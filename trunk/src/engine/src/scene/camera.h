/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CAMERA_H
#define _GEN_CAMERA_H

#include <iostream>
#include "../utility/mtxlib.h"
#include "../utility/debug.h"
#include "../core/global.h"
#include "sceneobject.h"
#include "spline.h"

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
	    void Init(float yaw=0.0f, float pitch=20.0f, float roll=0.0f, float dist=1000.0f, CameraMode mode=ft_StaticCamera);
		//! calculate and set Viewport for current camera settings
		void UpdateView();
		//! draw current target
		void  Render();
		//! calculate view matrix for current camera
		void OnUpdate(const double deltaTime);
		//! set camera mode
		void setCameraMode(const ft::CameraMode mode);
		//! get camera mode
		const ft::CameraMode getCameraMode();
		//! change camera mode
		void ChangeCameraMode();
		/// \brief Prints general information about values of object properties  
		const void PrintInfo() const;
		
		//! gets Information about Camera
		const std::string getCameraInfo();
		//! turn on or turn off zoom
		void ChangeZoom();
		//! check if zoom is on
		const bool IsZoom() { return static_cast<bool>(m_zoom);};
		
		///\brief Creates Fly camera
		void DefineFlyCam(vector3 & eyeIn, float yIn, float pIn, float rIn);
		///\brief Computes parameters for Fly Camera
		//void ComputeFlyCameraParameters();
		///\brief Computes viewing matrix according to flight
		const matrix44 getFlyCameraViewMatrix();
		///\brief Computes mouse affection on camera
		void OnMouseMove(float x, float y);
		///\brief Sets pressed key on the keyboard
		void OnKey(unsigned char key);
		///\brief Updates fly camera paraeters
		void UpdateFlyCamera(const double deltaTime);

		///\brief Sets radius for tracing camera
		void setOrbitCameraRadius(float radius=500.0f);
		
		//! get the value of yaw angle
		const float getYaw() const;
		//! get the value of pich angle
		const float getPitch() const;
		//! get the value of roll angle
		const float getRoll() const;
		//! get the value of distance
		const float getDistance() const;
		//! set the value of yaw angle
		void setYaw(const float yaw);
		//! set the value of pitch angle
		void setPitch(const float pitch);
		//! set the value of roll angle
		void setRoll(const float roll);
		//! set the value of distance
		void setDistance(const float distance);

	private:
		//! set unique ID from sceneobject instance that owns this camera
		void setID(const std::string name);
		
		vector3 SpringDamp(vector3 currPos, vector3 trgPos, vector3 prevTrgPos, 
			const double deltaTime, float springConst, float dampConst, float springLen);


		std::string m_id;
		SceneObject *m_scObj; // camera for scene object

		ft::CameraMode m_cameraMode;
		bool m_zoom;
		unsigned char m_key;

		matrix44 m_viewMtx;
		vector3 m_camPos, m_camUp, m_camAt;
		
		// Position (Point)
		vector3 m_eye;
		// Forward (Point) (IE LookAt)
		vector3 m_at;
		// Up (Vector)
		vector3 m_up;
		// Forward (Vector) (IE View Direction)
		vector3 m_fwd;
		// Side (right)
		vector3 m_side;

		// Orientation data
		// Yaw, Pitch, and Roll
		float m_Yaw, m_Pitch, m_Roll;
		vector3 m_angTrg;
		vector3 m_angTrgPrev;

		// Forward Speed
		float m_fwdS;

		// Damp Targets
		vector3 m_eyeTrg;
		vector3 m_eyeTrgPrev;

		float m_tracingRadius;


		float m_pitchAngle; // OX
		float m_yawAngle;   // OY
		float m_rollAngle;  // OZ
		float m_distance;
		float m_camLeftRight, m_camUpDown;
		
		//temporary
		Spline *cameraSpline;
    };


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#define RANGE(x, minX, maxX, minY, maxY) \
	((((x - minX) * (maxY - minY))/(maxX - minX)) + minY)


}

#endif //_GEN_CAMERA_H