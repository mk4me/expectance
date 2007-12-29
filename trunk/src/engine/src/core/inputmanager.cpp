/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "inputmanager.h"
#include "../utility/debug.h"

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
		_dbg << "InputManager::getInstace(): instance of InputManager created " << std::endl;
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

/**
 * \brief Adds listener  to InputManager
 * \param ft::InputListener* listener - listener to add
 * \return bool - true if listener added successfuly
 **/
void InputManager::AddListener(InputListener* listener)
{
    if (Debug::INPUT>0)
        _dbg << " InputManager::AddListener " << listener << std::endl;

    m_vListeners.push_back(listener);
}

bool  InputManager::RemoveListener(InputListener* listener)
{
    bool result = false;

    std::vector<InputListener*>::iterator iteratorListener;
    iteratorListener = m_vListeners.begin();

    while(iteratorListener != m_vListeners.end())
    {
        // find the specified action and remove it
        if((*iteratorListener) == listener)
        {
            // found, so remove
            m_vListeners.erase(iteratorListener);
            result = true;
            break;
        }
        iteratorListener++;
    }

        if (Debug::INPUT>0)
        {
            if (result)
                _dbg << " InputManager::RemoveListener " << listener << " removed " << std::endl;
             else
                _dbg << " InputManager::RemoveListener " << listener << " - removing FAILED " << std::endl;
        }

    return result;
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
    if (m_vListeners.size() > 0)
    {
        for (int m=0; m<(int)m_vListeners.size(); m++)
        {
            m_vListeners[m]->OnKey(key, x, y);
        }
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
    if (m_vListeners.size() > 0)
    {
        for (int m=0; m<(int)m_vListeners.size(); m++)
        {
            m_vListeners[m]->OnSpecial(key, x, y);
        }
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
    if (Debug::INPUT>0)
		_dbg << "Nacisnieto button: " << button <<"w pozycji (x,y) = (" <<x<<", "<<y<<")"<< std::endl;

    if (m_vListeners.size() > 0)
    {
        for (int m=0; m<(int)m_vListeners.size(); m++)
        {
            m_vListeners[m]->OnMouseButtonDown(button, x, y);
        }
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
    if (m_vListeners.size() > 0)
    {
        for (int m=0; m<(int)m_vListeners.size(); m++)
        {
            m_vListeners[m]->OnMouseButtonUp(button, x, y);
        }
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
    if (m_vListeners.size() > 0)
    {
        for (int m=0; m<(int)m_vListeners.size(); m++)
        {
            m_vListeners[m]->OnMouseMove(x, y);
        }
    }
}