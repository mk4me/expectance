/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "inputmanager.h"

using namespace ft;

InputManager* InputManager::m_instance = NULL;

InputManager* InputManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("InputManager::getInstace(): instance of InputManager created ");
        m_instance = new InputManager();
    }

    return m_instance;
}

void InputManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/*----- Handle a key event -----*/
void InputManager::OnKey(unsigned char key, int x, int y)
{
 
  switch(key)
  {
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
      ControlManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_RENDER_METHOD), true);  
      break;
	case 's':
      ControlManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_SHADOW), true);  
  	  break;
    case 'f':
		OGLContext::getInstance()->changeFloorType();
      break;
    default:
      if((key >= '0') && (key <= '9'))
      {
        float lod = (key == '0') ? 1.0f : (key - '0') * 0.1f;
        ControlManager::getInstance()->SendMessage(new Message(MSG_PROPERTY_LOD, new MessageParam(lod)), true);
      }
      break;
  }
}


/*----- Handle special keys (F1, F2, UP, DOWN, etc.)   -----*/
void InputManager::OnSpecial(int key, int x, int y)
{
  switch(key) 
  {
    case GLUT_KEY_UP:
      ControlManager::getInstance()->SendMessage(new Message(MSG_CONTROL_START, NULL), true);
      break;
    case GLUT_KEY_DOWN:
        ControlManager::getInstance()->SendMessage(new Message(MSG_CONTROL_STOP, NULL), true);
       break;
    case GLUT_KEY_RIGHT:
//      m_camheight += 3.0f;
      break;
    case GLUT_KEY_LEFT:
//      m_camheight -= 3.0f;
      break;
    default:
		Camera::getInstance()->OnSpecial(key, x, y);
      break;	
  }
}

/*----- Handle a mouse button down event -----*/
void InputManager::OnMouseButtonDown(int button, int x, int y)
{
    Camera::getInstance()->OnMouseButtonDown(button, x, y);
}

/*----- Handle a mouse button up event -----*/
void InputManager::OnMouseButtonUp(int button, int x, int y)
{
    Camera::getInstance()->OnMouseButtonUp(button, x, y);
}


/*----- Handle a mouse move event -----*/
void InputManager::OnMouseMove(int x, int y)
{
    Camera::getInstance()->OnMouseMove(x, y);
}