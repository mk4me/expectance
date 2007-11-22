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
    
    //clear modifiers
    for (int m=0; m<(int)m_vModifiers.size(); m++)
    {
        m_vModifiers[m]->Destroy();
        delete m_vModifiers[m];
    }
    m_vModifiers.clear();

}

/**
 * \brief Adds modifier that can change animation while this TimeLineMotion is executed
 *
 * \param ft::TimeLineModifier * modifier - modifier to add
 * \return bool - true if modifier added successfuly
 **/
bool TimeLineMotion::AddModifier(TimeLineModifier* modifier)
{
    if (Debug::TIMELINE>0)
        std::cout << " AddModifier " << modifier->toString() << " to " << toString() << std::endl;

    m_vModifiers.push_back(modifier);
    modifier->setParentMotion(this);
	return true;
}

bool  TimeLineMotion::RemoveModifier(TimeLineModifier* modifier)
{
    bool result = false;

    std::vector<TimeLineModifier*>::iterator iteratorModifier;
    iteratorModifier = m_vModifiers.begin();

    while(iteratorModifier != m_vModifiers.end())
    {
        // find the specified action and remove it
        if((*iteratorModifier) == modifier)
        {
            // found, so remove
            m_vModifiers.erase(iteratorModifier);
            result = true;
            break;
        }
        iteratorModifier++;
    }

    return result;
}

//        TimeLineModifier* TimeLineMotion::GetModifier(int);

/**
 * \brief Resets all objects related to this TimeLineMotion
 *
 **/
void TimeLineMotion::Reset(TimeLineContext* timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);

    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Reset(timeLineContext);
        }
    }
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
        std::cout << getDepthStr(depth+1) << "blender: " << std::endl;
        m_blender->Dump(depth+1);
    }
    else
        std::cout << getDepthStr(depth+1) << "blender: NULL " << std::endl;

    std::cout << getDepthStr(depth+1) << "modifiers list: " << m_vModifiers.size();
    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Dump(depth);
        }
    }
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