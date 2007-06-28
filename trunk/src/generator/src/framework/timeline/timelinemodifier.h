/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_MODIFIER_H
#define _GEN_TIMELINE_MODIFIER_H

#include "timelineobject.h"

namespace ft
{
    class TimeLineModifier : TimeLineObject
    {
    public:
        TimeLineModifier() { /* empty */ } 
        virtual ~TimeLineModifier(void) { /* empty */ }
    };
};


#endif //_GEN_TIMELINE_MODIFIER_H