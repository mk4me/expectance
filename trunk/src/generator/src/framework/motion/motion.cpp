/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "motion.h"

using namespace ft;


/// \brief Constructor
Motion::Motion()
{
    setAnimName("");
    setAnimID(-1); 
}
/// \brief Constructor
/// \param const std::string animName - name of animation which is referenced by this object
/// \param int anim_id - id of animation which is referenced by this object
Motion::Motion(const std::string animName, int anim_id)
{
    setAnimName(animName);
    setAnimID(anim_id); 
} 