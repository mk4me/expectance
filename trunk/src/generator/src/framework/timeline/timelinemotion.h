/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_MOTION_H
#define _GEN_TIMELINE_MOTION_H

#include "timelineobject.h"
#include "timelineblender.h"
#include "../motion/motion.h"

namespace ft
{
    class TimeLineMotion : public TimeLineObject
    {
    public:
        TimeLineMotion(Motion *motion):m_blender(NULL) { m_motionRef = motion; CLASS_NAME = "TimeLineMotion";} 
        virtual ~TimeLineMotion(void) { /* empty */ }

        void setBlender(TimeLineBlender* blender) { m_blender = blender; } 
        TimeLineBlender* getBlender() { return m_blender; }

        void Dump(int depth);   //OVERRIDEN
        std::string toString(); //OVERRIDEN

    private:
        Motion* m_motionRef; //reference to motion which is represented by this object
        TimeLineBlender* m_blender;
        
    };
};

#endif //_GEN_TIMELINE_MOTION_H