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
 * \param ft::Avatar * avatar - avatar to which this TimeLine is assigned
 **/
void TimeLine::Execute(float elapsedSeconds, Avatar* avatar)
{
    m_currTime += elapsedSeconds;

    // TODO:  must be passed m_currTime to deeper motions
    TimeLineMotion::Execute(elapsedSeconds, avatar);
}