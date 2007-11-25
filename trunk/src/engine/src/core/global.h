//----------------------------------------------------------------------------//
// global.h                                                                   //
// Copyright (C) 2007 Future Today                                            //
//                                                                            //
//----------------------------------------------------------------------------//


#ifndef GLOBAL_H
#define GLOBAL_H


#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <map>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include "../scene/GL/glew.h"
#include <GL/glut.h>

#include "cal3d/cal3d.h"
#include "../scene/textureManager.h"

namespace ft
{
	static const std::string FT_SHADERPATH = "data\\shaders\\";
	static const std::string FT_TEXTUREPATH = "data\\textures\\";
	static const std::string FT_MODELPATH = "data\\models\\";

	static char *CameraModeId[] = {"Static Camera", "3rd Person Camera", "Fly Camera", "Orbit Camera"};


	/***************************************************************************************************/ 
	// system enumerations
	/***************************************************************************************************/ 

	enum TimePrecision {ft_Microseconds, ft_Miliseconds};

	enum CameraType { ft_ActiveAvatarCamera, ft_MainCamera };
	
	enum CameraLocation {ft_FrontLeft, ft_FrontCenter, ft_FrontRight, ft_Left, ft_Center, ft_Right, ft_BackLeft, ft_BackCenter, ft_BackRight,
						 ft_TopFrontLeft, ft_TopFrontCenter, ft_TopFrontRight, ft_TopLeft, ft_TopCenter, ft_TopRight, ft_TopBackLeft, 
						 ft_TopBackCenter, ft_TopBackRight, ft_BottomFrontLeft, ft_BottomFrontCenter, ft_BottomFrontRight, ft_BottomLeft,
						 ft_BottomCenter, ft_BottomRight, ft_BottomBackLeft, ft_BottomBackCenter, ft_BottomBackRight, ft_AutoLocation
						 };

	//StaticCamera - rigid camera without moves, static viewport only 
	//ThirdPersonCamera - Third person camera following target without stearing mode
	//FlyCamera - dynamic viewport with possibility of moving
	//OrbitCamera - camera turning around the target point
	//                       0                 1               2              3
	enum CameraMode { ft_StaticCamera, ft_ThirdPersonCamera, ft_FlyCamera, ft_OrbitCamera };
	enum Direction { ft_Forward, ft_Backward };
	enum MarkerShape { ft_Point, ft_Diamond, ft_Cross };

	enum KeyCodes {ft_F1, ft_F2, ft_F3 ,ft_F4, ft_F5 ,ft_F6, ft_F7, ft_F8, ft_F9, ft_F10, ft_F11, ft_F12, ft_KEY_LEFT, ft_KEY_UP,
				   ft_KEY_RIGHT, ft_KEY_DOWN, ft_KEY_PAGE_UP, ft_KEY_PAGE_DOWN, ft_KEY_HOME, ft_KEY_END, ft_KEY_INSERT};

	/***************************************************************************************************/ 
	//inline mapping functions for configuration reading purposes
	/***************************************************************************************************/ 
	
	inline CameraLocation GETCameraLocation( const std::string& Key )
	{
		  std::map<std::string, CameraLocation> CL;

		  CL[ "ft_FrontLeft" ] = ft_FrontLeft;  CL[ "ft_FrontCenter" ] = ft_FrontCenter; CL[ "ft_FrontRight" ] = ft_FrontRight; 
		  CL[ "ft_Left" ]      = ft_Left;       CL[ "ft_Center" ]      = ft_Center;      CL[ "ft_Right" ]      = ft_Right; 
		  CL[ "ft_BackLeft" ]  = ft_BackLeft;   CL[ "ft_BackCenter" ]  = ft_BackCenter;  CL[ "ft_BackRight" ]  = ft_BackRight;   

		  CL[ "ft_TopFrontLeft" ] = ft_TopFrontLeft; CL[ "ft_TopFrontCenter" ] = ft_TopFrontCenter; CL[ "ft_TopFrontRight" ] = ft_TopFrontRight; 
		  CL[ "ft_TopLeft" ]      = ft_TopLeft;      CL[ "ft_TopCenter" ]      = ft_TopCenter;      CL[ "ft_TopRight" ]      = ft_TopRight; 
		  CL[ "ft_TopBackLeft" ]  = ft_TopBackLeft;  CL[ "ft_TopBackCenter" ]  = ft_TopBackCenter;  CL[ "ft_TopBackRight" ]  = ft_TopBackRight;   

		  CL[ "ft_BottomFrontLeft" ]  = ft_BottomFrontLeft;  CL[ "ft_BottomFrontCenter" ] = ft_BottomFrontCenter; 
		  CL[ "ft_BottomFrontRight" ] = ft_BottomFrontRight; CL[ "ft_BottomLeft" ]        = ft_Left; 
		  CL[ "ft_BottomCenter" ]     = ft_BottomCenter;     CL[ "ft_BottomRight" ]       = ft_BottomRight;      
		  CL[ "ft_BottomBackLeft" ]   = ft_BottomBackLeft;   CL[ "ft_BottomBackCenter" ]  = ft_BottomBackCenter; 
		  CL[ "ft_BottomBackRight" ]  = ft_BottomBackRight;  CL[ "ft_AutoLocation" ]      = ft_AutoLocation;  

		  return CL[ Key ];
	 };

	inline CameraType GETCameraType( const std::string& Key )
	{
		std::map<std::string, CameraType> CT;
		CT[ "ft_ActiveAvatarCamera" ] = ft_ActiveAvatarCamera; CT[ "ft_MainCamera" ] = ft_MainCamera; 
		return CT[ Key ];
	};

	inline CameraMode GETCameraMode( const std::string& Key )
	{
		std::map<std::string, CameraMode> CM;
		CM[ "ft_StaticCamera" ] = ft_StaticCamera; CM[ "ft_ThirdPersonCamera" ] = ft_ThirdPersonCamera; 
		CM[ "ft_FlyCamera" ]    = ft_FlyCamera;    CM[ "ft_OrbitCamera" ]       = ft_OrbitCamera; 
		return CM[ Key ];
	};

	inline int GETKeyCode( const std::string& Key ) //codes according to GLUT kodes table
	{
		std::map<std::string, int> KC;
		KC["ft_F1"] = 1; KC["ft_F2"] = 2; KC["ft_F3"] = 3;  KC["ft_F4"]  = 4;   KC["ft_F5"]  = 5;  KC["ft_F6"]  = 6; 
		KC["ft_F7"] = 7; KC["ft_F8"] = 8; KC["ft_F9"] = 9;  KC["ft_F10"] = 10;  KC["ft_F11"] = 11; KC["ft_F12"] = 12;

		KC["ft_KEY_LEFT"]    = 100; KC["ft_KEY_UP"]        = 101; KC["ft_KEY_RIGHT"] = 102; KC["ft_KEY_DOWN"] = 103;
		KC["ft_KEY_PAGE_UP"] = 104;	KC["ft_KEY_PAGE_DOWN"] = 105; KC["ft_KEY_HOME"]  = 106;	KC["ft_KEY_END"]  = 107;
		KC["ft_KEY_INSERT"]  = 108;
		
		return KC[ Key ];
	};


};
#endif



