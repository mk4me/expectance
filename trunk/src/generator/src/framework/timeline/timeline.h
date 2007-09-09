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
        TimeLine(void);
        virtual ~TimeLine(void) { /* empty */}

        void Destroy(void) { /* TODO: abak: to implement */ }

        virtual void Execute(float elapsedSeconds, TimeLineContext* timeLineContext);

        void StopTimeLine(TimeLineContext* timeLineContext);

        virtual void Start(TimeLineContext* timeLineContext, float fade_in, float fade_out);
        virtual void Stop(TimeLineContext* timeLineContext);

        void setWaitingState(bool set) { m_waitingState = set; }
        bool isWaitingState() { return m_waitingState; }

    protected:
        void RemoveExecutedMotions(TimeLineContext* timeLineContext);

    private:
        bool m_waitingState;
    };
};


#endif //_GEN_TIMELINE_H