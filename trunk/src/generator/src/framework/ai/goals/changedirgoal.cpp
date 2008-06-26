/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "changedirgoal.h"
#include "utility/randomgen.h"
#include "utility/mathconversions.h"
#include "../../avatar/calavatar.h"
#include "evolution/action.h"

using namespace ft;
using namespace ft::gil;
using namespace std;

/// \brief constructor
ChangeDirController::ChangeDirController(float minAngle, float maxAngle)
{
    int changingDir = RandomGenerator::RanIntValue(2);
    m_changeToLeft = (changingDir==0) ? true : false;
    m_changeSpeed = 1;

    int scope = (int)(maxAngle - minAngle);
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
    GoalController::Apply(elapsedSeconds, timeLineContext);

    if (m_fAngle > 0)
    {
        float dirToAdd = (m_fAngle>m_changeSpeed) ? m_changeSpeed : m_fAngle;
        m_fAngle -= dirToAdd;

        if (m_changeToLeft)
        {
            dirToAdd = -dirToAdd;
        }

        Quat addRot = Quat(degToRad(dirToAdd), Vec(0,1,0));
		GIL_Avatar* av = timeLineContext->getAvatar();

        ((CalAvatar*)av)->changeGlobalRotationOffset( QuatToCalQuat(addRot) );

    }
    else
    {
        setGoalReached(true);
    }
}


/// \brief Resets parameters of this modifier
void ChangeDirController::Reset(TimeLineContext * timeLineContext)
{
    GoalController::Reset(timeLineContext);
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

void ChangeDirGoal::OnEnter(Avatar* av)
{
	av->ExecuteAction("walk");
}

void ChangeDirGoal::OnExit(Avatar* av)
{
	av->ExecuteAction("idle");
}

int ChangeDirGoal::getActionToPerform()
{
    return Action::ACTION_WALK_ID;
}

GoalController* ChangeDirGoal::CreateController(Avatar *av)
{
    return new ChangeDirController(45, 180);
}

