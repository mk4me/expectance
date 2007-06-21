/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONTROL_OBJECT_H
#define _GEN_CONTROL_OBJECT_H

#include "message.h"
#include "../scene/sceneobject.h"

namespace ft
{
    class ControlObject : public SceneObject
    {
    public:
        ControlObject(void) { /*empty*/}
	    virtual ~ControlObject(void) { /*empty*/}

        virtual void OnMessage(Message* msg);  // receives a message from ControlManager

        virtual void OnUpdate(float elapsedTime);  // updates by ControlManager
    };
};

#endif //_GEN_CONTROL_OBJECT_H