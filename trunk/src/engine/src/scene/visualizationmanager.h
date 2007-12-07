/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_VISUALIZATION_MANAGER_H
#define _GEN_VISUALIZATION_MANAGER_H

#include <map>
#include "../utility/debug.h"
#include "line.h"
#include "traceline.h"
#include "sceneobject.h"
#include "cameramanager.h"
#include "oglcontext.h"
#include "texturemanager.h"
#include "../ui/menumanager.h"
#include "../core/application.h"
#include "../core/UpdateManager.h"
#include "../core/config.h"



namespace ft
{
	
	//! A VisualizationManager class
	/*!
	 *	This class is responsible for registering, rendering and communication with the rest parts of the system in the scope of rendering. 
	 *  It creates visual scene. Registers all requiring object to rendering pipeline from messages from InputManager and sends by UpdateManager 
	 *  messages to interested listeners.  
	 */ 
	class VisualizationManager
    {
    public:
        VisualizationManager (void) { /*empty*/ }
	    virtual ~VisualizationManager (void) { /*empty*/ }
        //! singleton - Returns the only instance of VisualizationManager
		/*! Creates the only instance of VisualizationManager as well as enforces instantiation of TextureManager and Camera singletons */
        static VisualizationManager * getInstance();
        //! it cerates only instance when called first time
        static VisualizationManager* createInstanceFirstTime();
        //! destroy all resources owned by VisualizationManager
        static void DestroyInstance();

		//! create scene and all connected scene objects
		/*! Initializes all objects connected with VisualizationManager */
		bool Init();
		/*! Creates display lists for system textures */
		bool InitSceneObjects();
		//! start main rendering pipe for all graphics objects of generator 
		/*! Renders all scene objects */
		void OnRender();
		//! clean all texture objects before shuting down of graphics system
		void CleanUp();
		//! register SceneObject
		/*! Registers object to be rendered */
		virtual bool AddObject(SceneObject* pObj);   
		
		//! get the pointer value of SceneObject by unique id
		SceneObject* getObject(std::string id);
		//! get the pointer value of SceneObject by name
		SceneObject* getObjectByName(std::string name);
		//! remove SceneObject
		/*! Unregisters SceneObject from Rendering queue and call their destructor */
		bool RemoveObject(SceneObject*);
		//! unregister SceneObject and destroy it
		bool RemoveObject(std::string id);
		//! main rendering queue for SceneObject type obejcts 
		/*! Calls all registered SceneObject type objects for rendering */
		void Render3DObjects();
		/*! Calls all 2D objects for rendering */
		void Render2DObjects();

        /*! Checks if object of given type can be traced by camera */
        virtual bool IsObjectTraceableByCamera(SceneObject* pObj);

    protected:
		
        static VisualizationManager* m_instance;
		std::map<std::string,SceneObject*> m_SceneObjects;
		int m_ActiveCameraMarker;
		RenderingOrder m_rendering;
    };
};

#endif //_GEN_VIZUALIZATION_MANAGER_H