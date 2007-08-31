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
    setCurrentObject(NULL);
    setAnimToFinish(false);
    SetTerminated(false);
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


/// \brief Starts execution of this TimeLineMotion
void TimeLineMotion::Start(TimeLineContext* timeLineContext, float fade_in, float fade_out)
{
//    PrintDebug("Start");

    setStarted(true);

    if (m_first != NULL)
    {
        //start first sub-motion if any
        setCurrentObject(m_first);
        TimeLineMotion* timeLineMotion = dynamic_cast<TimeLineMotion*>(m_first);
        timeLineMotion->Start(timeLineContext, fade_in, fade_out);
    }

    if (m_motionRef!=NULL)
    {
        m_animTime = 0;
        StartAnim(timeLineContext, fade_in, fade_out);
    }

}
///\brief Stops execution of this TimeLineMotion
void TimeLineMotion::Stop(TimeLineContext* timeLineContext)
{
//    std::cout << " TimeLineMotion::Stop " << toString() << std::endl;
    setStarted(false);
}

/**
 * \brief Updates this TimeLineMotion at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
* \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLineMotion is assigned
 **/
void TimeLineMotion::Execute(float elapsedSeconds, TimeLineContext* timeLineContext)
{
    bool anyStarted = false;
    
    anyStarted = ExecuteSubMotions(elapsedSeconds, timeLineContext);

//    ExecuteAnim(elapsedSeconds, timeLineContext);
    CheckAnimToStop(elapsedSeconds, timeLineContext);

    if (isStarted() && !anyStarted && !isAnimStarted())
    {
        Stop(timeLineContext);
    }
}

/**
 * \brief Updates all sub-motions at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
* \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLineMotion is assigned
 * \return bool - true if any submotion started, false if none submotion is started
 **/
bool TimeLineMotion::ExecuteSubMotions(float elapsedSeconds, TimeLineContext* timeLineContext)
{
    bool anyStarted = false;
 
    TimeLineMotion* currMotion = NULL;
    TimeLineMotion* nextMotion = NULL;

    //only if current indicator is valid (note that <m_currSubMotion==-1> means that there is no motions to execute)
    if (getCurrentObject() !=NULL)
    {
        currMotion = (TimeLineMotion*)getCurrentObject();

        nextMotion = (TimeLineMotion*)currMotion->m_next;
    }

    if (currMotion != NULL)
    {
        
        if (currMotion->isStarted())
        {
            if (nextMotion!= NULL && nextMotion->isInterupting())
            {
                currMotion->setAnimToFinish(true);
            }
            currMotion->Execute(elapsedSeconds, timeLineContext);
            anyStarted = true;
            
            if (!isTerminated())
            {
                bool nextMotionToStart = false;
                bool isBlendingToStart = false;
                float fade_in = 0;
                float fade_out = 0;

                if (nextMotion !=NULL)
                {
                    if (!currMotion->isStarted())
                    {
                        nextMotionToStart = true;
                    } 
                    else    
                    {
                    //////////// CALCULATE BLENDING ////////////////////////

                        isBlendingToStart = IsBlendingToStart(currMotion, nextMotion, timeLineContext);
                        if (isBlendingToStart)
                        {
                            nextMotionToStart = true;

                            fade_in = currMotion->getBlender()->getOverlap();
    
//                            std::cout << toString() << " sets blendig params: fade_in " << fade_in << " fade_out " << fade_out << std::endl;
                        }
                       //////////// End of BLENDING calculation ///////////////////
                    }
                }

                if (nextMotionToStart)
                {
                    // calculate fade_out
                    if (nextMotion->getMotion() != NULL)
                    {
                        if (nextMotion->getBlender() != NULL)
                            fade_out = nextMotion->getBlender()->getOverlap();
                        else
                        {
                            //try to find blender of this motion if current submotion is last
//                                    TimeLineBlender * bl = getBlender();
                            //TimeLineMotion * mot = (TimeLineMotion*)currMotion->getNextObject();
                            if (getBlender() != NULL && nextMotion->getNextObject() == NULL)
                            {
                                fade_out = getBlender()->getOverlap();
                            }
                        }
                    }
                    else
                    {
                        TimeLineMotion* nextMotionWithAnim = NULL;
                        //TODO: should be found sigle submotion (not current and not started)
                        nextMotionWithAnim = GetFirstSubMotionWithAnim(nextMotion);

                        if (nextMotionWithAnim != NULL)
                        {
                            if (nextMotionWithAnim->getBlender() != NULL)
                            {
                                //get fade_out from blender of nextMotionWithAnim
                                fade_out = nextMotionWithAnim->getBlender()->getOverlap();
                            }
                            else
                            {
                                //try to get global blender from nextMotion
                                // if it has only one submotion
                                if (nextMotion->getBlender() != NULL && nextMotion->m_first->getNextObject() == NULL)
                                    fade_out = nextMotion->getBlender()->getOverlap();
                            }
                        }
                    }



                    //start clearCycle if loop anim for blending
                    if (currMotion->isStarted() && currMotion->isAnimLoop())
                    {

                        TimeLineMotion* blendMotion;
                        // find any of current sub-motions with anim
                        if (currMotion->getMotion() != NULL)
                            blendMotion = currMotion;
                        else
                            blendMotion = GetSubMotionWithAnim(currMotion);

                        if (blendMotion != NULL)
                        {
//                            std::cout << toString() << " clearCycle " << blendMotion->toString() << " for blending " << std::endl;
                            blendMotion->StopLoopAnim(timeLineContext, fade_in);
//                            timeLineContext->getAvatar()->GetCalModel()->getMixer()->clearCycle(blendMotion->getMotion()->getAnimID(),
//                                currMotion->getBlender()->getOverlap());
                        }
                    }

                    setCurrentObject(nextMotion);
                    nextMotion->Start(timeLineContext, fade_in, fade_out);
                    nextMotion->Execute(elapsedSeconds, timeLineContext);
                    anyStarted = true; 
                }
                else
                {
                    if (!currMotion->isStarted())
                        m_current = NULL;  // no more submotions to execute
                }
            }
        }
    }


    //TODO : consider if next motion is shorter than blending overlap
    return anyStarted;
}

