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
        cout << "VisualizationManager::getInstace(): instance of VisualizationManager created " << endl;
        m_instance = new VisualizationManager();
		
		TextureManager::getInstance();  // enforced creation of singleton
		CameraManager::getInstance();			// enforced creation of singleton
    }

    return m_instance;
}

void VisualizationManager::DestroyInstance()
{
    if (m_instance != NULL)
    {
    	std::map<std::string,SceneObject*>::iterator it = m_instance->m_SceneObjects.begin();
		for( ; it != m_instance->m_SceneObjects.end(); ++it ) {
			delete it->second;
	    }
	    m_instance->m_SceneObjects.clear();

        delete m_instance;
    }
}


bool VisualizationManager::Init()
{
	if (!CameraManager::getInstance()->Init())
	{
		return false;
	}

	if (!OGLContext::getInstance()->Init())
	{
		return false;
	}

	if (!InitSceneObjects()) 
	{
		return false;
	}

	if (!MenuManager::getInstance()->Init(0,0))
	{
		return false;
	}
	
	if(Config::getInstance()->IsKey("camera_marker_visible"))
	{
		m_ActiveCameraMarker = Config::getInstance()->GetIntVal("camera_marker_visible");
	}
	else
		m_ActiveCameraMarker = 0; // no active camera marker visible

	return true;
}


bool VisualizationManager::InitSceneObjects()
{

	OGLContext::getInstance()->InitNormalFloorDL(20);
	if (!OGLContext::getInstance()->InitTexturedFloorDL(20)) return false;
	if (!OGLContext::getInstance()->InitLogoDL()) return false;

	//init 4 scene cameras
	//CameraManager::getInstance()->AddCamera("frontLeft",20,45,0.0,1000);
	//CameraManager::getInstance()->AddCamera("backLeft",20,135,0.0,1000);
	//CameraManager::getInstance()->AddCamera("backRight",20,225,0.0,1000);
	//CameraManager::getInstance()->AddCamera("frontRight",20,315,0.0,1000);

	return true;
}


void VisualizationManager::OnRender()
{
	OGLContext::getInstance()->setPerspective( CameraManager::getInstance()->IsZoom() ); 
	CameraManager::getInstance()->UpdateView(); //update current camera View
	
	OGLContext::getInstance()->RenderScene(); 
	if (m_ActiveCameraMarker)
		CameraManager::getInstance()->RenderCurrentCamera(); //show avatar's camera position
	Render3DObjects();
	Render2DObjects();
	// swap the front- and back-buffer
	glutSwapBuffers();

	// increase frame counter
	ft::UpdateManager::getInstance()->increraseFramesCounter();
}

void VisualizationManager::CleanUp()
{
	TextureManager::getInstance()->DestroyInstance();	
}


bool VisualizationManager::AddObject(SceneObject* pObj)
{
	SceneObject *pScObj;
	std::string _id = pObj->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,SceneObject*>::iterator it = m_SceneObjects.find(_id);
		if ( it!=m_SceneObjects.end()) { 
			return false;
		}
	    m_SceneObjects.insert( std::make_pair( std::string(_id), pObj ) );

		if ((pScObj = dynamic_cast<Avatar*>(pObj))!=NULL)
		{
			CameraManager::getInstance()->AddCamera(pObj); // add camera by default to Avatar object only
			CameraManager::getInstance()->setCurrentSceneObjectID(_id);   // set current scene object ID to last added scene object
		}

	}
	return true;
}  


SceneObject* VisualizationManager::getObject(std::string id)
{
 	std::map<std::string,SceneObject*>::iterator it = m_SceneObjects.find(id);
	if ( it!=m_SceneObjects.end()) { 
		return it->second;
	}
	return NULL;
}

SceneObject* VisualizationManager::getObjectByName(std::string name)
{
	std::map<std::string,SceneObject*>::iterator it = m_instance->m_SceneObjects.begin();
	for( ; it != m_instance->m_SceneObjects.end(); ++it ) {
		if (name.compare(it->second->getName()) == 0)
			return it->second;
    }
	return NULL;
}

bool VisualizationManager::RemoveObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
	bool done;	
	CameraManager::getInstance()->RemoveCamera(_id); // remove pointer to camera from erased object
	done = RemoveObject(_id);
	return done;
}

bool VisualizationManager::RemoveObject(std::string id)
{
	if (!id.empty())
	{
	 	std::map<std::string,SceneObject*>::iterator it = m_SceneObjects.find(id);
		if ( it!=m_SceneObjects.end()) 
		{ 
			delete it->second;
			m_SceneObjects.erase(it);
			return true;
		}
	}
	return false;
}


void VisualizationManager::Render3DObjects()
{
	SceneObject *pObj;
	// iterate through the objects and render shadows
	std::map<std::string,SceneObject*>::iterator it=m_SceneObjects.begin();
	for( ; it != m_SceneObjects.end(); ++it ) {
		if ((pObj = dynamic_cast<SceneObject*>(it->second))!=NULL)
		{
			if (pObj->isVisible())
			{
				pObj->RenderShadow();
			}
		}
	}
	// iterate through the objects to find object needs rendering
	it=m_SceneObjects.begin();
	for( ; it != m_SceneObjects.end(); ++it ) {
		if ((pObj = dynamic_cast<SceneObject*>(it->second))!=NULL)
		{
			if (pObj->isVisible()) 
			pObj->Render();
		}
	}
}

void VisualizationManager::Render2DObjects()
{
	if ((OGLContext::getInstance()->IsLogoFTActive())||(MenuManager::getInstance()->IsMenuVisible()))
	{
		OGLContext::getInstance()->GLOrtho2DCorrection();

		//2D context for rendering
		MenuManager::getInstance()->Render();
		OGLContext::getInstance()->RenderLogo();
	}

}


