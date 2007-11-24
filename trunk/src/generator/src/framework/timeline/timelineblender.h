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
	 * Class TimeLineBlender: represents blending between two motions (animations) represented by TimeLineMotion-s
	 **/
    class TimeLineBlender : public TimeLineObject
    {
    public:
        TimeLineBlender(float overlap) { m_overlap = overlap; } 
        virtual ~TimeLineBlender(void) { /* empty */ }

        void setOverlap(bool set) { m_overlap = set; }
        float getOverlap() { return m_overlap; }


        virtual TimeLineObject* CreateInstance();
        virtual TimeLineObject* Clone();

    private:
        float m_overlap;
    };
};


#endif //_GEN_TIMELINE_BLENDER_H