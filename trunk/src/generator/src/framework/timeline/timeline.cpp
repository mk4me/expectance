/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timeline.h"

using namespace ft;

/// \brief constructor
TimeLine::TimeLine()
{
    m_currTime = TIME_UNDEFINED;
}


/**
 * \brief Executes this timeline at current frame 
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLine is assigned
 **/
void TimeLine::Execute(float elapsedSeconds, TimeLineContext* timeLineContext)
{
    m_currTime += elapsedSeconds;

    // TODO:  must be passed m_currTime to deeper motions
    TimeLineMotion::Execute(elapsedSeconds, timeLineContext);
}

///\brief Stops execution of this TimeLineMotion
void TimeLine::StopTimeLine(TimeLineContext* timeLineContext)
{
    std::cout << " TimeLine::StopTimeLine - submotions will be terminated " << std::endl;
    SetTerminated(true);
}
