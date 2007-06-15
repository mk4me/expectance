/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_MANAGER_H
#define _GEN_CONTROL_MANAGER_H

#include "message.h"
#include "controlobject.h" 

namespace ft
{
    class ControlManager
    {
    public:
        ControlManager(void) { /*empty*/}
	    virtual ~ControlManager(void) { /*empty*/}
        
        static ControlManager* getInstance();
        static void DestroyInstance();

        void OnUpdate(float elapsedTime);  // caled from window application

        void SendMessage(const Message& msg);  //request to send a message to registered objects

        void AddControlObject(ControlObject* object);
        void RemoveControlObject(ControlObject* object);

    private:
        
        static ControlManager* m_instance;
        //ControlObject vector

        // focused object 

        //Input handler

        
    };

 
};

#endif //_GEN_CONTROL_MANAGER_H