/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "walkstate.h"

using namespace ft;
using namespace std;

/// \brief Overriden method from ft::ControlState
void WalkState::Init(MovableAvatar* avatar)
{
    ControlState::Init(avatar);
    m_tlWalk = new TimeLineMotion();

    Motion *mot = avatar->GetMotion(MOTION_WALK_START);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setInterupting(true);
        timeLineMotion->setBlender(new TimeLineBlender(0.25f));
        m_tlWalk->AddSubObject(timeLineMotion, ADD_OBJECT_AS_LAST);
    }

     mot = avatar->GetMotion(MOTION_WALK_LOOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setMotion(mot);
        timeLineMotion->setBlender(new TimeLineBlender(0.2f));
        m_tlWalk->AddSubObject(timeLineMotion);
    }

    mot = avatar->GetMotion(MOTION_WALK_STOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setBlender(new TimeLineBlender(0.2f));
        m_tlWalk->AddSubObject(timeLineMotion);
        m_tlStopWalk = timeLineMotion; 
    }

    m_tlWalk->setBlender(new TimeLineBlender(0.20f));
}

/// \brief Overriden method from ft::ControlState
void WalkState::Entry(MovableAvatar* avatar, ControlState* oldState)
{
    ControlState::Entry(avatar, oldState);

    m_tlStopWalk->setInterupting(false);
    avatar->getTimeLine()->AddSubObject(m_tlWalk);

    if (oldState->getId() == STATE_RUN_ID)
    {
        m_tlWalk->AddSubObject(m_tlStopWalk);
    }
}

/// \brief Overriden method from ft::ControlState
void WalkState::Exit(MovableAvatar* avatar, ControlState* newState)
{
    ControlState::Exit(avatar, newState);
    m_tlStopWalk->setInterupting(true);
}

/// \brief Overriden method from ft::ControlState
void WalkState::Reset(MovableAvatar* avatar)
{
    ControlState::Reset(avatar);
}

/// \brief Overriden method from ft::ControlState
std::string WalkState::toString()
{
    return "<WalkState>";
}

