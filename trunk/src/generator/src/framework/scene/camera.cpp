/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "camera.h"

using namespace ft;

Camera* Camera::m_instance = NULL;

/**
 * getInstance: <describe the responsibilities and behavior of this method>
 *
 * \return ft::Camera * <describe what is returned if appropriate>
 **/
Camera* Camera::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("Camera::getInstace(): instance of Camera created ");
        m_instance = new Camera();
    }

    return m_instance;
}

/**
 * DestroyInstance: <describe the responsibilities and behavior of this method>
 *
 **/
void Camera::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * Init: <describe the responsibilities and behavior of this method>
 *
 **/
void Camera::Init()
{
	m_pitchAngle = 20.0f;
	m_yawAngle = 0; 
	m_distance = 800.0f;
	m_camLeftRight = 0.0f;
	m_camUpDown = 0.0f;
	m_mouseX = 0;
	m_mouseY = 0;
	m_bLeftMouseButtonDown = false;
	m_bRightMouseButtonDown = false;
	m_bMiddleMouseButtonDown = false;

	//c_debug = 0.0;
}

/*----- Handle a key event -----*/
/**
 * OnKey: <describe the responsibilities and behavior of this method>
 *
 * \param unsigned char key <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void Camera::OnKey(unsigned char key, int x, int y)
{
  //switch(key)
  //{
  //  // test for .... everything
  // //// case 'o':
  // ////   c_debug+=0.1;if (c_debug>1.0) c_debug=0.0;
	 // ////std::cout << "info -> "<<c_debug<< std::endl;
  // ////   break;
  // //// case 'p':
  // ////   c_debug-=0.1;if (c_debug<0.0) c_debug=1.0;
	 // ////std::cout << "blending -> "<<c_debug<< std::endl;
  // ////   break;
  //  default:
  //    break;
  //}
}


/*----- Handle special keys (F1, F2, UP, DOWN, etc.)   -----*/
/**
 * OnSpecial: <describe the responsibilities and behavior of this method>
 *
 * \param int key <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void Camera::OnSpecial(int key, int x, int y)
{

}

/*----- Handle a mouse button down event -----*/
/**
 * OnMouseButtonDown: <describe the responsibilities and behavior of this method>
 *
 * \param int button <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void Camera::OnMouseButtonDown(int button, int x, int y)
{
	// update mouse button states
	if(button == GLUT_LEFT_BUTTON)
	{
	m_bLeftMouseButtonDown = true;
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
	m_bRightMouseButtonDown = true;
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
	m_bMiddleMouseButtonDown = true;
	}
	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;    
}

/*----- Handle a mouse button up event -----*/
/**
 * OnMouseButtonUp: <describe the responsibilities and behavior of this method>
 *
 * \param int button <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void Camera::OnMouseButtonUp(int button, int x, int y)
{
	// update mouse button states
	if(button == GLUT_LEFT_BUTTON)
	{
		m_bLeftMouseButtonDown = false;
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		m_bRightMouseButtonDown = false;
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
		m_bMiddleMouseButtonDown = false;
	}

	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;
}


/*----- Handle a mouse move event -----*/
/**
 * OnMouseMove: <describe the responsibilities and behavior of this method>
 *
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void Camera::OnMouseMove(int x, int y)
{
	// update pitch/yaw angles
	if(m_bLeftMouseButtonDown)
	{
		// calculate new angles
		m_yawAngle += (float)(x - m_mouseX);
		m_pitchAngle -= (float)(y - m_mouseY);
	}

	// update distance
	if(m_bRightMouseButtonDown)
	{
		// calculate new distance
		m_distance -= (float)(y - m_mouseY); 
		if(m_distance < 0.0f) m_distance = 0.0f;
	}

	// update Y screen position
	if(m_bMiddleMouseButtonDown)
	{
		//calculate new Y position
		m_camUpDown +=(float)(y-m_mouseY);	
	}	
	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;
}

/**
 * getDistance: <describe the responsibilities and behavior of this method>
 *
 * \return float <describe what is returned if appropriate>
 **/
float Camera::getDistance()
{
	return m_distance;
}

/**
 * getPitchAngle: <describe the responsibilities and behavior of this method>
 *
 * \return float <describe what is returned if appropriate>
 **/
float Camera::getPitchAngle()
{
	return m_pitchAngle;
}

/**
 * getYawAngle: <describe the responsibilities and behavior of this method>
 *
 * \return float <describe what is returned if appropriate>
 **/
float Camera::getYawAngle()
{
	return m_yawAngle;
}

/**
 * getRollAngle: <describe the responsibilities and behavior of this method>
 *
 * \return float <describe what is returned if appropriate>
 **/
float Camera::getRollAngle()
{
	return m_rollAngle;
}

/**
 * getCamUpDown: <describe the responsibilities and behavior of this method>
 *
 * \return float <describe what is returned if appropriate>
 **/
float Camera::getCamUpDown()
{
	return m_camUpDown;
}