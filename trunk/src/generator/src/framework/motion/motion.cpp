/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "motion.h"

using namespace ft;

/**
 * Motion: <describe the responsibilities and behavior of this method>
 *
 **/
Motion::Motion()
{
    setAnimName("");
    setAnimID(-1); 
}
/**
 * Motion: <describe the responsibilities and behavior of this method>
 *
 * \param const std::string animName <argument description>
 * \param int anim_id <argument description>
 **/
Motion::Motion(const std::string animName, int anim_id)
{
    setAnimName(animName);
    setAnimID(anim_id); 
} 