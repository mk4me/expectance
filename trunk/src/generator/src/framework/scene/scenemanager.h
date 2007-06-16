/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_SCENE_MANAGER_H
#define _GEN_SCENE_MANAGER_H

#include "../utility/debug.h"
#include "sceneobject.h"
#include <map>

namespace ft
{
    class SceneManager
    {
    public:
        SceneManager (void) { /*empty*/ }
	    virtual ~SceneManager (void) { /*empty*/ }

        static SceneManager * getInstance();
        static void DestroyInstance();

		bool AddObject(SceneObject* pObj);   
		SceneObject* getObject(std::string id);
		bool RemoveObject(SceneObject*);
		bool RemoveObject(std::string id);
		void Render();

    private:
		
        static SceneManager* m_instance;
		std::map<std::string,SceneObject*> g_SceneObjects;        

                
    };
};

#endif //_GEN_SCENE_MANAGER_H