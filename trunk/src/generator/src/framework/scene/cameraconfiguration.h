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
	enum CameraType { ft_ActiveAvatar, ft_MainCamera };
	enum CameraLocation {ft_FrontLeft, ft_FrontCenter, ft_FrontRight, ft_Left, ft_Center, ft_Right, ft_BackLeft, ft_BackCenter, ft_BackRight,
						 ft_TopFrontLeft, ft_TopFrontCenter, ft_TopFrontRight, ft_TopLeft, ft_TopCenter, ft_TopRight, ft_TopBackLeft, 
						 ft_TopBackCenter, ft_TopBackRight, ft_BottomFrontLeft, ft_BottomFrontCenter, ft_BottomFrontRight, ft_BottomLeft,
						 ft_BottomCenter, ft_BottomRight, ft_BottomBackLeft, ft_BottomBackCenter, ft_BottomBackRight, ft_AutoLocation
						 };


	inline CameraLocation GETCameraLocation( const std::string& Key )
	 {
	  std::map<std::string, CameraLocation> CL;

	  CL[ "ft_FrontLeft" ]=ft_FrontLeft; CL[ "ft_FrontCenter" ]=ft_FrontCenter; CL[ "ft_FrontRight" ]=ft_FrontRight; 
	  CL[ "ft_Left" ]=ft_Left; CL[ "ft_Center" ]=ft_Center; CL[ "ft_Right" ]=ft_Right; CL[ "ft_BackLeft" ]=ft_BackLeft;
	  CL[ "ft_BackCenter" ]=ft_BackCenter; CL[ "ft_BackRight" ]=ft_BackRight;   

	  CL[ "ft_TopFrontLeft" ]=ft_TopFrontLeft; CL[ "ft_TopFrontCenter" ]=ft_TopFrontCenter; CL[ "ft_TopFrontRight" ]=ft_TopFrontRight; 
	  CL[ "ft_TopLeft" ]=ft_TopLeft; CL[ "ft_TopCenter" ]=ft_TopCenter; CL[ "ft_TopRight" ]=ft_TopRight; CL[ "ft_TopBackLeft" ]=ft_TopBackLeft;
	  CL[ "ft_TopBackCenter" ]=ft_TopBackCenter; CL[ "ft_TopBackRight" ]=ft_TopBackRight;   

  	  CL[ "ft_BottomFrontLeft" ]=ft_BottomFrontLeft; CL[ "ft_BottomFrontCenter" ]=ft_BottomFrontCenter; 
	  CL[ "ft_BottomFrontRight" ]=ft_BottomFrontRight; CL[ "ft_BottomLeft" ]=ft_Left; CL[ "ft_BottomCenter" ]=ft_BottomCenter; 
	  CL[ "ft_BottomRight" ]=ft_BottomRight; CL[ "ft_BottomBackLeft" ]=ft_BottomBackLeft; CL[ "ft_BottomBackCenter" ]=ft_BottomBackCenter; 
	  CL[ "ft_BottomBackRight" ]=ft_BottomBackRight; CL[ "ft_AutoLocation" ]=ft_AutoLocation;  

	  return CL[ Key ];
	 };

	inline CameraType GETCameraType( const std::string& Key )
	{
		std::map<std::string, CameraType> CT;
		CT[ "ft_ActiveAvatar" ]=ft_ActiveAvatar; CT[ "ft_MainCamera" ]=ft_MainCamera; 
		return CT[ Key ];
	};
	
	inline CameraMode GETCameraMode( const std::string& Key )
	{
		std::map<std::string, CameraMode> CM;
		CM[ "ft_StaticCamera" ]=ft_StaticCamera; CM[ "ft_ThirdPersonCamera" ]=ft_ThirdPersonCamera; 
		CM[ "ft_FlyCamera" ]=ft_FlyCamera; CM[ "ft_OrbitCamera" ]=ft_OrbitCamera; 
		return CM[ Key ];
	};

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