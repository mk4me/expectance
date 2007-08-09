/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timeline.h"

using namespace ft;

/**
 * TimeLine: <describe the responsibilities and behavior of this method>
 *
 **/
TimeLine::TimeLine()
{
    m_currTime = TIME_UNDEFINED;
}


/**
 * Execute: <describe the responsibilities and behavior of this method>
 *
 * \param float elapsedSeconds <argument description>
 * \param ft::Avatar * avatar <argument description>
 **/
void TimeLine::Execute(float elapsedSeconds, Avatar* avatar)
{
    m_currTime += elapsedSeconds;

    // TODO:  must be passed m_currTime to deeper motions
    TimeLineMotion::Execute(elapsedSeconds, avatar);
}