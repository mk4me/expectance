/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_MODIFIER_H
#define _GEN_TIMELINE_MODIFIER_H

#include "timelineobject.h"
#include "timelinecontext.h"

namespace ft
{
	/**
	 * Class TimeLineModifier: base class for modifiers that can change animiation at each frame
	 **/
    class TimeLineModifier : public TimeLineObject
    {
    public:
        TimeLineModifier() { /*m_parentMotion = NULL;*/ } 
        virtual ~TimeLineModifier(void) { /* empty */ }

        virtual void Apply(float elapsedSeconds, TimeLineContext* timeLineContext);

        virtual std::string toString(); //OVERRIDEN
    };
};


#endif //_GEN_TIMELINE_MODIFIER_H