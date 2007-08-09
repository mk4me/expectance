/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_MODIFIER_H
#define _GEN_TIMELINE_MODIFIER_H

#include "timelineobject.h"
#include "../avatar/avatar.h"

namespace ft
{
	/**
	 * Class TimeLineModifier: <describe the behavior and responsibility of this class>
	 **/
    class TimeLineModifier : public TimeLineObject
    {
    public:
        TimeLineModifier() { /* empty */ } 
        virtual ~TimeLineModifier(void) { /* empty */ }

        virtual void Apply(float elapsedSeconds, Avatar* avatar);
    };
};


#endif //_GEN_TIMELINE_MODIFIER_H