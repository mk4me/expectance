/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinemodifier.h"

using namespace ft;

/**
 * Applies this modifier at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
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