void TimeLineMotion::StartAnim(TimeLineContext* timeLineContext, float fade_in, float fade_out)
{
        if (!isTerminated() && !isAnimToFinish())
        {
            if (this->isAnimLoop())
            {
                if (! m_motionRef->isNullAnim() )
                {
                    std::cout << toString() << " StartAnim-blendCycle with fade_in " << fade_in << std::endl;
                    timeLineContext->getAvatar()->GetCalModel()->getMixer()->blendCycle(m_motionRef->getAnimID(), 1.0f, fade_in);
                }
                m_currLoop = 0;
            }
            else
            {
                if (! m_motionRef->isNullAnim() )
                {
                    std::cout << toString() << " StartAnim-executeAction with fade_in " << fade_in << " fade_out " << fade_out << std::endl;
                    timeLineContext->getAvatar()->GetCalModel()->getMixer()->executeAction(m_motionRef->getAnimID(), fade_in, fade_out);
                }
            }
            setAnimStarted(true);
        }
}

bool TimeLineMotion::IsAnimInLastLoop()
{
    return (m_loopNumber>=0   &&    (m_currLoop >= (m_loopNumber-1)));
}
void TimeLineMotion::CheckAnimToStop(float elapsedSeconds, TimeLineContext* timeLineContext)
{
    if (m_motionRef == NULL)
        return;

    if (isAnimStarted())
    {
        if (isTerminated())
        {
            if (timeLineContext->stop_immediate)
            {
                StopAnimImmediate(timeLineContext);
                setAnimStarted(false);
                return;
            }
            else
            {
                setAnimToFinish(true);
            }
        }

        int animId = m_motionRef->getAnimID();
        CalCoreAnimation* anim = timeLineContext->getAvatar()->GetCalCoreModel()->getCoreAnimation(animId);

        m_animTime += elapsedSeconds;  //TODO: ABAK: consider other animation time detection (maybe by cla3d function)

        float animTime = timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationTime();
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
           // check if current loop is finished
            if ( m_animTime >= animDuration)
            {
                //if m_loopNumber is -1 this means that this motion is infinitive
                if ( isAnimToFinish() || IsAnimInLastLoop())
                {
                    std::cout << " loop anim stop: m_animTime " << m_animTime << " anim duration " <<  animDuration << std::endl;      
                    StopLoopAnim(timeLineContext, 0);
//                            timeLineContext->getAvatar()->GetCalModel()->getMixer()->clearCycle(m_motionRef->getAnimID(), 0);
//                            std::cout << " anim stopped (cycled)" << std::endl;
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
                std::cout << " action anim stopped (action)" << std::endl;
                setAnimStarted(false);
            }
        }
    }
}

/**
 * \brief Updates motion (animation) related to this TimeLineMotion at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLineMotion is assigned
 **/
