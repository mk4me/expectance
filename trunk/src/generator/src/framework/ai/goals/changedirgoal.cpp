/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "changedirgoal.h"
#include "utility/randomgen.h"

using namespace ft;
using namespace std;

/// \brief constructor
ChangeDirController::ChangeDirController(float minAngle, float maxAngle)
{
    int changingDir = RandomGenerator::RanIntValue(2);
    m_changeToLeft = (changingDir==0) ? true : false;
    m_changeSpeed = 1;

    int scope = maxAngle - minAngle;
    int angle = RandomGenerator::RanIntValue(scope);

    m_fAngle = minAngle + angle;

    //_dbg << "  ------------ Random  angle -------- " <<  m_fAngle;
}

/// \brief destructor
ChangeDirController::~ChangeDirController(void)
{
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void ChangeDirController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    AIController::Apply(elapsedSeconds, timeLineContext);

    if (m_fAngle > 0)
    {
        float dirToAdd = (m_fAngle>m_changeSpeed) ? m_changeSpeed : m_fAngle;
        m_fAngle -= dirToAdd;

        if (m_changeToLeft)
        {
            dirToAdd = -dirToAdd;
        }

        Quat addRot = Quat(degToRad(dirToAdd), Vec(0,1,0));
        timeLineContext->getAvatar()->changeGlobalRotationOffset( QuatToCalQuat(addRot) );

    }
    else
    {
        setGoalReached(true);
    }
}


/// \brief Resets parameters of this modifier
void ChangeDirController::Reset(TimeLineContext * timeLineContext)
{
    AIController::Reset(timeLineContext);
}

/// \brief Constructor
ChangeDirGoal::ChangeDirGoal()
{
}

// \brief Destructor
ChangeDirGoal::~ChangeDirGoal()
{
    //empty
}

bool ChangeDirGoal::ExecuteAction(ActionAvatar *av)
{
    bool result = false;
    int actionId = -1;

    if (av->getCurrAction() != NULL && (av->getCurrAction()->getId() == ACTION_WALK_ID || av->getCurrAction()->getId() == ACTION_RUN_ID))
    {
        //do nothing - leve walk or run
        result = true;
    }
    else
    {
        //execute default action for this goal
        result = Goal::ExecuteAction(av);
    }

    return result;
}

int ChangeDirGoal::getActionToPerform()
{
    return ACTION_WALK_ID;
}

AIController* ChangeDirGoal::CreateController(ActionAvatar *av)
{
    return new ChangeDirController(45, 180);
}

