/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_UPDATE_MANAGER_H
#define _GEN_UPDATE_MANAGER_H

#include "message.h"
#include "UpdateObject.h" 
#include "simulation.h"
#include <map>

namespace ft
{
	/**
	 * Class UpdateManager: manages messages flow and updating objects 
	 **/
    class UpdateManager
    {
    public:
        UpdateManager(void) { /*empty*/}
	    virtual ~UpdateManager(void) { /*empty*/}
        
        static UpdateManager* getInstance();
        static void DestroyInstance();

        void Init();

        void OnUpdate();  // called from window application

        bool AddUpdateObject(UpdateObject* pObj);
        bool  RemoveUpdateObject(UpdateObject* pObj);

        void setTimeScale(float timeScale) { m_timeScale = timeScale; }
        float getTimeScale() { return m_timeScale; }

        void UpdateObjects(const double elapsedSeconds);
        void increraseFramesCounter() {   m_fpsFrames++;  }

		void Dump();

    private:
        
        static UpdateManager* m_instance;

        
		unsigned long long m_lastTick;

        double m_fpsDuration;
        int m_fpsFrames;
        int m_fps;
        float m_timeScale;

        std::map<std::string,UpdateObject*> m_objects;

        // focused object 

        //Input handler

        
    };

 
}

#endif //_GEN_UPDATE_MANAGER_H