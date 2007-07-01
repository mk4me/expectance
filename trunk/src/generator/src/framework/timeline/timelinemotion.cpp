/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelinemotion.h"

using namespace ft;

TimeLineMotion::TimeLineMotion(void)
{
    m_animLoop = false;
    m_blender = NULL;
    m_motionRef = NULL; 

    m_loopNumber = 0;

    setInterupting(false);

    ResetParams();

}

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

void TimeLineMotion::ResetParams()
{
    // initial paramters here
    m_currSubMotion = -1;  // no any motion
    setToFinish(false);
    m_currLoop = -1;  // not started yet
    setAnimStarted(false);
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


void TimeLineMotion::Start()
{
    setStarted(true);

    if (m_vObjects.size()>0)
    {
        //start first sub-motion if any
        m_currSubMotion = 0;
        TimeLineMotion* timeLineMotion = dynamic_cast<TimeLineMotion*>(m_vObjects[0]);
        timeLineMotion->Start();
    }

}
void TimeLineMotion::Stop()
{
    setStarted(false);
}

void TimeLineMotion::Execute(float elapsedSeconds, Avatar* avatar)
{
    ExecuteTracks(elapsedSeconds, avatar);
    bool anyStarted = ExecuteSubMotions(elapsedSeconds, avatar);
    ExecuteAnim(elapsedSeconds, avatar);
    ExecuteModifiers(elapsedSeconds, avatar);

    if (!anyStarted && !isAnimStarted())
    {
        Stop();
    }
}

void TimeLineMotion::ExecuteTracks(float elapsedSeconds, Avatar* avatar)
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
}

// Returns true if any submotion started, false if none submotion is started
//
bool TimeLineMotion::ExecuteSubMotions(float elapsedSeconds, Avatar* avatar)
{
//ad 2) wykonanie sekwencji pod-ruchow					
//- ruch musi zawierac wskaznik na akatualnie wykonywany pod-ruch 					
//- sprawdz czy wykonywany jest jakis pod-ruch
//	- jesli tak:
        //- sprawdz czy jest nastepny z flaga 'interupting'
	       // - jesli tak:
	       //     - ustaw aktualnie wykonywany podruch do zakonczenia
        //    - jesli nie nie to nie rob nic
//
//	- jesli nie:
//			- sprawdz czy jest jakis nastepny ruch do wykonania
//				- jesli tak to wykonaj podruch
//				- jesli nie to zaznacz ze nie ma wiecej ruchow do wykonania

    bool anyStarted = false;
 
    TimeLineMotion* currMotion = NULL;
    TimeLineMotion* nextMotion = NULL;

    //only if current indicator is valid (note that <m_currSubMotion==-1> means that there is no motions to execute)
    if (m_currSubMotion >= 0)
    {
        currMotion = (TimeLineMotion*)m_vObjects[m_currSubMotion];

        int size = m_vObjects.size();
        if (m_currSubMotion < size - 1)  //if there is next sub-motion
        {
            nextMotion = (TimeLineMotion*)m_vObjects[m_currSubMotion + 1];
        }
    }

    if (currMotion != NULL)
    {
        
        if (currMotion->isStarted())
        {
            if (nextMotion!= NULL && nextMotion->isInterupting())
            {
                currMotion->setToFinish(true);
            }
            currMotion->Execute(elapsedSeconds, avatar);
            anyStarted = true;
        }
        else  // there is no started motion
        {
            if (nextMotion !=NULL)
            {
                m_currSubMotion++;
                nextMotion->Start();
                nextMotion->Execute(elapsedSeconds, avatar);
                anyStarted = true;
            }
            else
            {
                m_currSubMotion = -1;  // no more submotions to execute
            }
        }
    }
    return anyStarted;
}


void TimeLineMotion::ExecuteAnim(float elapsedSeconds, Avatar* avatar)
{

 //   ad 1) Wykonywanie pojedynczego ruchu:
    //- jesli podlaczona jest animacja
	//  - jesi tak to sprawdz czy animacja wystartowana:
	    //	- jesli tak to:
	    //		- sprawdz czy ruch cykliczny
				    //- jesli tak to sprawdz czy biezaca petla sie zakonczyla
        //                    - jesli tak:  sprawdz czy to ostatnia petla lub ruch jest zaznaczony do zakonczenia
        //                           - jesli tak wykonaj clearCycle() oraz ustaw glowna animacje jako skonczona 
    //                               - jesli nie to zwieksz licznik biezacej petli
      
			     //   - jesli nie to sprawdz czy animacja sie zakonczyla
				    //	    - jesli tak to ustaw glowna animacje jako skonczona 

	    //	- jesli nie to sprawdz czy cykliczny
	    //		- jesli tak wystartuj animacje przez blendCycle() 
	    //		- jesli nie to wystartuj animacje przez executeAction()

    if (m_motionRef != NULL)
    {
        if (isAnimStarted())
        {
            CalCoreAnimation* anim = avatar->GetCalCoreModel()->getCoreAnimation(m_motionRef->getAnimID());

            if (this->isAnimLoop())
            {
                // check if current loop is finished
                if (avatar->GetCalModel()->getMixer()->getAnimationTime() >= anim->getDuration())
                {
                    if (isToFinish() || m_currLoop >= (m_loopNumber -1))
                    {
                        avatar->GetCalModel()->getMixer()->clearCycle(m_motionRef->getAnimID(), 0);
                        setAnimStarted(false);
                    }
                    else
                    {
                        m_currLoop++;
                    }
                }
            }
            else
            {
                //check if anim cycle finished
                if (avatar->GetCalModel()->getMixer()->getAnimationTime() >= anim->getDuration())
                {
                    setAnimStarted(false);
                }
            }
        }
        else
        {
            if (this->isAnimLoop())
            {
                avatar->GetCalModel()->getMixer()->blendCycle(m_motionRef->getAnimID(), 1.0f, 0.0f);
                m_currLoop = 0;
            }
            else
            {
                avatar->GetCalModel()->getMixer()->executeAction(m_motionRef->getAnimID(), 0, 0);
            }
            setAnimStarted(true);
        }
    }

    return;
}

void TimeLineMotion::ExecuteModifiers(float elapsedSeconds, Avatar* avatar)
{
    //Execute all modfiers
    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Apply(elapsedSeconds, avatar);
        }
    }
}

void TimeLineMotion::Reset()
{
    TimeLineObject::Reset();

    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Reset();
        }
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