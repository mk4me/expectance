/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CAMERA_CONFIGURATION_H
#define _GEN_CAMERA_CONFIGURATION_H

#include "../core/global.h"
#include <map>

namespace ft
{
	enum CameraType { ft_ActiveAvatar, ft_MainCamera };
	enum CameraLocation {ft_FrontLeft, ft_FrontCenter, ft_FrontRight, ft_Left, ft_Center, ft_Right, ft_BackLeft, ft_BackCenter, ft_BackRight,
						 ft_TopFrontLeft, ft_TopFrontCenter, ft_TopFrontRight, ft_TopLeft, ft_TopCenter, ft_TopRight, ft_TopBackLeft, 
						 ft_TopBackCenter, ft_TopBackRight, ft_BottomFrontLeft, ft_BottomFrontCenter, ft_BottomFrontRight, ft_BottomLeft,
						 ft_BottomCenter, ft_BottomRight, ft_BottomBackLeft, ft_BottomBackCenter, ft_BottomBackRight, ft_AutoLocation
						 };

	typedef struct CameraParameters
	{
		int hotKey;
		CameraType type;
		CameraLocation location;
	}CameraDefinition;
	//! A Camera class
	/*!
	 *	This class is responsible for schemas definition for cameras. 
	 */
	class CameraConfiguration
	{
	public:
		CameraConfiguration(void) { /*empty*/};
		virtual ~CameraConfiguration(void) {/*empty*/};

		/// \brief reads confguration from the file sets current configuration
		void Init();
		

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

		std::map<std::string, CameraDefinition> m_cameraConfigurationContainer; 

	};
};
#endif //_GEN_CAMERA_CONFIGURATION_H