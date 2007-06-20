/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_SCENE_MANAGER_H
#define _GEN_SCENE_MANAGER_H

#include <map>
#include "../utility/debug.h"
#include "sceneobject.h"
#include "camera.h"
#include "oglcontext.h"
#include "texturemanager.h"
#include "../core/application.h"
#include "../core/controlmanager.h"


namespace ft
{
    class SceneManager
    {
    public:
        SceneManager (void) { /*empty*/ }
	    virtual ~SceneManager (void) { /*empty*/ }

        static SceneManager * getInstance();
        static void DestroyInstance();

		bool Init();
		bool InitSceneObjects();
		void OnRender();
		void CleanUp();

		bool AddObject(SceneObject* pObj);   
		SceneObject* getObject(std::string id);
		bool RemoveObject(SceneObject*);
		bool RemoveObject(std::string id);
		void RenderObjects();

    private:
		
        static SceneManager* m_instance;
		std::map<std::string,SceneObject*> g_SceneObjects;             
    };
};

#endif //_GEN_SCENE_MANAGER_H