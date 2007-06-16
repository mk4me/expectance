/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_MANAGER_H
#define _GEN_CONTROL_MANAGER_H

#include "message.h"
#include "controlobject.h" 
#include "simulation.h"
#include <map>

namespace ft
{
    class ControlManager
    {
    public:
        ControlManager(void) { /*empty*/}
	    virtual ~ControlManager(void) { /*empty*/}
        
        static ControlManager* getInstance();
        static void DestroyInstance();

        void Init();

        void OnUpdate();  // caled from window application

        void SendMessage(const Message& msg);  //request to send a message to registered objects

        bool AddControlObject(ControlObject* pObj);
        bool  RemoveControlObject(ControlObject* pObj);

        void setTimeScale(float timeScale) { m_timeScale = timeScale; }
        float getTimeScale() { return m_timeScale; }

        void UpdateObjects(float elapsedSeconds);
        void increraseFramesCounter() {   m_fpsFrames++;  }

        void Dump();

    private:

        unsigned int m_lastTick;

        float m_fpsDuration;
        int m_fpsFrames;
        int m_fps;
        float m_timeScale;

        
        static ControlManager* m_instance;

        std::map<std::string,ControlObject*> m_objects;

        // focused object 

        //Input handler

        
    };

 
};

#endif //_GEN_CONTROL_MANAGER_H