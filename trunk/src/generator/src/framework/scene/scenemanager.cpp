/*
 * Copyright (c) 2007, Future Today. All rights reserved.
 * author: abak, mka
 */

#include "scenemanager.h"

using namespace ft;

SceneManager* SceneManager::m_instance = NULL;

/**
 * getInstance: <describe the responsibilities and behavior of this method>
 *
 * \return ft::SceneManager * <describe what is returned if appropriate>
 **/
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

/**
 * DestroyInstance: <describe the responsibilities and behavior of this method>
 *
 **/
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

/**
 * Init: <describe the responsibilities and behavior of this method>
 *
 * \return bool <describe what is returned if appropriate>
 **/
bool SceneManager::Init()
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

/**
 * InitSceneObjects: <describe the responsibilities and behavior of this method>
 *
 * \return bool <describe what is returned if appropriate>
 **/
bool SceneManager::InitSceneObjects()
{

	OGLContext::getInstance()->InitNormalFloorDL(20);
	if (!OGLContext::getInstance()->InitTexturedFloorDL(20)) return false;
	if (!OGLContext::getInstance()->InitLogoDL()) return false;

///MKATODO	ControlManager::getInstance()->AddControlObject(MenuManager::getInstance());

	return true;
}

/**
 * OnRender: <describe the responsibilities and behavior of this method>
 *
 **/
void SceneManager::OnRender()
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

/**
 * CleanUp: <describe the responsibilities and behavior of this method>
 *
 **/
void SceneManager::CleanUp()
{
	TextureManager::getInstance()->DestroyInstance();	
}


/**
 * AddObject: <describe the responsibilities and behavior of this method>
 *
 * \param ft::SceneObject * pObj <argument description>
 * \return bool <describe what is returned if appropriate>
 **/
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


/**
 * getObject: <describe the responsibilities and behavior of this method>
 *
 * \param std::string id <argument description>
 * \return ft::SceneObject * <describe what is returned if appropriate>
 **/
SceneObject * SceneManager::getObject(std::string id)
{
 	std::map<std::string,SceneObject*>::iterator it = g_SceneObjects.find(id);
	if ( it!=g_SceneObjects.end()) { 
		return it->second;
	}
	return NULL;
}


/**
 * getObjectByName: <describe the responsibilities and behavior of this method>
 *
 * \param std::string name <argument description>
 * \return ft::SceneObject * <describe what is returned if appropriate>
 **/
SceneObject * SceneManager::getObjectByName(std::string name)
{
	std::map<std::string,SceneObject*>::iterator it = m_instance->g_SceneObjects.begin();
	for( ; it != m_instance->g_SceneObjects.end(); ++it ) {
		if (name.compare(it->second->getName()) == 0)
			return it->second;
    }
	return NULL;
}

/**
 * RemoveObject: <describe the responsibilities and behavior of this method>
 *
 * \param ft::SceneObject *  <argument description>
 * \return bool <describe what is returned if appropriate>
 **/
bool SceneManager::RemoveObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
	bool done;
	done = RemoveObject(_id);
	return done;
}


/**
 * RemoveObject: <describe the responsibilities and behavior of this method>
 *
 * \param std::string id <argument description>
 * \return bool <describe what is returned if appropriate>
 **/
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

/**
 * RenderObjects: <describe the responsibilities and behavior of this method>
 *
 **/
void SceneManager::RenderObjects()
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

/**
 * hideMenu: <describe the responsibilities and behavior of this method>
 *
 **/
void SceneManager::hideMenu()
{
	m_menuOGL = !m_menuOGL;
}