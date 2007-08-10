/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_SCENE_MANAGER_H
#define _GEN_SCENE_MANAGER_H

#include <map>
#include "../utility/debug.h"
#include "line.h"
#include "traceline.h"
#include "sceneobject.h"
#include "camera.h"
#include "oglcontext.h"
#include "texturemanager.h"
#include "../ui/menumanager.h"
#include "../core/application.h"
#include "../core/controlmanager.h"



namespace ft
{
	//! A SceneManager class
	/*!
	 *	This class is responsible for registering, rendering and communication with the rest parts of the system in the scope of rendering. 
	 *  It creates scene. Registers all requiring object to rendering pipeline from messages from InputManager and sends by ControlManager 
	 *  messages to interested listeners.  
	 */ 
	class SceneManager
    {
    public:
        SceneManager (void) { /*empty*/ }
	    virtual ~SceneManager (void) { /*empty*/ }
        //! singleton - Returns the only instance of SceneManager
		/*! Creates the only instance of SceneManager as well as enforces instantiation of TextureManager and Camera singletons */
        static SceneManager * getInstance();
		//! destroy all resources owned by SceneManager
        static void DestroyInstance();

		//! create scene and all connected scene objects
		/*! Initializes all objects connected with SceneManager */
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
		bool AddObject(SceneObject* pObj);   
		
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
		void RenderObjects();
		//! throw out graphics menu from rendering pipeline
		void hideMenu();
    private:
		
        static SceneManager* m_instance;
		std::map<std::string,SceneObject*> g_SceneObjects;
		bool m_menuOGL;
    };
};

#endif //_GEN_SCENE_MANAGER_H