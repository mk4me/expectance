/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CAMERA_MANAGER_H
#define _GEN_CAMERA_MANAGER_H


#include "../core/global.h"
#include "../core/updateobject.h"
#include "../core/inputlistener.h"
#include "camera.h"
#include "cameraconfiguration.h"



namespace ft
{

	//! A Camera Manager class
	/*!
	 *	This class is responsible for global current camera operations and management of all system cameras.
	 */
	class ENGINE_API CameraManager : public UpdateObject, public InputListener
    {
    public:
        CameraManager(void) { /*empty*/}
	    virtual ~CameraManager(void) { /*empty*/}
        /// \brief singleton - Returns the only instance of Camera Manager
        static CameraManager* getInstance();
		//! destroy all resources owned by Camera Manager
        static void DestroyInstance();

		/// \brief Initializes basic Camera Manager parameters
		bool Init();

		/// \brief Updates current camera View
		void UpdateView();
        void OnUpdate(const double elapsedSeconds);  // OVERRIDEN, updates by UpdateManager

		/*! Add camera and register object to be tracked by camera */
		bool AddCamera(SceneObject *pScObj);
		/*! Add independent camera with id */
		bool AddCamera(Camera *pCamObj);

		/*! Add independent camera with id */
		bool AddCamera(std::string camName, float yaw=0.0f, float pitch=20.0f, float roll=0.0f, float dist=800.0f, CameraMode mode=ft_StaticCamera);

		//! get the pointer value of Camera object by unique id
		Camera* getCamera(std::string id);
		//! set indicated camera as current camera
		void setCurrentCamera(std::string id);
		//! set current camera according to choosen configuration
		void setCurrentCameraFromConfiguration(int key);
		//! change current camera
		void changeCurrentCamera(ft::Direction direction);
		//! remove CameraObject
		/*! Unregisters Camera object from m_cameraContainer list and deactivate it in object */
		bool RemoveCamera(Camera*);
		//! unregister Camera object
		bool RemoveCamera(std::string id);
		//! check if zoom is on
		const bool IsZoom() { return m_currentCamera->IsZoom();};


		//! get Information about current Camera
		const std::string getCurrentCameraInfo();
		//! draw/show current camera position
		const void RenderCurrentCamera();

		//! get pointer value of selected scene object
		const std::string getCurrentSceneObjectID();
		//! set pointer value to selected scene object
		const void setCurrentSceneObjectID(const std::string id);

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
        unsigned int m_currentCameraIndex;
		std::vector<std::string> m_cameraIndexContainer;
		std::string m_currentSceneObjectID;
		bool m_bLeftMouseButtonDown, m_bRightMouseButtonDown, m_bMiddleMouseButtonDown;

		//camera configuration
		CameraConfiguration m_cameraConfiguration;

    };
}

#endif //_GEN_CAMERA_MANAGER_H
