/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "limitedareagoal.h"
#include "../../utility/randomgen.h"
#include "../../utility/mathutil.h"
#include <math.h>

using namespace ft;
using namespace std;

/// \brief constructor
LimitedAreaController::LimitedAreaController(Avatar *av)
:ChangeDirController(150,180)
{
	OsgAvatar* avImpl = dynamic_cast<OsgAvatar*>(av->getImplementation());
	osg::Vec3d pos = avImpl->getPosition();
	CalVector distVect (pos.x(),pos.y(),pos.z());
    //reverse vector on the XY plane
    distVect.x = -distVect.x; distVect.z = -distVect.z;
    distVect.normalize();

	CalVector sceneDirection(0,1,0);
    

    float dot = sceneDirection*distVect;
    dot = UTIL_GetRightArgForArcCos(dot);

    float angle = acos(dot);
    m_fAngle = radToDeg(angle);

    float changeDir = UTIL_GetSignForDirChange(sceneDirection, distVect, CalVector(0,0,1));
    if (changeDir > 0)
    {
        m_changeToLeft = false;
    }
    else
    {
        m_changeToLeft = true;
    }

//    _dbg << " !!!---------!!! ----------  Awaryjny zwrot " << m_fAngle << endl;
    

    m_changeSpeed = 2;
}

/// \brief destructor
LimitedAreaController::~LimitedAreaController(void)
{
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void LimitedAreaController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    ChangeDirController::Apply(elapsedSeconds, timeLineContext);
}


/// \brief Resets parameters of this modifier
void LimitedAreaController::Reset(TimeLineContext * timeLineContext)
{
    GoalController::Reset(timeLineContext);
}

/// \brief Constructor
LimitedAreaGoal::LimitedAreaGoal()
{
    setPriority(PRIORITY_HIGH);
}

// \brief Destructor
LimitedAreaGoal::~LimitedAreaGoal()
{
    //empty
}

GoalController* LimitedAreaGoal::CreateController(Avatar *av)
{
    return new LimitedAreaController(av);
}

