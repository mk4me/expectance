/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelineobject.h"

using namespace ft;

/// \brief constructor
TimeLineObject::TimeLineObject()
{
    m_startTime = TIME_UNDEFINED;
    m_endTime = TIME_UNDEFINED;

    m_started = false;
}

/**
 * \brief Releases all resources and objects related to this TimeLineObject
 *
 **/
void TimeLineObject::Destroy(void)
{
    std::cout << toString() << " Destroy() " << std::endl;
    
    for (int n=0; n<(int)m_vObjects.size(); n++)
    {
        m_vObjects[n]->Destroy();
        delete m_vObjects[n];
    }
    m_vObjects.clear();
}

/**
 * \brief Adds sub-objects to this TimeLineObject
 *
 * \param ft::TimeLineObject * object - sub-object to add
 * \return bool - true if sub-object added
 **/
bool TimeLineObject::AddObject(TimeLineObject* object)
{
    std::cout << " AddObject " << object->toString() << " to " << toString() << std::endl;
    m_vObjects.push_back(object);
	return true;
}
/*bool  TimeLineObject::RemoveMotion(Motion* motion)
{
    std::string _id = motion->getAnimName();

    if (!_id.empty())
	{
	 	std::map<std::string,Motion*>::iterator it = m_motions.find(_id);
		if ( it!=m_motions.end()) { 
            m_motions.erase(it);
            cout << "MovableAvatar::AddMotion motion " << _id << " removed form avatar " << std::endl;
			return true;
		}
	    m_motions.insert( std::make_pair( std::string(_id), motion) );
	}
    cout << "ERR: MovableAvatar::AddMotion motion " << _id << " not found in Avatar " << std::endl;
	return false;
}

Motion* MovableAvatar::GetMotion(std::string motionName)
{
    Motion * motion = NULL;
 	std::map<std::string,Motion*>::iterator it = m_motions.find(motionName);
	if ( it!=m_motions.end()) { 
        motion = dynamic_cast<Motion*>(it->second);
    }
    return motion;
}
*/

/**
 * \brief Returns sub-object that is currently executed
 *
 * \return ft::TimeLineObject * - sub-object
 **/
TimeLineObject* TimeLineObject::GetCurrentObject()
{
    TimeLineObject* result = NULL;
    if (m_vObjects.size() > 0)
        result = m_vObjects[0];
    return result;
}

/// \brief Resets all sub-objects 
void TimeLineObject::Reset(TimeLineContext* timeLineContext)
{
    for (int n=0; n<(int)m_vObjects.size(); n++)
    {
        m_vObjects[n]->Reset(timeLineContext);
    }

    //here is place to reset this object
}

/**
 * \brief Returns indent for dumping debug information on output console
 *
 * \param int depth - indent length
 * \return std::string - string that should be added before debug information for each line describing this TimeLineObject
 **/
std::string TimeLineObject::getDepthStr(int depth)
{
    std::string sep = "";
    for (int i=0; i<depth; i++)
        sep += "- ";
    return sep;
}

/**
 * \brief Prints debug information describing this TimeLineObject on output console
 *
 * \param int depth - indent on output console
 **/
void TimeLineObject::Dump(int depth)
{

    std::cout << getDepthStr(depth) << toString() << std::endl;

    for (int n=0; n<(int)m_vObjects.size(); n++)
    {
        m_vObjects[n]->Dump(depth+1);
    }
}

/**
 * \brief Prints debug info for this object
 *
 * \param const std::string & text - text to display
 **/
void TimeLineObject::PrintDebug(const std::string& text)
{
    std::cout << toString()<< ":" << text << std::endl;
}
