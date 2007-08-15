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

    ResetParams();

}

/// \brief Releases all resources and objects related to this TimeLineMotion
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

/// \brief Resets parameters of this TimeLineMotion
void TimeLineMotion::ResetParams()
{
    // initial paramters here
    m_currSubMotion = -1;  // no any motion
    setToFinish(false);
    m_currLoop = -1;  // not started yet
    setAnimStarted(false);
    m_animTime = -1;
}

/**
 * \brief Adds modifier that can change animation while this TimeLineMotion is executed
 *
 * \param ft::TimeLineModifier * modifier - modifier to add
 * \return bool - true if modifier added successfuly
 **/
bool TimeLineMotion::AddModifier(TimeLineModifier* modifier)
{
    std::cout << " AddModifier " << modifier->toString() << " to " << toString() << std::endl;
    m_vModifiers.push_back(modifier);
	return true;
}
//        bool  TimeLineMotion::RemoveMdofier(TimeLineModifier* modfier);
//        TimeLineModifier* TimeLineMotion::GetModifier(int);

/**
 * \brief Adds TimeLineMotion on new track
 *
 * \param ft::TimeLineMotion * - motion to add
 * \return bool - true if TimeLineMotion added to new track
 **/
bool TimeLineMotion::AddTrack(TimeLineMotion* trackMotion)
{
    std::cout << " AddOTrack " << trackMotion->toString() << " to " << toString() << std::endl;
    m_vTracks.push_back(trackMotion);
	return true;
}
//        bool  TimeLineMotion::RemoveTrack(TimeLineMotion* trackMotion);
//        TimeLineMotion* TimeLineMotion::GetTrack(int);


/// \brief Starts execution of this TimeLineMotion
void TimeLineMotion::Start()
{
    PrintDebug("Start");

    setStarted(true);

    if (m_motionRef!=NULL)
        m_animTime = 0;

    if (m_vObjects.size()>0)
    {
        //start first sub-motion if any
        m_currSubMotion = 0;
        TimeLineMotion* timeLineMotion = dynamic_cast<TimeLineMotion*>(m_vObjects[0]);
        timeLineMotion->Start();
    }

}
///\brief Stops execution of this TimeLineMotion
void TimeLineMotion::Stop()
{
    PrintDebug("Stop");
    setStarted(false);
}

/**
 * \brief Updates this TimeLineMotion at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
 **/
void TimeLineMotion::Execute(float elapsedSeconds, Avatar* avatar)
{
    ExecuteTracks(elapsedSeconds, avatar);
    bool anyStarted = ExecuteSubMotions(elapsedSeconds, avatar);
    ExecuteAnim(elapsedSeconds, avatar);

    if (!anyStarted && !isAnimStarted())
    {
        Stop();
    }
}

/**
 * \brief Updates all tracks at current frame 
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
 **/
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

// Returns 
//
/**
 * \brief Updates all sub-motions at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
 * \return bool - true if any submotion started, false if none submotion is started
 **/
bool TimeLineMotion::ExecuteSubMotions(float elapsedSeconds, Avatar* avatar)
{
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
            
            
            bool nextMotionToStart = false;

            if (!currMotion->isStarted() || IsBlendingToStart(currMotion, nextMotion, avatar))
            {
                nextMotionToStart = true;
            }

            if (nextMotionToStart && nextMotion !=NULL)
            {
                m_currSubMotion++;
                nextMotion->Start();
                nextMotion->Execute(elapsedSeconds, avatar);
                anyStarted = true; 
            }
            else
            {
                if (!currMotion->isStarted())
                    m_currSubMotion = -1;  // no more submotions to execute
            }
        }
    }


    //TODO : consider if next motion is shorter than blending overlap
    return anyStarted;
}


/**
 * \brief Updates motion (animation) related to this TimeLineMotion at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
 **/
