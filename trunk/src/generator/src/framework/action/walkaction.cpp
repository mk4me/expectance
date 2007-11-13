/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "walkaction.h"

using namespace ft;
using namespace std;

/// \brief Overriden method from ft::Action
void WalkAction::Init(PhysicsAvatar* avatar)
{
    Action::Init(avatar);
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

/// \brief Overriden method from ft::Action
void WalkAction::Entry(PhysicsAvatar* avatar, Action* oldAction)
{
    Action::Entry(avatar, oldAction);

    m_tlStopWalk->setInterupting(false);
    avatar->getTimeLine()->AddSubObject(m_tlWalk);

    if (oldAction->getId() == ACTION_RUN_ID)
    {
        m_tlWalk->AddSubObject(m_tlStopWalk);
    }
}

/// \brief Overriden method from ft::Action
void WalkAction::Exit(PhysicsAvatar* avatar, Action* newAction)
{
    Action::Exit(avatar, newAction);
    m_tlStopWalk->setInterupting(true);
}

/// \brief Overriden method from ft::Action
void WalkAction::Reset(PhysicsAvatar* avatar)
{
    Action::Reset(avatar);
}

/// \brief Overriden method from ft::Action
std::string WalkAction::toString()
{
    return "<WalkAction>";
}

