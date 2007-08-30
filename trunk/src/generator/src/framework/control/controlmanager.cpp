/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "controlmanager.h"
#include "../utility/debug.h"

using namespace ft;
using namespace std;

ControlManager* ControlManager::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::ControlManager (creates it at first call to this method)
 *
 * \return ft::ControlManager * - the only instance of ControlManager
 **/
ControlManager* ControlManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("ControlManager::getInstace(): instance of ControlManager created ");
        m_instance = new ControlManager();
    }

    return m_instance;
}

/**
 \brief Releases all resources related to ControlManager
 *
 **/
void ControlManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * \brief Initializes ControlManager
 *
 **/
void ControlManager::Init()
{
    setActiveAvatar(NULL);
}

/**
 * \brief Called from InputManager when special keys pressed (F1, F2, UP, DOWN, etc.)
 *
 * \param int key - pressed key
 * \param int x - x coordinate on the application window
 * \param int y - y coordinate on the application window
 **/
void ControlManager::OnSpecial(int key, int x, int y)
{
  int controlMessage = -1;
  switch(key) 
  {
    case GLUT_KEY_UP:
        controlMessage = MSG_CONTROL_START;
        break;
    case GLUT_KEY_DOWN:
        controlMessage = MSG_CONTROL_STOP;
        break;
    case GLUT_KEY_RIGHT:
        controlMessage = MSG_CONTROL_TURN_RIGHT;
        break;
    case GLUT_KEY_LEFT:
        controlMessage = MSG_CONTROL_TURN_LEFT;
        break;
    default:
        break;	
  }

  if (controlMessage != -1 && getActiveAvatar() != NULL)
  {
      UpdateManager::getInstance()->SendMessage(new Message(controlMessage, new MessageParam(getActiveAvatar()->getName())), true);
  }
}


