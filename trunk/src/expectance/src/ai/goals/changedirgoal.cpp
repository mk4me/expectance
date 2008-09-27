/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "changedirgoal.h"
#include "../../utility/randomgen.h"
#include "../../utility/mathutil.h"
#include "../../avatar/osgavatar.h"
#include "evolution/action.h"
#include "osg/Vec3d"
#include "osg/Quat"

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

	bool isInMotion = false;
	//check if avatar moves on (not stands)
	if (timeLineContext->getAvatar()->getCurrTopMotion() != NULL)
	{
		if (timeLineContext->getAvatar()->getCurrTopMotion()->getName().compare("idle") == 0)
		{
			timeLineContext->getAvatar()->ExecuteAction("walk");
			//cout << " ChagerDir::Apply execute walk " << endl;
			isInMotion = true;
		}
		else
			isInMotion = true;
	}

	if (isInMotion)
	{
		if (m_fAngle > 0)
		{
			float dirToAdd = (m_fAngle>m_changeSpeed) ? m_changeSpeed : m_fAngle;
			m_fAngle -= dirToAdd;

			if (m_changeToLeft)
			{
				dirToAdd = -dirToAdd;
			}

			ft::OsgAvatar* avImpl = dynamic_cast<ft::OsgAvatar*>(timeLineContext->getAvatarImpl());
			osg::Quat currRot = avImpl->getGlobalRotation();
			currRot *= osg::Quat(degToRad(dirToAdd),osg::Vec3d(0,0,1));
			avImpl->setGlobalRotation(currRot);
			//cout << " ChangeDirController " << 

	//		osg::Quat addRot = osg::Quat(degToRad(dirToAdd), osg::Vec3d(0,1,0));
	//		GIL_Avatar* av = timeLineContext->getAvatarImpl();

	//        avImpl->changeGlobalRotationOffset( QuatToCalQuat(addRot) );

		}
		else
		{
			setGoalReached(true);
		}
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
	//cout << " ChagerDir::OnEnter " << endl;
/*
	cout << " ChangeDirGoal::OnEnter " << endl;
	if (av->getCurrTopMotion() != NULL)
	{
		cout << " ChangeDirGoal::OnEnter - motion SET " << endl;
		if (av->getCurrTopMotion()->getName().compare("idle") == 0)
			av->ExecuteAction("walk");
	}
	else
	{
		cout << " ChangeDirGoal::OnEnter - motion NULL " << endl;
	}
	*/

}

void ChangeDirGoal::OnExit(Avatar* av)
{
	//cout << " ChagerDir::OnExit " << endl;
//	av->ExecuteAction("idle");
}

int ChangeDirGoal::getActionToPerform()
{
    return Action::ACTION_WALK_ID;
}

GoalController* ChangeDirGoal::CreateController(Avatar *av)
{
    return new ChangeDirController(45, 180);
}

