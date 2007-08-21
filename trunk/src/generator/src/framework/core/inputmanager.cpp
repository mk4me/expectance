/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "inputmanager.h"

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
        DBG("InputManager::getInstace(): instance of InputManager created ");
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
    case 't':
      ControlManager::getInstance()->SendMessage(new Message(MSG_TEST), true);
       break;     
    // test for quit event
    case 27:
    case 'q':
    case 'Q':
      exit(0);
      break;
    case ' ':
      ControlManager::getInstance()->SendMessage(new Message(MSG_CONTROL_PAUSE), true);
      break;
    case '*':
      ControlManager::getInstance()->setTimeScale(  ControlManager::getInstance()->getTimeScale() * 1.1f);
      break;
    case '/':
      ControlManager::getInstance()->setTimeScale(  ControlManager::getInstance()->getTimeScale() / 1.1f);
      break;
    case 'r':
    case 'R':
      ControlManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_RENDER_METHOD), true);  
      break;
	case 's':
	case 'S':
      ControlManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_SHADOW), true);  
  	  break;
    case 'f':
	case 'F':
		OGLContext::getInstance()->changeFloorType();
      break;
	case 'l':
	case 'L':
		OGLContext::getInstance()->hideFTLogo();
      break;
	case 'm':
	case 'M':
		VisualizationManager::getInstance()->hideMenu();
      break;
    default:
      if((key >= '0') && (key <= '9'))
      {
        float lod = (key == '0') ? 1.0f : (key - '0') * 0.1f;
        ControlManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_LOD, new MessageParam(lod)), true);
      }
	  Camera::getInstance()->OnKey(key, x, y);
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
  switch(key) 
  {
    case GLUT_KEY_UP:
      ControlManager::getInstance()->SendMessage(new Message(MSG_CONTROL_START, new MessageParam("FirstAvatar")), true);
      break;
    case GLUT_KEY_DOWN:
       ControlManager::getInstance()->SendMessage(new Message(MSG_CONTROL_STOP, new MessageParam("FirstAvatar")), true);
       break;
    case GLUT_KEY_RIGHT:
      ControlManager::getInstance()->SendMessage(new Message(MSG_CONTROL_TURN_RIGHT, new MessageParam("FirstAvatar")), true);
      break;
    case GLUT_KEY_LEFT:
      ControlManager::getInstance()->SendMessage(new Message(MSG_CONTROL_TURN_LEFT, new MessageParam("FirstAvatar")), true);
      break;
    default:
		Camera::getInstance()->OnSpecial(key, x, y);
      break;	
  }
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
	std::cout << "Nacisnieto button: " << button <<"w pozycji (x,y) = (" <<x<<", "<<y<<")"<<endl;
	if (y < 60)
	{
		MenuManager::getInstance()->OnMouseButtonDown(button, x, y);
	}
	else	//the rest
	{
		Camera::getInstance()->OnMouseButtonDown(button, x, y);
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
		Camera::getInstance()->OnMouseButtonUp(button, x, y);
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
    Camera::getInstance()->OnMouseMove(x, y);
}