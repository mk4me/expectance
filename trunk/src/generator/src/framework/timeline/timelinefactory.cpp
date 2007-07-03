/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinefactory.h"
#include "translationmodifier.h"

using namespace ft;

TimeLineFactory* TimeLineFactory::m_instance = NULL;

TimeLineFactory* TimeLineFactory::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("TimeLineFactory::getInstace(): instance of TimeLineFactory created ");
        m_instance = new TimeLineFactory();
    }

    return m_instance;
}

void TimeLineFactory::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

TimeLine* TimeLineFactory::CreateTimeLine(Motion *motion)
{
    TimeLine* timeLine = new TimeLine();
//    TimeLineTrack* timeLineTrack = new TimeLineTrack();
    TimeLineMotion* timeLineMotion = new TimeLineMotion();
    timeLineMotion->setMotion(motion);
    timeLineMotion->setLoopNumber(2);
//    timeLineMotion->setAnimLoop(true);
//    timeLineMotion->AddModifier(new TranslationModifier());
//    timeLineTrack->AddObject(timeLineMotion);
    timeLine->AddObject(timeLineMotion);
    
    return timeLine;
}