void TimeLineMotion::ExecuteAnim(float elapsedSeconds, TimeLineContext* timeLineContext)
{
//    if (m_motionRef == NULL)
//        return;
//
//    if (isAnimStarted())
//    {
//                if (isTerminated())
//                {
//                    if (timeLineContext->stop_immediate)
//                    {
//                        StopAnimImmediate(timeLineContext);
//                        setAnimStarted(false);
//                        return;
//                    }
//                    else
//                    {
//                        setAnimToFinish(true);
//                    }
//                }
//
//                int animId = m_motionRef->getAnimID();
//                CalCoreAnimation* anim = timeLineContext->getAvatar()->GetCalCoreModel()->getCoreAnimation(animId);
//
//                m_animTime += elapsedSeconds;  //TODO: ABAK: consider other animation time detection (maybe by cla3d function)
//
//                float animTime = timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationTime();
//                float animDuration;
//                
//                if (m_motionRef->isNullAnim())
//                {
//                    animDuration = 0;
//                }
//                else
//                {
//                    animDuration = anim->getDuration();
//                }
//
//                if (this->isAnimLoop())
//                {
//                    int i=0;
//                    // check if current loop is finished
//                    if ( m_animTime >= animDuration)
//                    {
//                        //if m_loopNumber is -1 this means that this motion is infinitive
//                        if ( isAnimToFinish() || (  m_loopNumber>=0   &&    (m_currLoop >= (m_loopNumber-1)) ) )
//                        {
//                            std::cout << " anim stop: m_animTime " << m_animTime << " anim duration " <<  animDuration << std::endl;      
//                            StopLoopAnim(timeLineContext, 0);
////                            timeLineContext->getAvatar()->GetCalModel()->getMixer()->clearCycle(m_motionRef->getAnimID(), 0);
////                            std::cout << " anim stopped (cycled)" << std::endl;
//                            setAnimStarted(false);
//                        }
//                        else
//                        {
//                            m_currLoop++;
//                            m_animTime = 0;
//                        }
//                    }
//                    
//                }
//                else
//                {
//                    //check if anim finished
//                    if (m_animTime >= animDuration)
//                    {
//                        std::cout << " anim stopped (action)" << std::endl;
//                        setAnimStarted(false);
//                    }
//                }
//    }
//    else // anim not started 
//    {
    //    if (!isTerminated() && !isAnimToFinish())
    //    {
    //        if (this->isAnimLoop())
    //        {
    //            if (! m_motionRef->isNullAnim() )
    //            {
    //                timeLineContext->getAvatar()->GetCalModel()->getMixer()->blendCycle(m_motionRef->getAnimID(), 1.0f, 0.0f);
    //            }
    //            m_currLoop = 0;
    //        }
    //        else
    //        {
    //            if (! m_motionRef->isNullAnim() )
    //            {
    //                float fade_out = 0;
    ////                if (this->getBlender() != NULL)
    ////                    fade_out = this->getBlender()->getOverlap();
    //                    timeLineContext->getAvatar()->GetCalModel()->getMixer()->executeAction(m_motionRef->getAnimID(), 0.0f, fade_out);
    //            }
    //        }
    //        setAnimStarted(true);
    //    }
    //}

    return;
}

/**
 * \brief Updates modifiers related to this TimeLineMotion at current frame
 *
 * \param float elapsedSeconds - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLineMotion is assigned
 **/
void TimeLineMotion::ExecuteModifiers(float elapsedSeconds, TimeLineContext* timeLineContext)
{
    //execute modifiers for track 
        // - TODO : in the future

    //execute modifiers for current submotion
    TimeLineMotion* currMotion = (TimeLineMotion*)getCurrentObject();;

    if (currMotion != NULL)
        currMotion->ExecuteModifiers(elapsedSeconds, timeLineContext);

    
    //Execute modfiers of this object
    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Apply(elapsedSeconds, timeLineContext);
        }
    }
}

void TimeLineMotion::StopLoopAnim(TimeLineContext* timeLineContext, float fade_out)
{
    if (m_motionRef != NULL)
    {
        std::cout << toString() << " StopLoopAnim-clearCycle with fade out " << fade_out << std::endl;
        timeLineContext->getAvatar()->GetCalModel()->getMixer()->clearCycle(m_motionRef->getAnimID(), fade_out);
    }
}

void TimeLineMotion::StopAnimImmediate(TimeLineContext* timeLineContext)
{
    if (m_motionRef != NULL && this->isAnimStarted())
    {
        if (this->isAnimLoop())
        {
            StopLoopAnim(timeLineContext, 0);
        }
        else
        {
            timeLineContext->getAvatar()->GetCalModel()->getMixer()->removeAction(m_motionRef->getAnimID());
        }
    }
}

