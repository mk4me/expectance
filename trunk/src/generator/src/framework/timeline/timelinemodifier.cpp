/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinemodifier.h"

using namespace ft;

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