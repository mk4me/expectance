/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_SCENEMANAGER_H
#define _GEN_SCENEMANAGER_H

#include <map>
#include "../utility/debug.h"
#include "sceneobject.h"
#include "../core/UpdateManager.h"
#include "../core/config.h"
#include "visualizationmanager.h"



namespace ft
{
	
	//! A SceneManager class
	/*!
	 *	This class is responsible for registering, releasing and communication with the rest parts of the system in the scope of sceneobjects. 
	 *  It creates visual objects collection. Registers all requiring objects to scene.  
	 */ 
	class SceneManager
    {
    public:
        SceneManager (void) { /*empty*/ }
	    virtual ~SceneManager (void) { /*empty*/ }
        //! singleton - Returns the only instance of SceneManager
		/*! Creates the only instance of SceneManager */
        static SceneManager * getInstance();
        //! it cerates only instance when called first time
        static SceneManager* createInstanceFirstTime();
        //! destroy all resources owned by SceneManager
        static void DestroyInstance();

		//! register SceneObject
		/*! Registers object to be managed by SceneManager */
		virtual bool AddObject(SceneObject* pObj);   
		
		//! get the pointer value of SceneObject by unique id
		SceneObject* getObject(std::string id);
		//! get the pointer value of SceneObject by name
		SceneObject* getObjectByName(std::string name);
		//! remove SceneObject
		/*! Unregisters SceneObject from SceneManager queue and call their destructor */
		bool RemoveObject(SceneObject*);
		//! unregister SceneObject and destroy it
		bool RemoveObject(std::string id);
		
		std::map<std::string,SceneObject*>& getSceneObjectsMap(void){ return m_SceneObjectsMap;}; //tmp

    protected:
		
        static SceneManager* m_instance;
		std::map<std::string,SceneObject*> m_SceneObjectsMap;
		std::list<SceneObject*> m_SceneGraph; // for future use :)
		int m_ActiveCameraMarker;
    };
}

#endif //_GEN_SCENEMANAGER_H