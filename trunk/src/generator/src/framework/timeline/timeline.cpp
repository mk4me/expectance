/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timeline.h"

using namespace ft;

/// \brief constructor
TimeLine::TimeLine()
{
    setWaitingState(false);
}

/**
 * \brief Executes this timeline at current frame 
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLine is assigned
 **/
void TimeLine::Execute(float elapsedSeconds, TimeLineContext* timeLineContext)
{
    if (isWaitingState())
    {
        if (!isEmpty())
        {
            Start(timeLineContext, 0, 0);
        }
    }
    else
    {
        // TODO:  must be passed m_currTime to deeper motions
        TimeLineMotion::Execute(elapsedSeconds, timeLineContext);
        //if already execute motion should be removed?
        if (m_first!= NULL && timeLineContext->remove_after_execution &&   getCurrentObject()!=m_first)
        {
            RemoveExecutedMotions(timeLineContext);
        }
    }
}

void TimeLine::RemoveExecutedMotions(TimeLineContext* timeLineContext)
{
    if (m_first != NULL)
    {
        TimeLineObject* obj = m_first;
        
        while (obj != getCurrentObject())
        {
            TimeLineObject* obj_to_delete = obj;
            obj = obj->getNextObject();
            RemoveSubObject(obj_to_delete);
        }
    }
}

void TimeLine::Start(TimeLineContext* timeLineContext, float fade_in, float fade_out)
{
   std::cout << " TimeLine::Start - waitingState from " << isWaitingState() << " to false " << std::endl;
   setWaitingState(false);
   TimeLineMotion::Start(timeLineContext, fade_in, fade_out);
}


///\brief Stops execution of this TimeLineMotion
void TimeLine::Stop(TimeLineContext* timeLineContext)
{
   std::cout << " TimeLine::Stop - waitingState from " << isWaitingState() << " to true " << std::endl;
   setWaitingState(true);
}

///\brief Terminates TimeLine
void TimeLine::StopTimeLine(TimeLineContext* timeLineContext)
{
    std::cout << " TimeLine::StopTimeLine - submotions will be terminated " << std::endl;
    SetTerminated(true);
}
