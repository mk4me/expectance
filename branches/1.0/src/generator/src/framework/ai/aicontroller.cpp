/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "aicontroller.h"

using namespace ft;

/// \brief constructor
AIController::AIController()
{
    setGoalReached(false);
}

/// \brief destructor
AIController::~AIController(void)
{
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void AIController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    TimeLineModifier::Apply(elapsedSeconds, timeLineContext);
}


/// \brief Resets parameters of this modifier
void AIController::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);
    setGoalReached(false);
}

/**
 * \brief Returns string representation of this AIController
 *
 * \return std::string - string representation
 **/
std::string AIController::toString()
{
    std::string result = TimeLineModifier::toString() + "[AIController]";
    return result;
}
