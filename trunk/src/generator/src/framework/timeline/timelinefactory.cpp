/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinefactory.h"
#include "lcsmodifier.h"

using namespace ft;

TimeLineFactory* TimeLineFactory::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::TimeLineFactory (creates it at first call to this method)
 *
 * \return ft::TimeLineFactory * the only instance of TimeLineFactory
 **/
TimeLineFactory* TimeLineFactory::getInstance()
{
    if (m_instance == NULL)
    {
        if (Debug::TIMELINE>0)
            cout << "TimeLineFactory::getInstace(): instance of TimeLineFactory created " << endl;

        m_instance = new TimeLineFactory();
    }

    return m_instance;
}

/**
 * \brief Releases all resources related to this factory
 *
 **/
void TimeLineFactory::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * \brief Creates a new TimeLine
 *
 * \param ft::Motion * motion - motion (animation) that will be put on TimeLine
 * \return ft::TimeLine * - created TimeLine
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
//    timeLine->AddObject(timeLineMotion);
    
    return timeLine;
}