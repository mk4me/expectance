/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelineexecutor.h"

using namespace ft;

/// \brief constructor
TimeLineExecutor::TimeLineExecutor(void)
{
    Reset();
}

/// \brief Releases all resources and objects related to this TimeLineExecutor
void TimeLineExecutor::Destroy(void)
{
}

void TimeLineExecutor::Reset()
{
    setState(EXEC_STATE_NOT_INITED);
    m_prevMotion = NULL;
    m_currMotion = NULL;
    m_nextMotion = NULL;

    m_currAnimTime = 0.0f;
    m_currAnimDuration = 0.0f;

    m_overlapPrev = 0.0f;
    m_overlapCurr = 0.0f; 
}

void TimeLineExecutor::Initiate(TimeLine* timeLine, TimeLineContext* timeLineContext)
{
    setTimeLine(timeLine);
    setTimeLineContext(timeLineContext);
}

void TimeLineExecutor::Start()
{
    setTerminated(false);
}

void TimeLineExecutor::StopRequest()
{
    setTerminated(true);
}

void TimeLineExecutor::ChangeState(int newState)
{

}

void TimeLineExecutor::UpdateMotions(float elapsedSeconds)
{
    int test = 0;
}

void TimeLineExecutor::UpdateModifiers(float elapsedSeconds)
{
}

/**
 * \brief Returns string representation of states
 *
 * \param int state - identifier of message
 * \return std::string - string representation of message
 **/
std::string TimeLineExecutor::_GET_STATE_NAME(int state)
{
    std::string strState;

    switch(state)
    {
    case EXEC_STATE_NOT_INITED: strState= "EXEC_STATE_NOT_INITED"; break;
    case EXEC_STATE_WAIT: strState = "EXEC_STATE_WAIT"; break;
    case EXEC_STATE_STARTED: strState = "EXEC_STATE_STARTED"; break;
    default: strState = "<unknown>"; break;
    }
    
    return strState;
}


