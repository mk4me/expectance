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
    class TimeLine : public TimeLineMotion
    {
    public:
        TimeLine(void);
        virtual ~TimeLine(void) { /* empty */}

        void Destroy(void) { /* TODO: abak: to implement */ }

        void Reset();

        virtual void Execute(float elapsedSeconds, Avatar* avatar);


    private:
        float m_currTime;  //current time while timeline is being executed
    };
};


#endif //_GEN_TIMELINE_H