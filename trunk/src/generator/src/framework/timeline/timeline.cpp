/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timeline.h"

using namespace ft;

TimeLine::TimeLine()
{
    m_currTime = TIME_UNDEFINED;
}

void TimeLine::Reset()
{
    m_currTime = 0;
}
void TimeLine::Execute(float elapsedSeconds, Avatar* avatar)
{
    m_currTime += elapsedSeconds;

    // TODO:  must be passed m_currTime to deeper motions
    TimeLineMotion::Execute(elapsedSeconds, avatar);
}