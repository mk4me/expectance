/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinemotion.h"

using namespace ft;

void TimeLineMotion::Destroy(void)
{
    TimeLineObject::Destroy();
    
    //clear tracks
    for (int n=0; n<(int)m_vTracks.size(); n++)
    {
        m_vTracks[n]->Destroy();
        delete m_vTracks[n];
    }
    m_vTracks.clear();

    //clear modifiers
    for (int m=0; m<(int)m_vModifiers.size(); m++)
    {
        m_vModifiers[m]->Destroy();
        delete m_vModifiers[m];
    }
    m_vModifiers.clear();

}

TimeLineMotion::TimeLineMotion(void)
{
    m_animLoop = false;
    m_blender = NULL;
    m_motionRef = NULL; 
}

bool TimeLineMotion::AddModifier(TimeLineModifier* modifier)
{
    std::cout << " AddModifier " << modifier->toString() << " to " << toString() << std::endl;
    m_vModifiers.push_back(modifier);
	return true;
}
//        bool  TimeLineMotion::RemoveMdofier(TimeLineModifier* modfier);
//        TimeLineModifier* TimeLineMotion::GetModifier(int);

bool TimeLineMotion::AddTrack(TimeLineMotion* trackMotion)
{
    std::cout << " AddOTrack " << trackMotion->toString() << " to " << toString() << std::endl;
    m_vTracks.push_back(trackMotion);
	return true;
}
//        bool  TimeLineMotion::RemoveTrack(TimeLineMotion* trackMotion);
//        TimeLineMotion* TimeLineMotion::GetTrack(int);


void TimeLineMotion::Execute(float elapsedSeconds, Avatar* avatar)
{
    //handle all tracks
   if (m_vTracks.size() > 0)
   {
       //execute every track
       for (int t=0; t<(int)m_vTracks.size(); t++)
       {
           m_vTracks[t]->Execute(elapsedSeconds, avatar);
       }
   }

    // execute submotion
    TimeLineMotion* subMotion = (TimeLineMotion*)GetCurrentObject();
    if (subMotion)
    {
        subMotion->Execute(elapsedSeconds, avatar);
    }

    ExecuteAnim(elapsedSeconds, avatar);

    //Execute all modfiers
    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Apply(elapsedSeconds, avatar);
        }
    }
}

void TimeLineMotion::ExecuteAnim(float elapsedSeconds, Avatar* avatar)
{
    if (m_motionRef != NULL)
    {
        if (!isStarted())
        {
            avatar->GetCalModel()->getMixer()->blendCycle(m_motionRef->getAnimID(), 1.0f, 0.0f);
            setStarted(true);
        }
        // regard  m_blender, m_animLoop

        //- execute if start
		//- stop if time's off
		//- start/stop blending
    
    }
}


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