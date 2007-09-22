/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_H
#define _GEN_TIMELINE_H


#include "timelinemotion.h"

#include "../motion/motion.h"

namespace ft
{
	/**
	 * Class TimeLine: represents task to execute by avatar in sense of motion or sequence of motions. 
     * It is specific time of TimeLineMotion and can be trated in the same way as TimeLineMotion.
	 **/
    class TimeLine : public TimeLineMotion
    {
    public:
        TimeLine(void) { /* empty */}
        virtual ~TimeLine(void) { /* empty */}

        void Destroy(void) { /* TODO: abak: to implement */ }
    };
};


#endif //_GEN_TIMELINE_H