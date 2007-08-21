/*
 * Copyright (c) 2007, Future Today. All rights reserved.
 * author: abak, mka
 */

#include "visualizationmanager.h"

using namespace ft;

VisualizationManager* VisualizationManager::m_instance = NULL;

VisualizationManager* VisualizationManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("VisualizationManager::getInstace(): instance of VisualizationManager created ");
        m_instance = new VisualizationManager();
		
		TextureManager::getInstance();  // enforced creation of singleton
		Camera::getInstance();			// enforced creation of singleton
    }

    return m_instance;
}

void VisualizationManager::DestroyInstance()
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


bool VisualizationManager::Init()
{
	Camera::getInstance()->Init();
	if (!OGLContext::getInstance()->Init())
	{
		return false;
	}

	if (!InitSceneObjects()) 
	{
		return false;
	}

	MenuManager::getInstance()->Init(0,0); 

	m_menuOGL = true; //show OGL menu by default

	return true;
}


bool VisualizationManager::InitSceneObjects()
{

	OGLContext::getInstance()->InitNormalFloorDL(20);
	if (!OGLContext::getInstance()->InitTexturedFloorDL(20)) return false;
	if (!OGLContext::getInstance()->InitLogoDL()) return false;

	return true;
}


void VisualizationManager::OnRender()
{
	OGLContext::getInstance()->RenderScene();
	RenderObjects();
	//render 2D
	OGLContext::getInstance()->Render2D();
	if (m_menuOGL)
		MenuManager::getInstance()->Render();

	///	RenderCursor();
	// swap the front- and back-buffer
	glutSwapBuffers();

	// increase frame counter
	ft::ControlManager::getInstance()->increraseFramesCounter();
}

void VisualizationManager::CleanUp()
{
	TextureManager::getInstance()->DestroyInstance();	
}


bool VisualizationManager::AddObject(SceneObject* pObj)
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


SceneObject * VisualizationManager::getObject(std::string id)
{
 	std::map<std::string,SceneObject*>::iterator it = g_SceneObjects.find(id);
	if ( it!=g_SceneObjects.end()) { 
		return it->second;
	}
	return NULL;
}

SceneObject * VisualizationManager::getObjectByName(std::string name)
{
	std::map<std::string,SceneObject*>::iterator it = m_instance->g_SceneObjects.begin();
	for( ; it != m_instance->g_SceneObjects.end(); ++it ) {
		if (name.compare(it->second->getName()) == 0)
			return it->second;
    }
	return NULL;
}

bool VisualizationManager::RemoveObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
	bool done;
	done = RemoveObject(_id);
	return done;
}

bool VisualizationManager::RemoveObject(std::string id)
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


void VisualizationManager::RenderObjects()
{
	SceneObject *pObj;
	// iterate through the objects to find object needs rendering
	std::map<std::string,SceneObject*>::iterator it=g_SceneObjects.begin();
	for( ; it != g_SceneObjects.end(); ++it ) {
		if ((pObj = dynamic_cast<SceneObject*>(it->second))!=NULL)
		{
			if (pObj->isVisible()) 
			pObj->Render();
		}
	}
}


void VisualizationManager::hideMenu()
{
	m_menuOGL = !m_menuOGL;
}