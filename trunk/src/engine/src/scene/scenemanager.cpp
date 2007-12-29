/*
 * Copyright (c) 2007, Future Today. All rights reserved.
 * author: mka
 */

#include "scenemanager.h"
#include "visualizationmanager.h"

using namespace ft;

SceneManager* SceneManager::m_instance = NULL;

SceneManager* SceneManager::getInstance()
{
    if (m_instance == NULL)
    {
		_dbg << "SceneManager::getInstace(): instance of SceneManager created " << std::endl;
        m_instance = new SceneManager();
    }

    return m_instance;
}

SceneManager* SceneManager::createInstanceFirstTime()
{
    if (m_instance == NULL)
    {
        m_instance = new SceneManager();
    }
    else
    {
        if (Debug::ERR)
			_dbg << "SceneManager::createInstanceFirstTime(): instance alraedy created!!!! " << std::endl;
    }
    return m_instance;
}

void SceneManager::DestroyInstance()
{
    if (m_instance != NULL)
    {
    	std::map<std::string,SceneObject*>::iterator it = m_instance->m_SceneObjectsMap.begin();
		for( ; it != m_instance->m_SceneObjectsMap.end(); ++it ) {
			delete it->second;
	    }
	    m_instance->m_SceneObjectsMap.clear();

        delete m_instance;
    }
}



bool SceneManager::AddObject(SceneObject* pObj)
{
	bool result = false;
	std::string _id = pObj->getID();
//	Cal3DObject *_C3DObj = dynamic_cast<Cal3DObject*>(pObj);
	if (!_id.empty())
	{
	 	std::map<std::string,SceneObject*>::iterator it = m_SceneObjectsMap.find(_id);
		if ( it!=m_SceneObjectsMap.end()) { 
			return false;
		}
	    m_SceneObjectsMap.insert( std::make_pair( std::string(_id), pObj ) );

		result = VisualizationManager::getInstance()->AddObject(pObj);
//		if (_C3DObj!=NULL)
//			m_SceneGraph.push_back(_C3DObj);

  //      if (IsObjectTraceableByCamera(pObj))
  //      {
		//	CameraManager::getInstance()->AddCamera(pObj); // add camera by default to Avatar object only
		//	CameraManager::getInstance()->setCurrentSceneObjectID(_id);   // set current scene object ID to last added scene object
		//}

	}
	return result;
}  

bool SceneManager::AddDataObject(SceneObject* pObj)
{
	bool result = false;
	std::string _id = pObj->getID();
	if (!_id.empty())
	{
		result = VisualizationManager::getInstance()->AddDataObject(pObj);
	}
	return result;
}  

SceneObject* SceneManager::getObject(std::string id)
{
 	std::map<std::string,SceneObject*>::iterator it = m_SceneObjectsMap.find(id);
	if ( it!=m_SceneObjectsMap.end()) { 
		return it->second;
	}
	return NULL;
}

SceneObject* SceneManager::getObjectByName(std::string name)
{
	std::map<std::string,SceneObject*>::iterator it = m_instance->m_SceneObjectsMap.begin();
	for( ; it != m_instance->m_SceneObjectsMap.end(); ++it ) {
		if (name.compare(it->second->getName()) == 0)
			return it->second;
    }
	return NULL;
}

bool SceneManager::RemoveObject(SceneObject* pObj)
{
	bool result = false;
	std::string _id = pObj->getID();
	if (!_id.empty())
	{
		VisualizationManager::getInstance()->RemoveObject(pObj);
		result = RemoveObject(_id);
	}
	return result;
}

bool SceneManager::RemoveObject(std::string id)
{
	if (!id.empty())
	{
		std::map<std::string,SceneObject*>::iterator it = m_SceneObjectsMap.find(id);
		if ( it!=m_SceneObjectsMap.end()) 
		{ 
			delete it->second;
			m_SceneObjectsMap.erase(it);
			return true;
		}
	}
	return false;
}


bool SceneManager::RemoveDataObject(SceneObject* pObj)
{
	bool result = false;
	std::string _id = pObj->getID();
	if (!_id.empty())
	{
		result = VisualizationManager::getInstance()->RemoveDataObject(pObj);
	}
	return result;
}

bool SceneManager::RemoveDataObject(std::string id)
{
	if (!id.empty())
	{
		return VisualizationManager::getInstance()->RemoveDataObject(id);
	}
	return false;
}

void SceneManager::OnUpdate(const double elapsedSeconds)  // OVERRIDEN, updates by UpdateManager 
{
	//TODO update collision detection matrix
}

void SceneManager::UpdateDistanceMatrix()
{
	//calculate distances between every pair of dynamic object
	//SceneObject*
	//for (int
}