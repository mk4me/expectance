/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AVATAR_H
#define _GEN_AVATAR_H

#include "cal3d/cal3d.h"
#include "../core/controlobject.h"

namespace ft
{
    class Avatar : public ControlObject
    {
    public:
        Avatar(const std::string name) { m_name = name; }
	    virtual ~Avatar(void) { /*empty*/}

        void SetCalModel(CalModel* m_calModel);

        void OnMessage(Message& msg);  // OVERRIDEN, receives a message from ControlManager
        virtual void OnUpdate(float elapsedTime);  // OVERRIDEN, updates by ControlManager 

    private:
        std::string m_name;

        CalModel* m_calModel;  //cal3d model which is represented by this avatar object
       
    };
};

#endif //_GEN_AVATAR_H
