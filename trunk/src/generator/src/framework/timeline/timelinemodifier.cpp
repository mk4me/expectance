/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinemodifier.h"

using namespace ft;

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLineModifier is assigned
 **/
void TimeLineModifier::Apply(float elapsedSeconds,TimeLineContext* timeLineContext)
{
    // execute submodifier
    //TimeLineModifier* subModifier = (TimeLineModifier*)getCurrentObject();
    //if (subModifier != NULL)
    //{
    //    subModifier->Apply(elapsedSeconds, timeLineContext);
    //}

    // Here apply this modifier
}

/**
 * \brief Returns string representation of this TimeLineModifier
 *
 * \return std::string - string representation
 **/
std::string TimeLineModifier::toString()
{
    std::string result = TimeLineObject::toString() + "[Modifier]";
    return result;
}