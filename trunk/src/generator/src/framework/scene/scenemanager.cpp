/*
 * Copyright (c) 2007, Future Today. All rights reserved.
 * author: abak, mka
 */

#include "scenemanager.h"


using namespace ft;

SceneManager* SceneManager::m_instance = NULL;

SceneManager* SceneManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("SceneManager::getInstace(): instance of SceneManager created ");
        m_instance = new SceneManager();
    }

    return m_instance;
}

void SceneManager::DestroyInstance()
{
    if (m_instance != NULL)
    {
    	std::map<std::string,SceneObject*>::iterator it = m_instance->g_SceneObjects.begin();
		for( ; it != m_instance->g_SceneObjects.end(); ++it ) {
			delete it->second;
	    }
	    m_instance->g_SceneObjects.clear();

        delete m_instance;
    }
}



bool SceneManager::AddObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,SceneObject*>::iterator it = g_SceneObjects.find(_id);
		if ( it!=g_SceneObjects.end()) { 
			return false;
		}
	    g_SceneObjects.insert( std::make_pair( std::string(_id), pObj ) );
	}
	return true;
}  


SceneObject * SceneManager::getObject(std::string id)
{
 	std::map<std::string,SceneObject*>::iterator it = g_SceneObjects.find(id);
	if ( it!=g_SceneObjects.end()) { 
		return it->second;
	}
	return NULL;
}


bool SceneManager::RemoveObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
	bool done;
	done = RemoveObject(_id);
	return done;
}


bool SceneManager::RemoveObject(std::string id)
{
	if (!id.empty())
	{
	 	std::map<std::string,SceneObject*>::iterator it = g_SceneObjects.find(id);
		if ( it!=g_SceneObjects.end()) 
		{ 
			delete it->second;
			g_SceneObjects.erase(it);
			return true;
		}
	}
	return false;
}

void SceneManager::Render()
{
	SceneObject *pObj;
	// iterate through the objects to find object needs rendering
	std::map<std::string,SceneObject*>::iterator it=g_SceneObjects.begin();
	for( ; it != g_SceneObjects.end(); ++it ) {
		if ((pObj = dynamic_cast<SceneObject*>(it->second))!=NULL)
		{
			// TODO mka if (pObj->Visible) 
			pObj->Render();
		}
	}
}