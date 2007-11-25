/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CAMERA_CONFIGURATION_H
#define _GEN_CAMERA_CONFIGURATION_H

#include <map>
#include <sstream>
#include "../core/global.h"
#include "../core/config.h"
#include "../utility/stringhelper.h"
#include "camera.h"


namespace ft
{


	class CameraDefinition
	{
		public:
			int hotKey;
			CameraType type;
			CameraMode mode;
			CameraLocation location;
			
			//! get unique ID of camera object
			const std::string& getID() const{return m_id;};
			void setID(const std::string & id) {m_id = id;};
		private:
			std::string m_id;
	};
	
	//! A Camera class
	/*!
	 *	This class is responsible for schemas definition for cameras. 
	 */
	class CameraConfiguration
	{
	public:
		CameraConfiguration(void) { /*empty*/};
		virtual ~CameraConfiguration(void);

		/// \brief reads confguration from the file sets current configuration
		bool Init();
		bool AddConfiguration(CameraDefinition *pCamObj);	
		bool AddConfiguration(int hotKey, CameraType type, CameraMode mode, CameraLocation location);
		CameraDefinition* getConfiguration(std::string id);
		CameraDefinition* getConfiguration(int id);

		/// \brief 
//2.definition for camera in configuration file
//-read, parse configuration
//3.operations on camera configuration (from CameraManager):
//-preparePredefinedCameraConfiguration (read from file, parse, createCameras or connections to existing cameras)
//-setPredefinedCameraConfiguration (change existing camera parameters according to requirements)
//-getPredefinedCameraConfiguration (get camera configuration on the base of given key - get camera from existing camera pool, and parameters from camera configuration and prepare camera on the base of that parameters)
//
//-setCurrentCameraConfiguration
	private:
		CameraDefinition m_currentConfiguration; //first configuration from the list

		std::map<std::string, CameraDefinition*> m_cameraConfigurationContainer; 

	};
};
#endif //_GEN_CAMERA_CONFIGURATION_H