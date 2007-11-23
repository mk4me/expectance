/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timeline.h"

using namespace ft;

/// \brief Releases all resources and objects related to this TimeLineMotion
void TimeLine::Destroy(void)
{
    TimeLineMotion::Destroy();
    
    //clear modifiers
    for (int m=0; m<(int)m_vModifiers.size(); m++)
    {
        m_vModifiers[m]->Destroy();
        delete m_vModifiers[m];
    }
    m_vModifiers.clear();

}


/**
 * \brief OVERRIDEN from ft::TimeLineObject
 **/
bool TimeLine::AddSubObject(TimeLineObject* object, int where_to_add)
{
    if (Debug::TIMELINE>0)
        _dbg << "TimeLine::AddSubObject " << object->toString() << endl;

    return TimeLineObject::AddSubObject(object, where_to_add);
}

/**
 * \brief OVERRIDEN from ft::TimeLineObject
 **/
void TimeLine::RemoveSubObject(TimeLineObject* obj_to_delete, bool deleteFromMemory)
{
    if (Debug::TIMELINE>0)
        _dbg << "TimeLine::RemoveSubObject " << obj_to_delete->toString() << endl;

    TimeLineObject::RemoveSubObject(obj_to_delete, deleteFromMemory);
}

/**
 * \brief Adds modifier that can change animation while this TimeLine is executed
 *
 * \param ft::TimeLineModifier * modifier - modifier to add
 * \return bool - true if modifier added successfuly
 **/
bool TimeLine::AddModifier(TimeLineModifier* modifier)
{
    if (Debug::TIMELINE>0)
        _dbg << " AddModifier " << modifier->toString() << " to " << toString() << std::endl;

    m_vModifiers.push_back(modifier);
//    modifier->setParentMotion(this);
	return true;
}

bool  TimeLine::RemoveModifier(TimeLineModifier* modifier)
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
 * \brief Resets all objects related to this TimeLine
 *
 **/
void TimeLine::Reset(TimeLineContext* timeLineContext)
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
void TimeLine::Dump()
{
    _dbg << " TimeLine::Dump() for " << toString() << endl;
    _dbg << " modifiers list: " << m_vModifiers.size();
    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Dump(0);
        }
    }

    TimeLineMotion::Dump(2);

}   
