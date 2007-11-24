/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "idleaction.h"

using namespace ft;
using namespace std;

/// \brief Overriden method from ft::Action
void IdleAction::Init(PhysicsAvatar* avatar)
{
    Action::Init(avatar);

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

/// \brief Overriden method from ft::Action
void IdleAction::Entry(PhysicsAvatar* avatar, Action* oldAction)
{
    Action::Entry(avatar, oldAction);
    avatar->getTimeLine()->AddSubObject(m_tlIdle->Clone());
}

/// \brief Overriden method from ft::Action
void IdleAction::Exit(PhysicsAvatar* avatar, Action* newAction)
{
    Action::Exit(avatar, newAction);
}

/// \brief Overriden method from ft::Action
void IdleAction::Reset(PhysicsAvatar* avatar)
{
    Action::Reset(avatar);
}

/// \brief Overriden method from ft::Action
std::string IdleAction::toString()
{
    return "<IdleAction>";
}
