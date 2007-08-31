/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "walkstate.h"

using namespace ft;
using namespace std;

void WalkState::Init(MovableAvatar* avatar)
{
    ControlState::Init(avatar);
    m_tlWalk = new TimeLineMotion();

    Motion *mot = avatar->GetMotion(MOTION_WALK_START);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setLoopNumber(1);
        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setBlender(new TimeLineBlender(0.2f));
        m_tlWalk->AddSubObject(timeLineMotion, ADD_OBJECT_AS_LAST);
    }
 
 
    mot = avatar->GetMotion(MOTION_WALK_LOOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
//        timeLineMotion->setLoopNumber(1);
        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setMotion(mot);
//        timeLineMotion->setBlender(new TimeLineBlender(0.1f));
        m_tlWalk->AddSubObject(timeLineMotion);
    }

    

    mot = avatar->GetMotion(MOTION_WALK_STOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
//        timeLineMotion->setLoopNumber(1);
//        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setBlender(new TimeLineBlender(0.2f));
        timeLineMotion->setInterupting(true);
        m_tlWalk->AddSubObject(timeLineMotion);
        m_tlStopWalk = timeLineMotion; 
    }

    //m_tlWalk->setBlender(new TimeLineBlender(0.1f));
}

void WalkState::Entry(MovableAvatar* avatar)
{
    ControlState::Entry(avatar);

    m_tlStopWalk->setInterupting(false);
    m_tlWalk->Reset(avatar->getTimeLineContext());
    avatar->getTimeLine()->AddSubObject(m_tlWalk);
}

void WalkState::Exit(MovableAvatar* avatar)
{
    ControlState::Exit(avatar);
    m_tlStopWalk->setInterupting(true);
}

void WalkState::Reset(MovableAvatar* avatar)
{
    ControlState::Reset(avatar);
//    m_tlStopWalk->setInterupting(false);
}

std::string WalkState::toString()
{
    return "<WalkState>";
}

