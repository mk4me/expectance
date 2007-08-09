/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinemodifier.h"

using namespace ft;

/**
 * Apply: <describe the responsibilities and behavior of this method>
 *
 * \param float elapsedSeconds <argument description>
 * \param ft::Avatar * avatar <argument description>
 **/
void TimeLineModifier::Apply(float elapsedSeconds,Avatar* avatar)
{
    // execute submodifier
    TimeLineModifier* subModifier = (TimeLineModifier*)GetCurrentObject();
    if (subModifier)
    {
        subModifier->Apply(elapsedSeconds, avatar);
    }

    // Here apply this modifier
}