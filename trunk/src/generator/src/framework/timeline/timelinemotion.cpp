/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinemotion.h"

using namespace ft;

/// \brief constructor
TimeLineMotion::TimeLineMotion(void)
{
    m_animLoop = false;
    m_blender = NULL;
    m_motionRef = NULL; 

    m_loopNumber = -1;

    setInterupting(false);
}

/// \brief Releases all resources and objects related to this TimeLineMotion
void TimeLineMotion::Destroy(void)
{
    TimeLineObject::Destroy();
}

/**
 * \brief Prints debug information describing this TimeLineMotion on output console
 *
 * \param int depth - indent on output console
 **/
void TimeLineMotion::Dump(int depth)
{
    TimeLineObject::Dump(depth);
    
    if (m_blender != NULL)
    {
        _dbg << getDepthStr(depth+1) << "blender: ";
        m_blender->Dump(depth+1);
    }
    else
        _dbg << getDepthStr(depth+1) << "blender: NULL " << std::endl;

    _dbg << endl;
}   

/**
 * \brief Returns string representation of this TimeLineMotion
 *
 * \return std::string - string representation
 **/
std::string TimeLineMotion::toString()
{
    std::string animName;

    if (m_motionRef != NULL)
        animName = m_motionRef->getAnimName();
    else
        animName = "NULL";

    std::string result = TimeLineObject::toString() + "[anim:" + animName + "]";
    return result;
}