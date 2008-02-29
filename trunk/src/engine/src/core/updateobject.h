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
    class ENGINE_API UpdateObject : public virtual BaseObject
    {
    public:
        UpdateObject(void) { /*empty*/}
	    virtual ~UpdateObject(void) { /*empty*/}

        virtual void OnUpdate(const double elapsedTime);  // updates by UpdateManager
    };
}

#endif //_GEN_CONTROL_OBJECT_H
