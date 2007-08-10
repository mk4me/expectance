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
	    virtual ~Camera(void) { /*empty*/}
        /// \brief singleton - Returns the only instance of Camera
        static Camera* getInstance();
		//! destroy all resources owned by Camera
        static void DestroyInstance();
		
		/// \brief Inits basic camera parameters
		void Init();
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
        static Camera* m_instance;

		float m_pitchAngle; // OX
		float m_yawAngle;   // OY
		float m_rollAngle;  // OZ
		float m_distance;
		float m_camLeftRight, m_camUpDown;
		int m_mouseX, m_mouseY;
		bool m_bLeftMouseButtonDown, m_bRightMouseButtonDown, m_bMiddleMouseButtonDown;

    };
};

#endif //_GEN_CAMERA_H