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
	/**
	 * Class UpdateObject: base class for objects that can be registered in UpdateManager
	 **/
    class UpdateObject : public virtual BaseObject
    {
    public:
        UpdateObject(void) { /*empty*/}
	    virtual ~UpdateObject(void) { /*empty*/}

        virtual void OnMessage(Message* msg);  // receives a message from UpdateManager

        virtual void OnUpdate(float elapsedTime);  // updates by UpdateManager
    };
};

#endif //_GEN_CONTROL_OBJECT_H