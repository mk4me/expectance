/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINEEXECUTOR_H
#define _GEN_TIMELINEEXECUTOR_H

#include "timeline.h"

namespace ft
{
    //STATES of TimeLineExecutor
    static const int EXEC_STATE_NOT_INITED = 0; 
    static const int EXEC_STATE_WAIT = 1; 
    static const int EXEC_STATE_STARTED = 2;
    

	/**
	 * Class TimeLine: responsible for timeline execution 
	 **/
    class TimeLineExecutor
    {
    public:
        TimeLineExecutor(void);
        virtual ~TimeLineExecutor(void) { /* empty */}

        void Destroy(void);

        void setState(int state) { m_state = state; }
        int getState() { return m_state; }

        void setTimeLine(TimeLine* tl) { m_timeLine = tl; }
        TimeLine* getTimeLine() { return m_timeLine; }

        void setTimeLineContext(TimeLineContext* tlCtx) { m_timeLineContext = tlCtx; }
        TimeLineContext* getTimeLineContext() { return m_timeLineContext; }



        void Reset();

        void Initiate(TimeLine* timeLine, TimeLineContext* timeLineContext);

        void Start();
        void StopRequest();

        void ChangeState(int newState);

        void UpdateMotions(float elapsedSeconds);
        void UpdateModifiers(float elapsedSeconds);

        void setTerminated(bool set) { m_isTerminated = set; }
        bool isTerminated() { return m_isTerminated; }


        static std::string _GET_STATE_NAME(int state);

    private:
        int m_state;
        TimeLine* m_timeLine;
        TimeLineContext* m_timeLineContext;

        TimeLineMotion* m_prevMotion;  //only informative
        TimeLineMotion* m_currMotion;
        TimeLineMotion* m_nextMotion;

        float m_currAnimTime;
        float m_currAnimDuration;

        float m_overlapPrev;  //only informative
        float m_overlapCurr; 
        float m_overlapNext; 

        bool m_isTerminated;

    };
};

#endif //_GEN_TIMELINEEXECUTOR_H