/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "runaction.h"

using namespace ft;
using namespace std;

/// \brief Overriden method from ft::Action
void RunAction::Init(PhysicsAvatar* avatar)
{
    Action::Init(avatar);
    m_tlRun = new TimeLineMotion();

    Motion *mot = avatar->GetMotion(MOTION_RUN_START);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setInterupting(true);
        timeLineMotion->setBlender(new TimeLineBlender(0.25f));
        m_tlRun->AddSubObject(timeLineMotion, ADD_OBJECT_AS_LAST);
    }

     mot = avatar->GetMotion(MOTION_RUN_LOOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setAnimLoop(true);
        timeLineMotion->setMotion(mot);
        timeLineMotion->setBlender(new TimeLineBlender(0.2f));
        m_tlRun->AddSubObject(timeLineMotion);
    }

    

    mot = avatar->GetMotion(MOTION_RUN_STOP);
    if (mot != NULL)
    {
        TimeLineMotion* timeLineMotion = new TimeLineMotion();
        timeLineMotion->setMotion(mot);
        timeLineMotion->setBlender(new TimeLineBlender(0.2f));
        m_tlRun->AddSubObject(timeLineMotion);
        m_tlStopRun = timeLineMotion; 
    }

    m_tlRun->setBlender(new TimeLineBlender(0.20f));
}

/// \brief Overriden method from ft::Action
void RunAction::Entry(PhysicsAvatar* avatar, Action* oldAction)
{
    Action::Entry(avatar, oldAction);

    if (oldAction->getId() == ACTION_WALK_ID)
    {
        if (avatar->getTLExecutor() != NULL)
        {
            avatar->getTLExecutor()->RemoveUnexecutedMotions();
        }
    }

    m_tlStopRun->setInterupting(false);
    avatar->getTimeLine()->AddSubObject(m_tlRun);
}

/// \brief Overriden method from ft::Action
void RunAction::Exit(PhysicsAvatar* avatar, Action* newAction)
{
    Action::Exit(avatar, newAction);
    m_tlStopRun->setInterupting(true);
}

/// \brief Overriden method from ft::Action
void RunAction::Reset(PhysicsAvatar* avatar)
{
    Action::Reset(avatar);
}

/// \brief Overriden method from ft::Action
std::string RunAction::toString()
{
    return "<RunAction>";
}

