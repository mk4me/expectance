/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "inputmanager.h"

using namespace ft;

InputManager* InputManager::m_instance = NULL;

/**
 * getInstance: <describe the responsibilities and behavior of this method>
 *
 * \return ft::InputManager * <describe what is returned if appropriate>
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
 * DestroyInstance: <describe the responsibilities and behavior of this method>
 *
 **/
void InputManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/*----- Handle a key event -----*/
/**
 * OnKey: <describe the responsibilities and behavior of this method>
 *
 * \param unsigned char key <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
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
		SceneManager::getInstance()->hideMenu();
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


/*----- Handle special keys (F1, F2, UP, DOWN, etc.)   -----*/
/**
 * OnSpecial: <describe the responsibilities and behavior of this method>
 *
 * \param int key <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
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

/*----- Handle a mouse button down event -----*/
/**
 * OnMouseButtonDown: <describe the responsibilities and behavior of this method>
 *
 * \param int button <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
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

/*----- Handle a mouse button up event -----*/
/**
 * OnMouseButtonUp: <describe the responsibilities and behavior of this method>
 *
 * \param int button <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
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


/*----- Handle a mouse move event -----*/
/**
 * OnMouseMove: <describe the responsibilities and behavior of this method>
 *
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void InputManager::OnMouseMove(int x, int y)
{
    Camera::getInstance()->OnMouseMove(x, y);
}