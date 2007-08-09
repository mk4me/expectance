/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinefactory.h"
#include "lcsmodifier.h"

using namespace ft;

TimeLineFactory* TimeLineFactory::m_instance = NULL;

/**
 * getInstance: <describe the responsibilities and behavior of this method>
 *
 * \return ft::TimeLineFactory * <describe what is returned if appropriate>
 **/
TimeLineFactory* TimeLineFactory::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("TimeLineFactory::getInstace(): instance of TimeLineFactory created ");
        m_instance = new TimeLineFactory();
    }

    return m_instance;
}

/**
 * DestroyInstance: <describe the responsibilities and behavior of this method>
 *
 **/
void TimeLineFactory::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * CreateTimeLine: <describe the responsibilities and behavior of this method>
 *
 * \param ft::Motion * motion <argument description>
 * \return ft::TimeLine * <describe what is returned if appropriate>
 **/
TimeLine* TimeLineFactory::CreateTimeLine(Motion *motion)
{
    TimeLine* timeLine = new TimeLine();
//    TimeLineTrack* timeLineTrack = new TimeLineTrack();
    TimeLineMotion* timeLineMotion = new TimeLineMotion();
    timeLineMotion->setMotion(motion);
    timeLineMotion->setLoopNumber(2);
//    timeLineMotion->setAnimLoop(true);
//    timeLineMotion->AddModifier(new LCSModifier());
//    timeLineTrack->AddObject(timeLineMotion);
    timeLine->AddObject(timeLineMotion);
    
    return timeLine;
}