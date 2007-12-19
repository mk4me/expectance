    /*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "speedcontroller.h"

using namespace ft;

/// \brief constructor
SpeedController::SpeedController()
{
}

/// \brief destructor
SpeedController::~SpeedController(void)
{
}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void SpeedController::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    TimeLineModifier::Apply(elapsedSeconds, timeLineContext);
}


/// \brief Resets parameters of this modifier
void SpeedController::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);
}

/**
 * \brief Returns string representation of this SpeedController
 *
 * \return std::string - string representation
 **/
std::string SpeedController::toString()
{
    std::string result = TimeLineModifier::toString() + "[SpeedController]";
    return result;
}
