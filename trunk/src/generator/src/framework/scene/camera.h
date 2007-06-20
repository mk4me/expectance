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
    class Camera
    {
    public:
        Camera(void) { /*empty*/}
	    virtual ~Camera(void) { /*empty*/}
        
        static Camera* getInstance();
        static void DestroyInstance();
		
		void Init();
        void OnKey(unsigned char key, int x, int y);
		void OnSpecial(int key, int x, int y);
        void OnMouseButtonDown(int button, int x, int y);
        void OnMouseButtonUp(int button, int x, int y);
        void OnMouseMove(int x, int y);
		float getDistance();
		float getPitchAngle();
		float getYawAngle();
		float getRollAngle();
		float getCamUpDown();

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