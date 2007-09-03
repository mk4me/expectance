/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CAMERA_MANAGER_H
#define _GEN_CAMERA_MANAGER_H

#include <iostream>
#include <map>
#include "../utility/debug.h"
#include "../core/global.h"
#include "camera.h"


namespace ft
{
	//! A Camera Manager class
	/*!
	 *	This class is responsible for global current camera operations and management of all system cameras. 
	 */
    class CameraManager
    {
    public:
        CameraManager(void) { /*empty*/}
	    virtual ~CameraManager(void) { /*empty*/}
        /// \brief singleton - Returns the only instance of Camera Manager
        static CameraManager* getInstance();
		//! destroy all resources owned by Camera Manager
        static void DestroyInstance();
		
		/// \brief Initializes basic Camera Manager parameters
		void Init();
		
		/*! Registers object to be tracked by camera */
		bool AddCamera(ft::Camera* pObj);   
		//! get the pointer value of Camera object by unique id
		Camera* getCamera(std::string id);
		//! remove CameraObject
		/*! Unregisters Camera object from m_CameraContainer list and deactivate it in object */
		bool RemoveCamera(Camera*);
		//! unregister Camera object
		bool RemoveCamera(std::string id);

		/// \brief Updates current camera parameters
		void Update();


		/// \brief Handles a key event
        void OnKey(unsigned char key, int x, int y);
		/// \brief Handles special keys (F1, F2, UP, DOWN, etc.)
		void OnSpecial(int key, int x, int y);
		/// \brief Handles a mouse button down event 
        void OnMouseButtonDown(int button, int x, int y);
        /// \brief Handles a mouse button up event
		void OnMouseButtonUp(int button, int x, int y);
        /// \brief Handles a mouse move event 
		void OnMouseMove(int x, int y);
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
    private:
        static CameraManager* m_instance;
		Camera* m_currentCamera;
		Camera* m_sceneCamera;
		std::map<std::string,Camera*> m_CameraContainer;

		float m_pitchAngle; // OX
		float m_yawAngle;   // OY
		float m_rollAngle;  // OZ
		float m_distance;
		float m_camLeftRight, m_camUpDown;
		int m_mouseX, m_mouseY;
		bool m_bLeftMouseButtonDown, m_bRightMouseButtonDown, m_bMiddleMouseButtonDown;

    };
};

#endif //_GEN_CAMERA_MANAGER_H