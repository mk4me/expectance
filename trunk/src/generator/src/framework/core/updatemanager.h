/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_MANAGER_H
#define _GEN_CONTROL_MANAGER_H

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

        void OnUpdate();  // caled from window application

        void SendMessage(Message* msg, bool deleteAfterSent);  //request to send a message to registered objects

        bool AddUpdateObject(UpdateObject* pObj);
        bool  RemoveUpdateObject(UpdateObject* pObj);

        void setTimeScale(float timeScale) { m_timeScale = timeScale; }
        float getTimeScale() { return m_timeScale; }

        void UpdateObjects(float elapsedSeconds);
        void increraseFramesCounter() {   m_fpsFrames++;  }

        void Dump();

    private:
        
        static UpdateManager* m_instance;

        unsigned int m_lastTick;

        float m_fpsDuration;
        int m_fpsFrames;
        int m_fps;
        float m_timeScale;

        std::map<std::string,UpdateObject*> m_objects;

        // focused object 

        //Input handler

        
    };

 
};

#endif //_GEN_CONTROL_MANAGER_H