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
		
		TextureManager::getInstance();  // enforced creation of singleton
		Camera::getInstance();			// enforced creation of singleton
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

bool SceneManager::Init()
{
	Camera::getInstance()->Init();
	OGLContext::getInstance()->Init();
	Application::getInstance()->InitSceneObjects();
	
	if (!InitSceneObjects()) 
	{
		return false;
	}
	return true;
}

bool SceneManager::InitSceneObjects()
{

	OGLContext::getInstance()->InitNormalFloorDL(20);
	if (!OGLContext::getInstance()->InitTexturedFloorDL(20)) return false;
	if (!OGLContext::getInstance()->InitLogoDL()) return false;

	ControlManager::getInstance()->Init();

	return true;
}

void SceneManager::OnRender()
{
	OGLContext::getInstance()->RenderScene();
	RenderObjects();
	OGLContext::getInstance()->RenderLogo();

	///	RenderCursor();
	// swap the front- and back-buffer
	glutSwapBuffers();

	// increase frame counter
	ft::ControlManager::getInstance()->increraseFramesCounter();
}

void SceneManager::CleanUp()
{
	TextureManager::getInstance()->DestroyInstance();	
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

void SceneManager::RenderObjects()
{
	SceneObject *pObj;
	// iterate through the objects to find object needs rendering
	std::map<std::string,SceneObject*>::iterator it=g_SceneObjects.begin();
	for( ; it != g_SceneObjects.end(); ++it ) {
		if ((pObj = dynamic_cast<SceneObject*>(it->second))!=NULL)
		{
			// TODOMKA if (pObj->Visible) 
			pObj->Render();
		}
	}
}