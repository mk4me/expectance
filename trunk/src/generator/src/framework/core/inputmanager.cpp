/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "inputmanager.h"
#include "../control/controlmanager.h"

using namespace ft;

InputManager* InputManager::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::InputManager (creates it at first call to this method)
 *
 * \return ft::InputManager * - the only instance of InputManager
 **/
InputManager* InputManager::getInstance()
{
    if (m_instance == NULL)
    {
        _dbg << "InputManager::getInstace(): instance of InputManager created " << endl;
        m_instance = new InputManager();
    }

    return m_instance;
}

/**
 * \brief Releases all resources related to this InputManager
 *
 **/
void InputManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/*-----  -----*/
/**
 * \brief Handles a key event from keyboard
 *
 * \param unsigned char key - pressed key
 * \param int x - x coordinate on the application window
 * \param int y - y coordinate on the application window
 **/
void InputManager::OnKey(unsigned char key, int x, int y)
{
 
  switch(key)
  {
    case 13: //enter
//      UpdateManager::getInstance()->SendMessage(new Message(MSG_START_SIMULATION), true);
      break;
    case 'z':
    case 'Z': 
      UpdateManager::getInstance()->SendMessage(new Message(MSG_RESTART_SIMULATION), true);
      break;
    case 't':
      UpdateManager::getInstance()->SendMessage(new Message(MSG_TEST), true);
       break;     
    case 'i':
      UpdateManager::getInstance()->SendMessage(new Message(MSG_DUMP_STATE), true);
       break;     
    // test for quit event
    case 27:
    case 'Q':
      exit(0);
      break;
    case ' ':
      UpdateManager::getInstance()->SendMessage(new Message(MSG_CONTROL_PAUSE), true);
      break;
    case '*':
      UpdateManager::getInstance()->setTimeScale(  UpdateManager::getInstance()->getTimeScale() * 1.1f);
      break;
    case '/':
      UpdateManager::getInstance()->setTimeScale(  UpdateManager::getInstance()->getTimeScale() / 1.1f);
      break;
    case 'R':
      UpdateManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_RENDER_METHOD), true);  
      break;
	case 'S':
      UpdateManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_SHADOW), true);  
  	  break;
	case 'F':
		OGLContext::getInstance()->changeFloorType();
      break;
	case 'L':
		OGLContext::getInstance()->hideFTLogo();
      break;
	case 'M':
		MenuManager::getInstance()->hideMenu();
      break;
    default:
      if((key >= '0') && (key <= '9'))
      {
        float lod = (key == '0') ? 1.0f : (key - '0') * 0.1f;
        UpdateManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_LOD, new MessageParam(lod)), true);
      }
	  CameraManager::getInstance()->OnKey(key, x, y);
      break;
  }
}


/*-----    -----*/
/**
 * \brief Handles special keys (F1, F2, UP, DOWN, etc.)
 *
 * \param int key - pressed key
 * \param int x - x coordinate on the application window
 * \param int y - y coordinate on the application window
 **/
void InputManager::OnSpecial(int key, int x, int y)
{
 /* switch(key) 
  {
    case GLUT_KEY_UP:
      //nothing
      break;
    case GLUT_KEY_DOWN:
       //nothing
       break;
    case GLUT_KEY_RIGHT:
      //nothing
      break;
    case GLUT_KEY_LEFT:
      //nothing
      break;
    default:
      break;	
  }
  */
 ControlManager::getInstance()->OnSpecial(key, x, y);
 CameraManager::getInstance()->OnSpecial(key, x, y);
  
}

/**
 * \brief Handles a mouse button down event 
 *
 * \param int button - pressed button
 * \param int x - x coordinate on the application window
 * \param int y - y coordinate on the application window
 **/
void InputManager::OnMouseButtonDown(int button, int x, int y)
{
	_dbg << "Nacisnieto button: " << button <<"w pozycji (x,y) = (" <<x<<", "<<y<<")"<<endl;
	if (y < 60)
	{
		MenuManager::getInstance()->OnMouseButtonDown(button, x, y);
	}
	else	//the rest
	{
		CameraManager::getInstance()->OnMouseButtonDown(button, x, y);
	}
}

/**
 * \brief Handles a mouse button up event
 *
 * \param int button - pressed button
 * \param int x - x coordinate on the application window
 * \param int y - y coordinate on the application window
 **/
void InputManager::OnMouseButtonUp(int button, int x, int y)
{
	if (y<60)
	{
		MenuManager::getInstance()->OnMouseButtonUp(button, x, y);
	}
	else	//the rest
	{
		CameraManager::getInstance()->OnMouseButtonUp(button, x, y);
	}
}

/**
 * \brief Handles a mouse move event
 *
 * \param int x - x coordinate on the application window
 * \param int y - y coordinate on the application window
 **/
void InputManager::OnMouseMove(int x, int y)
{
    CameraManager::getInstance()->OnMouseMove(x, y);
}