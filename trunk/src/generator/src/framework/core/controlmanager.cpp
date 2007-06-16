/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "controlmanager.h"
#include "../utility/debug.h"

using namespace ft;
using namespace std;


ControlManager* ControlManager::m_instance = NULL;

ControlManager* ControlManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("ControlManager::getInstace(): instance of ControlManager created ");
        m_instance = new ControlManager();
    }

    return m_instance;
}

void ControlManager::Init()
{
  m_lastTick = 0;

  m_fpsDuration = 0.0f;
  m_fpsFrames = 0;
  m_fps = 0;

  setTimeScale(1);

  m_lastTick = ft::Simulation::GetTick();

}

void ControlManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}


void ControlManager::OnUpdate()
{
      // get the current tick value
  unsigned int tick = ft::Simulation::GetTick();

  // calculate the amount of elapsed seconds
  float elapsedSeconds = (float)(tick - m_lastTick) / 1000.0f;

  // adjust fps counter
  m_fpsDuration += elapsedSeconds;
  if(m_fpsDuration >= 1.0f)
  {
    m_fps = (int)((float)m_fpsFrames / m_fpsDuration);
	printf("%d\n",m_fps);
    m_fpsDuration = 0.0f;
    m_fpsFrames = 0;
  }

  elapsedSeconds *= m_timeScale;

  
  UpdateObjects(elapsedSeconds);
 
  // current tick will be last tick next round
  m_lastTick = tick;
}

void ControlManager::SendMessage(const Message& msg)
{
}

bool ControlManager::AddControlObject(ControlObject* pObj)
{
	std::string _id = pObj->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,ControlObject*>::iterator it = m_objects.find(_id);

		if ( it!=m_objects.end()) { 
            cout << "ERR: ControlManager::AddControlObject object " << _id << " already added to ControlManager " << std::endl;
			return false;
		}
	    m_objects.insert( std::make_pair( std::string(_id), pObj) );
	}
    cout << " ControlManager::AddControlObject object " << _id << " added to ControlManager " << std::endl;
	return true;
}

bool ControlManager::RemoveControlObject(ControlObject* pObj)
{
	std::string _id = pObj->getID();

    if (!_id.empty())
	{
	 	std::map<std::string,ControlObject*>::iterator it = m_objects.find(_id);
		if ( it!=m_objects.end()) { 
            m_objects.erase(it);
            cout << "ControlManager::RemoveControlObject object " << _id << " removed form ControlManager " << std::endl;
			return true;
		}
	    m_objects.insert( std::make_pair( std::string(_id), pObj) );
	}
    cout << "ERR: ControlManager::RemoveControlObject object " << _id << " not found in Control Manager " << std::endl;
	return false;

}

void ControlManager::Dump()
{
    cout << "Dump ControlManager content: " << std::endl;
    cout << "- objects: " << std::endl;

	std::map<std::string,ControlObject*>::iterator it=m_objects.begin();
	for( ; it != m_objects.end(); ++it ) 
    {
        cout << " - - id " << it->first << std::endl;
    }
  
}

void ControlManager::UpdateObjects(float elapsedSeconds)
{
    //TODO: abak:  this update should be synchronized with adding and removing ControlObjects

   	std::map<std::string,ControlObject*>::iterator it=m_objects.begin();
	for( ; it != m_objects.end(); ++it )
    {
        it->second->OnUpdate(elapsedSeconds);
    }
}