/**
 * \brief Finds any current submotion with non-NULL animation for given TimeLineMotion.
 * Additional constraint is that found sub-motion should last submotion of its parent
 *
 * \param ft::TimeLineMotion* motion - currently being executed TimeLineMotion
 * \return ft::TimeLineMotion* - submotion with non-NULL anim
 **/
TimeLineMotion* TimeLineMotion::GetSubMotionWithAnim(TimeLineMotion* motion)
{
    TimeLineMotion* result = NULL;
    
    TimeLineMotion* currSubObject = (TimeLineMotion* )motion->getCurrentObject();

    //if there is current subobject and it is last subobject
    if (currSubObject != NULL && currSubObject->getNextObject() == NULL)
    {
        if (currSubObject->getMotion() != NULL && currSubObject->isStarted())
        {
            result = currSubObject;
        }
        else
        {
            result = GetSubMotionWithAnim(currSubObject);
        }
    }
    return result;
}

TimeLineMotion* TimeLineMotion::GetFirstSubMotionWithAnim(TimeLineMotion* motion)
{
    TimeLineMotion* result = NULL;
    
    TimeLineMotion* firstObject = (TimeLineMotion* )motion->m_first;

    //if there is current subobject and it is last subobject
    if (firstObject != NULL)
    {
        if (firstObject->getMotion() != NULL)
        {
            result = firstObject;
        }
        else
        {
            result = GetFirstSubMotionWithAnim(firstObject);
        }
    }
    return result;
}

/**
 * \brief Determines if blending betwen currMotion and nextMotion should be started at current frame
 *
 * \param ft::TimeLineMotion * currMotion - currently being executed TimeLineMotion
 * \param ft::TimeLineMotion * nextMotion - next TimeLineMotion on TimeLine
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLineMotion is assigned
 * \return bool - true if blending should be started, false otherwise
 **/
bool TimeLineMotion::IsBlendingToStart(TimeLineMotion* currMotion, TimeLineMotion* nextMotion, TimeLineContext* timeLineContext)
{
    bool result = false;
    TimeLineBlender* blender = currMotion->getBlender();
    if (blender != NULL)
    {
        if (!currMotion->isAnimLoop() || currMotion->isAnimToFinish() || currMotion->IsAnimInLastLoop())
        {
            float animDuration = -1; 
            float animTime = -1; 
            TimeLineMotion* blendMotion;
            
            // find any of current sub-motions with anim
            if (currMotion->getMotion() != NULL)
            {
                blendMotion = currMotion;
            }
            else
            {
                blendMotion = GetSubMotionWithAnim(currMotion);
            }

            //if there is any sub-motion with anim
            if (blendMotion != NULL)
            {
                animDuration = blendMotion->GetMotionDuration(timeLineContext);
                animTime = blendMotion->getAnimTime();
            }

            if (animDuration>=0)
            {
                float animLeftTime = animDuration - animTime;
                if (animLeftTime <= blender->getOverlap())
                {
                    std::cout << toString() << " blending started for animLeftTime " << animLeftTime
                        << " between " << blendMotion->toString() << " and " << nextMotion->toString() << std::endl;
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
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this TimeLineMotion is assigned
 * \return float - duration of motion (animation)
 **/
float TimeLineMotion::GetMotionDuration(TimeLineContext* timeLineContext)
{
    float duration = -1;
    if (m_motionRef != NULL)
    {
        int animId = m_motionRef->getAnimID();
        CalCoreAnimation* anim = timeLineContext->getAvatar()->GetCalCoreModel()->getCoreAnimation(animId);
        if (anim != NULL)
        {
            duration = anim->getDuration();
        }
    }

    return duration;
}

void TimeLineMotion::SetTerminated(bool set)
{
    m_isTerminated = set; 

    //all submotions
    if (getCurrentObject() != NULL)
    {
        TimeLineMotion* timeLineMotion = dynamic_cast<TimeLineMotion*>(getCurrentObject());
        timeLineMotion->SetTerminated(true);
    }
}

/**
 * \brief Resets all objects related to this TimeLineMotion
 *
 **/
void TimeLineMotion::Reset(TimeLineContext* timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);

    StopAnimImmediate(timeLineContext);

    if (m_vModifiers.size() > 0)
    {
        for (int m=0; m<(int)m_vModifiers.size(); m++)
        {
            m_vModifiers[m]->Reset(timeLineContext);
        }
    }

    ResetParams();
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