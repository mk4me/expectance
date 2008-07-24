/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "restposgoal.h"
#include "player.h"
#include "utility/randomgen.h"
#include "utility/mathutil.h"
#include "evolution/action.h"
#include <math.h>

using namespace ft;
using namespace std;

/// \brief constructor
RestPosController::RestPosController(Avatar *av)
{
	m_changeSpeed = 1;
}

/// \brief destructor
RestPosController::~RestPosController(void)
{
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void RestPosController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
	Player* avImpl = (Player*)timeLineContext->getAvatar();

	CalVector vDist = avImpl->getRestPos() - avImpl->getPosition();

	//std::cout << " Reached: " <<  isGoalReached()  << " dist " << vDist.length() << std::endl;
	if (vDist.length() < 300)
	{
		std::cout << " GOAL REACHED .............." <<  std::endl;
		setGoalReached(true);
	}
	else
	{
		vDist.normalize();

		CalVector avDir = avImpl->getDirection();
		avDir.normalize();
	    
		// calculate angle to dest dir
		float dot = avDir*vDist;
		dot = UTIL_GetRightArgForArcCos(dot);
		float angle = acos(dot);
		float destAngle = RadToDeg(angle);

		if (destAngle > 0)
		{
			float dirToAdd;
			//determine change direction
			float changeDir = UTIL_GetSignForDirChange(avDir, vDist);
			if (changeDir > 0)
				dirToAdd = 5;
			else
				dirToAdd = -5;

			Quat addRot = Quat(degToRad(dirToAdd), Vec(0,1,0));
			GIL_Avatar* av = timeLineContext->getAvatar();

			((CalAvatar*)av)->changeGlobalRotationOffset( QuatToCalQuat(addRot) );
		}
	}

}


/// \brief Resets parameters of this modifier
void RestPosController::Reset(TimeLineContext * timeLineContext)
{
    GoalController::Reset(timeLineContext);
}

/// \brief Constructor
RestPosGoal::RestPosGoal()
{
    setPriority(PRIORITY_HIGH);
}

// \brief Destructor
RestPosGoal::~RestPosGoal()
{
    //empty
}

void RestPosGoal::OnEnter(Avatar* av)
{
	std::cout << " RestPosGoal::OnEnter .............." <<  std::endl;
    {
		av->ExecuteAction("walk");
    }
}

void RestPosGoal::OnExit(Avatar* av)
{
		av->ExecuteAction("idle");
}

int RestPosGoal::getActionToPerform()
{
    return Action::ACTION_WALK_ID;
}

GoalController* RestPosGoal::CreateController(Avatar *av)
{
    return new RestPosController(av);
}

