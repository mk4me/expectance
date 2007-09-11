/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "UpdateManager.h"
#include "../utility/debug.h"

using namespace ft;
using namespace std;


UpdateManager* UpdateManager::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::UpdateManager (creates it at first call to this method)
 *
 * \return ft::UpdateManager * - the only instance of UpdateManager
 **/
UpdateManager* UpdateManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("UpdateManager::getInstace(): instance of UpdateManager created ");
        m_instance = new UpdateManager();
    }

    return m_instance;
}

/**
 * \brief Initializes UpdateManager
 *
 **/
void UpdateManager::Init()
{
  m_lastTick = 0;

  m_fpsDuration = 0.0f;
  m_fpsFrames = 0;
  m_fps = 0;

  setTimeScale(1);
  ft::Simulation::setTimePrecision(ft_Microseconds); //set precision of program timer
  m_lastTick = ft::Simulation::getTick();

}

/**
 \brief Releases all resources related to UpdateManager
 *
 **/
void UpdateManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}


/**
 * \brief This method is called when new frame is updated
 *
 **/
void UpdateManager::OnUpdate()
{
  // get the current tick value
  unsigned long long tick = ft::Simulation::getTick();

  // calculate the amount of elapsed seconds
  unsigned int div =(ft::Simulation::getTimePrecision() == ft_Miliseconds)? 1000 : 1000000;

  double elapsedSeconds = (double)(tick - m_lastTick) / div;

  // adjust fps counter
  m_fpsDuration += elapsedSeconds;
  if(m_fpsDuration >= 1.0f)
  {
    m_fps = (int)((double)m_fpsFrames / m_fpsDuration);
	printf("%d\n",m_fps);
    m_fpsDuration = 0.0f;
    m_fpsFrames = 0;
  }

  elapsedSeconds *= m_timeScale;

  
  UpdateObjects(elapsedSeconds);
 
  // current tick will be last tick next round
  m_lastTick = tick;
}

/**
 * \brief This method is used to send new message to objectes registered in UpdateManager
 *
 * \param ft::Message * msg - message to send
 * \param bool deleteAfterSent - defines if message object msg should be deleted after sending
 **/
void UpdateManager::SendMessage(Message* msg, bool deleteAfterSent)
{
    if (DEBUG_MESSAGES)
        std::cout << "UpdateManager::SendMessage: "<< Message::_GET_MSG_NAME(msg->getType())  << std::endl;

    //TODO: abak:  this update should be synchronized with adding and removing UpdateObjects

   	std::map<std::string,UpdateObject*>::iterator it=m_objects.begin();
	for( ; it != m_objects.end(); ++it )
    {
        it->second->OnMessage(msg);
    }

    if (deleteAfterSent && msg != NULL)
    {
        if (msg->getParam() != NULL)
            delete msg->getParam();
        delete msg;
    }

}

/**
 * \brief Register UpdateObject in UpdateManager
 *
 * \param ft::UpdateObject * pObj - object that will be registered in UpdateManager
 * \return bool - true if registration succeed, false otherwise
 **/
bool UpdateManager::AddUpdateObject(UpdateObject* pObj)
{
	std::string _id = pObj->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,UpdateObject*>::iterator it = m_objects.find(_id);

		if ( it!=m_objects.end()) { 
            cout << "ERR: UpdateManager::AddUpdateObject object " << _id << " already added to UpdateManager " << std::endl;
			return false;
		}
	    m_objects.insert( std::make_pair( std::string(_id), pObj) );
	}
    cout << " UpdateManager::AddUpdateObject object " << _id << " added to UpdateManager " << std::endl;
	return true;
}

/**
 * \brief Unregisters UpdateObject from UpdateManager
 *
 * \param ft::UpdateObject * pObj - object that should be unregistered in UpdateManager
 * \return bool - true if object has been unregistered, false otherwise
 **/
bool UpdateManager::RemoveUpdateObject(UpdateObject* pObj)
{
	std::string _id = pObj->getID();

    if (!_id.empty())
	{
	 	std::map<std::string,UpdateObject*>::iterator it = m_objects.find(_id);
		if ( it!=m_objects.end()) { 
            m_objects.erase(it);
            cout << "UpdateManager::RemoveUpdateObject object " << _id << " removed form UpdateManager " << std::endl;
			return true;
		}
	    m_objects.insert( std::make_pair( std::string(_id), pObj) );
	}
    cout << "ERR: UpdateManager::RemoveUpdateObject object " << _id << " not found in Control Manager " << std::endl;
	return false;
}

/**
 * \brief Prints debug information describing UpdateManager on output console
 *
 **/
void UpdateManager::Dump()
{
    cout << "Dump UpdateManager content: " << std::endl;
    cout << "- objects: " << std::endl;

	std::map<std::string,UpdateObject*>::iterator it=m_objects.begin();
	for( ; it != m_objects.end(); ++it ) 
    {
        cout << " - - id " << it->first << std::endl;
    }
}

/**
 * \brief Updates all registered objects for current frame
 *
 * \param float elapsedSeconds - time elapsed since last frame (since last update)
 **/
void UpdateManager::UpdateObjects(float elapsedSeconds)
{
    //TODO: abak:  this update should be synchronized with adding and removing UpdateObjects

   	std::map<std::string,UpdateObject*>::iterator it=m_objects.begin();
	for( ; it != m_objects.end(); ++it )
    {
        it->second->OnUpdate(elapsedSeconds);
    }
}