void TimeLineMotion::ExecuteAnim(float elapsedSeconds, Avatar* avatar)
{
    if (m_motionRef == NULL)
        return;

    if (isAnimStarted())
    {
                int animId = m_motionRef->getAnimID();
                CalCoreAnimation* anim = avatar->GetCalCoreModel()->getCoreAnimation(animId);

                m_animTime += elapsedSeconds;  //TODO: ABAK: consider other animation time detection (maybe by cla3d function)

                float animTime = avatar->GetCalModel()->getMixer()->getAnimationTime();
                float animDuration;
                
                if (m_motionRef->isNullAnim())
                {
                    animDuration = 0;
                }
                else
                {
                    animDuration = anim->getDuration();
                }

                if (this->isAnimLoop())
                {
                    int i=0;
                    // check if current loop is finished
                    if ( m_animTime >= animDuration)
                    {
                        //if m_loopNumber is -1 this means that this motion is infinitive
                        if ( isToFinish() || (  m_loopNumber>=0   &&    (m_currLoop >= (m_loopNumber-1)) ) )
                        {
                            avatar->GetCalModel()->getMixer()->clearCycle(m_motionRef->getAnimID(), 0);
                            std::cout << " anim stopped (cycled)" << std::endl;
                            setAnimStarted(false);
                        }
                        else
                        {
                            m_currLoop++;
                            m_animTime = 0;
                        }
                    }
                    
                }
                else
                {
                    //check if anim finished
                    if (m_animTime >= animDuration)
                    {
                        std::cout << " anim stopped (action)" << std::endl;
                        setAnimStarted(false);
                    }
                }
    }
    else // anim not started 
    {
        if (this->isAnimLoop())
        {
            if (! m_motionRef->isNullAnim() )
            {
                avatar->GetCalModel()->getMixer()->blendCycle(m_motionRef->getAnimID(), 1.0f, 0.0f);
            }
            m_currLoop = 0;
        }
        else
        {
            if (! m_motionRef->isNullAnim() )
            {
                avatar->GetCalModel()->getMixer()->executeAction(m_motionRef->getAnimID(), 0.0f, 0.0f);
            }
        }
        setAnimStarted(true);
    }

    return;
}

/**
 * \brief Updates modifiers related to this TimeLineMotion at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
 **/
void TimeLineMotion::ExecuteModifiers(float elapsedSeconds, Avatar* avatar)
{
    //execute modifiers for track 
        // - TODO : in the future

    //execute modifiers for current submotion
    TimeLineMotion* currMotion = NULL;

    if (m_currSubMotion >= 0)
        currMotion = (TimeLineMotion*)m_vObjects[m_currSubMotion];

    if (currMotion != NULL)
        currMotion->ExecuteModifiers(elapsedSeconds, avatar);

    
    //Execute modfiers of this object
    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Apply(elapsedSeconds, avatar);
        }
    }
}

/**
 * \brief Determines if blending betwen currMotion and nextMotion should be started at current frame
 *
 * \param ft::TimeLineMotion * currMotion - currently being executed TimeLineMotion
 * \param ft::TimeLineMotion * nextMotion - next TimeLineMotion on TimeLine
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
 * \return bool - true if blending should be started, false otherwise
 **/
bool TimeLineMotion::IsBlendingToStart(TimeLineMotion* currMotion, TimeLineMotion* nextMotion, Avatar* avatar)
{
    bool result = false;
    TimeLineBlender* blender = currMotion->getBlender();
    if (blender != NULL)
    {
        if (!currMotion->isAnimLoop() || currMotion->isToFinish())
        {
            float animDuration = currMotion->GetMotionDuration(avatar);
            float animTime = currMotion->getAnimTime();
            

            if (animDuration>=0)
            {
                float animLeftTime = animDuration - animTime;
                if (animLeftTime <= blender->getOverlap())
                {
                    std::cout << toString() << " blending started for animLeftTime " << animLeftTime  << std::endl;
                    result = true;
                }
            }
        }
    }
    
    return result;
}

/**
 * \brief Returns duration of motion (animation) related to this TimeLineMotion
 *
 * \param ft::Avatar * avatar - avatar to which this modifier is assigned
 * \return float - duration of motion (animation)
 **/
float TimeLineMotion::GetMotionDuration(Avatar* avatar)
{
    float duration = -1;
    if (m_motionRef != NULL)
    {
        int animId = m_motionRef->getAnimID();
        CalCoreAnimation* anim = avatar->GetCalCoreModel()->getCoreAnimation(animId);
        if (anim != NULL)
        {
            duration = anim->getDuration();
        }
    }

    return duration;
}

/**
 * \brief Resets all objects related to this TimeLineMotion
 *
 **/
void TimeLineMotion::Reset(Avatar* avatar)
{
    TimeLineObject::Reset(avatar);

    if (m_motionRef != NULL && this->isAnimStarted() && this->isAnimLoop())
    {
        avatar->GetCalModel()->getMixer()->clearCycle(m_motionRef->getAnimID(), 0);
    }

    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Reset(avatar);
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