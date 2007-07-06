/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "motion.h"

using namespace ft;

Motion::Motion()
{
    setAnimName("");
    setAnimID(-1); 
}
Motion::Motion(const std::string animName, int anim_id)
{
    setAnimName(animName);
    setAnimID(anim_id); 
} 