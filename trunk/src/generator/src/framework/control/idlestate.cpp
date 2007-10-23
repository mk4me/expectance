/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "idlestate.h"

using namespace ft;
using namespace std;

/// \brief Overriden method from ft::ControlState
void IdleState::Init(MovableAvatar* avatar)
{
    ControlState::Init(avatar);

    m_tlIdle = new TimeLineMotion();

    Motion* mot = avatar->GetMotion(MOTION_WALK_IDLE);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
		timeLineMotion->setAnimLoop(true);
        m_tlIdle->AddSubObject(timeLineMotion);
        m_tlIdle->setBlender(new TimeLineBlender(0.05f));
    }
}

/// \brief Overriden method from ft::ControlState
void IdleState::Entry(MovableAvatar* avatar, ControlState* oldState)
{
    ControlState::Entry(avatar, oldState);
    avatar->getTimeLine()->AddSubObject(m_tlIdle);
}

/// \brief Overriden method from ft::ControlState
void IdleState::Exit(MovableAvatar* avatar, ControlState* newState)
{
    ControlState::Exit(avatar, newState);
}

/// \brief Overriden method from ft::ControlState
void IdleState::Reset(MovableAvatar* avatar)
{
    ControlState::Reset(avatar);
}

/// \brief Overriden method from ft::ControlState
std::string IdleState::toString()
{
    return "<IdleState>";
}
