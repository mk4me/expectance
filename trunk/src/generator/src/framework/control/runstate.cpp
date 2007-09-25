/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "runstate.h"

using namespace ft;
using namespace std;

void RunState::Init(MovableAvatar* avatar)
{
    ControlState::Init(avatar);
    m_tlRun = new TimeLineMotion();

    Motion *mot = avatar->GetMotion(MOTION_RUN_START);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setLoopNumber(1);
        timeLineMotion->setInterupting(true);
//        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setBlender(new TimeLineBlender(0.25f));
        m_tlRun->AddSubObject(timeLineMotion, ADD_OBJECT_AS_LAST);
    }

     mot = avatar->GetMotion(MOTION_RUN_LOOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
//        timeLineMotion->setLoopNumber(1);
        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setInterupting(true);
        timeLineMotion->setMotion(mot);
        timeLineMotion->setBlender(new TimeLineBlender(0.2f));
        m_tlRun->AddSubObject(timeLineMotion);
    }

    

    mot = avatar->GetMotion(MOTION_RUN_STOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
//        timeLineMotion->setLoopNumber(1);
        //timeLineMotion->setAnimLoop(true);
        timeLineMotion->setBlender(new TimeLineBlender(0.2f));
        //timeLineMotion->setInterupting(true);
        m_tlRun->AddSubObject(timeLineMotion);
        m_tlStopRun = timeLineMotion; 
    }

    m_tlRun->setBlender(new TimeLineBlender(0.20f));
}

void RunState::Entry(MovableAvatar* avatar, ControlState* oldState)
{
    ControlState::Entry(avatar, oldState);

    //Init(avatar);  //TODO: remove this line
    if (oldState->getId() == STATE_WALK_ID)
    {
        if (avatar->getTLExecutor() != NULL)
        {
            avatar->getTLExecutor()->RemoveUnexecutedMotions();
        }
    }

    m_tlStopRun->setInterupting(false);
    m_tlRun->Reset(avatar->getTimeLineContext());
    avatar->getTimeLine()->AddSubObject(m_tlRun);
}

void RunState::Exit(MovableAvatar* avatar, ControlState* newState)
{
    ControlState::Exit(avatar, newState);
    m_tlStopRun->setInterupting(true);
}

void RunState::Reset(MovableAvatar* avatar)
{
    ControlState::Reset(avatar);
}

std::string RunState::toString()
{
    return "<RunState>";
}

