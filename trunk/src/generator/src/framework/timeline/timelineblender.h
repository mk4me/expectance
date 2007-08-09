/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_BLENDER_H
#define _GEN_TIMELINE_BLENDER_H

#include "timelineobject.h"

namespace ft
{
	/**
	 * Class TimeLineBlender: <describe the behavior and responsibility of this class>
	 **/
    class TimeLineBlender : public TimeLineObject
    {
    public:
        TimeLineBlender(float overlap) { m_overlap = overlap; } 
        virtual ~TimeLineBlender(void) { /* empty */ }

        void setOverlap(bool set) { m_overlap = set; }
        float getOverlap() { return m_overlap; }

    private:
        float m_overlap;
    };
};


#endif //_GEN_TIMELINE_BLENDER_H