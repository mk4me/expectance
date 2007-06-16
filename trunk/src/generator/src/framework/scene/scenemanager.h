/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_SCENE_MANAGER_H
#define _GEN_SCENE_MANAGER_H

namespace ft
{
    class SceneManager
    {
    public:
        SceneManager (void) { /*empty*/ }
	    virtual ~SceneManager (void) { /*empty*/ }

        static SceneManager * getInstance();
        static void DestroyInstance();

    private:
        
        static SceneManager* m_instance;

        
        
    };
};

#endif //_GEN_SCENE_MANAGER_H