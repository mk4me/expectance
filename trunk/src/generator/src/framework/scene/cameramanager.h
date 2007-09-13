/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CAMERA_MANAGER_H
#define _GEN_CAMERA_MANAGER_H

#include <iostream>
#include <algorithm>
#include <map>
#include "../utility/debug.h"
#include "../core/global.h"
#include "camera.h"


namespace ft
{
	enum Direction { ft_Forward, ft_Backward };
	
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

		/*! Add camera and register object to be tracked by camera */
		bool AddCamera(SceneObject *pScObj);   
		/*! Add independent camera with id */
		bool AddCamera(Camera *pCamObj);   
		
		/*! Add independent camera with id */
		bool AddCamera(std::string camName, float pitch=20.0f, float yaw=0.0f, float roll=0.0f, float dist=800.0f, float leftRight=0.0f, float upDown=0.0f);   

		//! get the pointer value of Camera object by unique id
		Camera* getCamera(std::string id);
		//! set indicated camera as current camera
		void setCurrentCamera(std::string id);
		//! change current camera 
		void changeCurrentCamera(ft::Direction direction);
		//! remove CameraObject
		/*! Unregisters Camera object from m_cameraContainer list and deactivate it in object */
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

    private:
		/// \brief Helper for getting current camera index
		int getCurrentCameraIndex();
		/// \brief Helper for setting current camera index
		void setCurrentCameraIndex(const std::string id);
		/// \brief Removes camera index while camera is removed from main Camera Container
		void RemoveCameraIndex(const std::string id);

		static CameraManager* m_instance;
		Camera* m_currentCamera;
		std::map<std::string,Camera*> m_cameraContainer;
		
		//helpers for numerical indexing camera container
		int m_currentCameraIndex;
		std::vector<std::string> m_cameraIndexContainer;

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