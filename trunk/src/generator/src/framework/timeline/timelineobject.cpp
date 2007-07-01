/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelineobject.h"

using namespace ft;

TimeLineObject::TimeLineObject()
{
    m_startTime = TIME_UNDEFINED;
    m_endTime = TIME_UNDEFINED;

    m_started = false;
}

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

TimeLineObject* TimeLineObject::GetCurrentObject()
{
    TimeLineObject* result = NULL;
    if (m_vObjects.size() > 0)
        result = m_vObjects[0];
    return result;
}

void TimeLineObject::Reset()
{
    for (int n=0; n<(int)m_vObjects.size(); n++)
    {
        m_vObjects[n]->Reset();
    }

    //here is place to reset this object
}

std::string TimeLineObject::getDepthStr(int depth)
{
    std::string sep = "";
    for (int i=0; i<depth; i++)
        sep += "- ";
    return sep;
}

void TimeLineObject::Dump(int depth)
{

    std::cout << getDepthStr(depth) << toString() << std::endl;

    for (int n=0; n<(int)m_vObjects.size(); n++)
    {
        m_vObjects[n]->Dump(depth+1);
    }
}
