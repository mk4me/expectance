/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_MOTION_H
#define _GEN_TIMELINE_MOTION_H

#include "timelinemodifier.h"
#include "timelineblender.h"
#include "../motion/motion.h"
#include <vector>

namespace ft
{
	/**
	 * Class TimeLineMotion: object that can be put on TimeLine that represents motion (or set of motions) to execute 
     * by avatar
     **/
    class TimeLineMotion : public TimeLineObject
    {
    public:
        TimeLineMotion(void);
        virtual ~TimeLineMotion(void) { /* empty */ }
        virtual void Destroy(void);

        void setMotion(Motion *motion) { m_motionRef = motion; }
        Motion * getMotion() { return m_motionRef; } 

        void setBlender(TimeLineBlender* blender) { m_blender = blender; } 
        TimeLineBlender* getBlender() { return m_blender; }

        void setAnimLoop(bool set) { m_animLoop = set; }
        bool isAnimLoop() { return m_animLoop; }

        void setLoopNumber(int number) { m_loopNumber = number; }
        int getLoopNumber() { return m_loopNumber; } 

        void setInterupting(bool set) { m_interrupting = set; }
        bool isInterupting() { return m_interrupting; }

        void Dump(int depth);   //OVERRIDEN
        virtual std::string toString(); //OVERRIDEN

        virtual TimeLineObject* CreateInstance();
        virtual TimeLineObject* Clone();

    protected:
        Motion* m_motionRef; //reference to motion which is represented by this object
        TimeLineBlender* m_blender;

        bool m_animLoop;  // if the motion is looped
        int m_loopNumber; // defines length of looped motion by number of cycles (if value is -1 it is infinite)
        bool m_interrupting;  // indicates if this motion should immediately interuppt the previous motion
    };
}

#endif //_GEN_TIMELINE_MOTION_H
