/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_H
#define _GEN_TIMELINE_H

#include "timelineobject.h"
#include "timelinemotion.h"
#include "timelinemodifier.h"
#include "../motion/motion.h"

namespace ft
{
    class TimeLine : public TimeLineObject
    {
    public:
        TimeLine(void)  { CLASS_NAME = "TimeLine"; m_modifier = NULL;}
        virtual ~TimeLine(void) { /* empty */}

        void Destroy(void) { /* TODO: abak: to implement */ }


        /* Adds motion to timeline
         * @param index of motion on timeline
         * @return object that represents this motion on timeline
         */
        TimeLineMotion* AddMotion(Motion* motion, int ind);
        /* Removes motion of given index from timeline
         * @return if operation succeed
         */
        bool RemoveMotion(int ind);


    private:
           float length;  //length of timeline (defined by time)
           TimeLineModifier *m_modifier;


    };
};


#endif //_GEN_TIMELINE_H