/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _SCENE_TRANSFORMMANAGER_H
#define _SCENE_TRANSFORMMANAGER_H

#include "../utility/mathconversions.h"
#include "../core/platform.h"


namespace ft
{
    static const std::string TRANSFORM_FROM_3DSMAX = "transform_from_3dsmax";  

	/* A TransformManager class 
	 * Responsible for proper creation and initialization of Transform objects
	 */
    class ENGINE_API TransformManager
    {
    public:
        bool TRACK_TRANSFORM; 

        const static CalVector SCENE_FORWARD;
        const static CalVector SCENE_UP;
        const static CalVector SCENE_RIGHT;
        const static CalVector SCENE_LEFT;

        const static CalQuaternion Y_90;

		TransformManager();
	    virtual ~TransformManager(void);

        static TransformManager* getInstance();
        static void DestroyInstance();

    private:
        static TransformManager* m_instance;
        
    };
}

#endif //_SCENE_TRANSFORMMANAGER_H
