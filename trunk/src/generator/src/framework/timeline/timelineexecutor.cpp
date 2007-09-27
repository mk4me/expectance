/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelineexecutor.h"

using namespace ft;
using namespace std;

void clearExecItem(timeline_exec_item_t &item)
{
    item.motion = NULL;
    item.anim = NULL;
    item.animTime = 0;
    item.animDuration = 0;
    item.lastStep= false;
    item.loopStep= 0;
    item.lastTime = 0;
}

void copyExecItem(timeline_exec_item_t &src, timeline_exec_item_t &dst)
{
    dst.motion = src.motion;
    dst.anim = src.anim;
    dst.animTime = src.animTime;
    dst.animDuration = src.animDuration;
    dst.lastStep = src.lastStep;
    dst.loopStep = src.loopStep;
    dst.lastTime = src.lastTime;
}



/// \brief constructor
TimeLineExecutor::TimeLineExecutor(void)
{
    LOCAL_DEBUG = true;
    Reset();
}

/// \brief Releases all resources and objects related to this TimeLineExecutor
void TimeLineExecutor::Destroy(void)
{
}

void TimeLineExecutor::Reset()
{
    setState(EXEC_STATE_NOT_INITED);

    clearExecItem(m_prevMotion);
    clearExecItem(m_currMotion);
    clearExecItem(m_nextMotion);

    m_prevBlender = 0;
    m_currBlender = 0;
    m_nextBlender = 0;

    m_lastEvent = EXEC_EVENT_NONE;
}

void TimeLineExecutor::Initiate(TimeLine* timeLine, TimeLineContext* timeLineContext)
{
    setTimeLine(timeLine);
    setCtx(timeLineContext);
    ChangeState(EXEC_STATE_WAIT);
}

void TimeLineExecutor::setState(int state)
{ 
    m_state = state; 
}

int TimeLineExecutor::getState() 
{ 
    return m_state; 
}

void TimeLineExecutor::Start()
{
    setTerminated(false);
}

void TimeLineExecutor::StopRequest()
{
    setTerminated(true);
}

//////////////////// STATES ////////////////////
void TimeLineExecutor::ChangeState(int newState)
{
    int oldState = getState();
    if (LOCAL_DEBUG)
        cout  << " TimeLineExecutor::ChangeState to "<< _GET_STATE_NAME(newState) << "(old:"
                << _GET_STATE_NAME(oldState) << ")" << endl;

    
    setState(newState);
    EntryState(newState, oldState);
}

void TimeLineExecutor::EntryState(int state, int oldState)
{
    switch(state)
    {
    case EXEC_STATE_NOT_INITED: break;
    case EXEC_STATE_WAIT: EntryWaitState(oldState); break;
    case EXEC_STATE_SINGLE: EntrySingleState(oldState); break;
    case EXEC_STATE_OVERLAP: EntryOverlapState(oldState); break;
    case EXEC_STATE_FADE_IN: EntryFadeInState(oldState); break;
    case EXEC_STATE_FADE_OUT: EntryFadeOutState(oldState); break;
    default: break;
    }
}

void TimeLineExecutor::UpdateState(int state)
{
    switch(state)
    {
    case EXEC_STATE_NOT_INITED: break;
    case EXEC_STATE_WAIT: UpdateWaitState(); break;
    case EXEC_STATE_SINGLE: 
        UpdateSingleState(); 
        break;
    case EXEC_STATE_OVERLAP: UpdateOverlapState(); break;
    case EXEC_STATE_FADE_IN: UpdateFadeInState(); break;
    case EXEC_STATE_FADE_OUT: UpdateFadeOutState(); break;
    default: break;
    }
}

void TimeLineExecutor::EntryWaitState(int oldState)
{
}

void TimeLineExecutor::EntrySingleState(int oldState)
{
    if (oldState != EXEC_STATE_FADE_IN && oldState != EXEC_STATE_OVERLAP)
    {
        StartNextMotion();
    }
}

void TimeLineExecutor::EntryOverlapState(int oldState)
{
    StopLoopAnim();
    StartNextMotion();
}

void TimeLineExecutor::EntryFadeInState(int oldState)
{
    StartNextMotion();
}

void TimeLineExecutor::EntryFadeOutState(int oldState)
{
   if (m_currMotion.motion->isAnimLoop())
   {
       StopLoopAnim();
   }
   else
   {
       //when is not terminated the action should finish automatically
        if (isTerminated())
        {
            StopActionAnim();
        }
   }
    
}

void TimeLineExecutor::EntryTerminatedState(int oldState)
{
    if (m_currMotion.motion !=NULL && m_currMotion.anim != NULL)
    {
        if (m_currMotion.motion->isAnimLoop())
            StopLoopAnim();
        else
            StopActionAnim();
    }
}



void TimeLineExecutor::UpdateWaitState()
{
    if (!isTerminated())
    {
        if (!getTimeLine()->isEmpty())
        {
            if (m_currBlender > 0)
                ChangeState(EXEC_STATE_FADE_IN);
            else
                ChangeState(EXEC_STATE_SINGLE);
        }
    }
    else
    {
        ChangeState(EXEC_STATE_TERMINATED);
    }
}

void TimeLineExecutor::UpdateSingleState()
{
    if (m_currMotion.motion != NULL)
    {
        if (m_currMotion.anim == NULL )
        {
            if (!isTerminated())
            {
                if (m_nextMotion.motion != NULL)
                {
                    if (m_currBlender > 0)
                        ChangeState(EXEC_STATE_FADE_IN);
                    else
                        ChangeState(EXEC_STATE_SINGLE);
                }
                else
                {
                    ChangeState(EXEC_STATE_WAIT);
                }
            }
            else
            {
                ChangeState(EXEC_STATE_TERMINATED); //anims (if still sterted) will be finished at entry to TERMINATED state
            }
        }
        else
        {
            if (!isTerminated())
            {
                float animLeftTime = m_currMotion.animDuration - m_currMotion.animTime;

                if (!m_currMotion.motion->isAnimLoop() || m_currMotion.lastStep)
                {
                    if (m_currBlender > 0 && m_currBlender >= animLeftTime)
                    {
                        if (m_nextMotion.motion != NULL)
                            ChangeState(EXEC_STATE_OVERLAP);
                        else
                            ChangeState(EXEC_STATE_FADE_OUT);
                    }
                }
            }
            else
            {
                bool toStopNow = false;
                if (getCtx()->stop_immediate)
                {
                    toStopNow = true;
                }
                else
                {
                    float animLeftTime = m_currMotion.animDuration - m_currMotion.animTime;
                    if (m_currBlender >= animLeftTime)
                    {
                        toStopNow = true;
                    }
                }

                if (toStopNow)
                {
                    if (m_currBlender > 0)
                        ChangeState(EXEC_STATE_FADE_OUT);
                    else
                        ChangeState(EXEC_STATE_TERMINATED);
                }
            }
        }
    }
}

void TimeLineExecutor::UpdateOverlapState()
{
    if (m_prevMotion.motion != NULL)
    {
//        cout << " UpdateOverlapState m_prevMotion.anim " << m_prevMotion.anim << endl;
        if (m_prevMotion.anim == NULL)
        {
            ChangeState(EXEC_STATE_SINGLE);
        }
    }
}

void TimeLineExecutor::UpdateFadeInState()
{
    if (m_currMotion.motion != NULL)
    {
        if (m_currMotion.anim == NULL)
        {
            if (m_nextMotion.motion != NULL)
            {
                ChangeState(EXEC_STATE_SINGLE);
            }
            else
            {
                ChangeState(EXEC_STATE_WAIT);
            }
        }
        else
        {
            float animLeftTime = m_currMotion.animDuration - m_currMotion.animTime;
            if (m_prevBlender > 0 && m_prevBlender >= animLeftTime)
            {
                ChangeState(EXEC_STATE_SINGLE);
            }
        }
    }

}

void TimeLineExecutor::UpdateFadeOutState()
{
    if (m_currMotion.motion != NULL)
    {
        if (m_currMotion.anim == NULL)
        {
            if (!isTerminated())
            {
                if (m_nextMotion.motion != NULL)
                {
                    ChangeState(EXEC_STATE_SINGLE);
                }
                else
                {
                    ChangeState(EXEC_STATE_WAIT);
                }
            }
            else
            {
                ChangeState(EXEC_STATE_TERMINATED);
            }
        }
    }
    else
    {
        cout << " ERR: TimeLineExecutor::UpdateFadeOutState() " << " null motion for " 
                            << _GET_STATE_NAME(getState()) << " !!! " << endl;
    }
}

void TimeLineExecutor::UpdateTerminatedState()
{
    if (!isTerminated())
    {
        ChangeState(EXEC_STATE_WAIT);
    }
}

void TimeLineExecutor::StartNextMotion()
{
    if (isTerminated())
    {
        return;
    }

    if (m_nextMotion.motion != NULL)
    {
        float fade_in = 0;
        float fade_out = 0;
        if (m_currBlender > 0)
            fade_in = m_currBlender;
        if (m_nextBlender > 0)
            fade_out = m_nextBlender;

        if (!m_nextMotion.motion->getMotion()->isNullAnim())
        {
            if (m_nextMotion.motion->isAnimLoop())
            {
                std::cout << " StartAnim-blendCycle "  << m_nextMotion.motion->toString() << " with fade_in " << fade_in << std::endl;
                getCtx()->getAvatar()->GetCalModel()->getMixer()->blendCycle(m_nextMotion.motion->getMotion()->getAnimID(),
                        1.0f, fade_in);

                m_nextMotion.anim = FindAddedAnimInCal3d(CalAnimation::TYPE_CYCLE);
                if (m_nextMotion.anim == NULL)
                {
                    cout << "ERROR: TimeLineExecutor::StartNextMotion - no cycle anim extracted from Cal3d after adding motion " << endl;
                }

            }
            else
            {
                std::cout << " StartAnim-executeAction with  " << m_nextMotion.motion->toString() << " fade_in " << fade_in << " fade_out " << fade_out << std::endl;
                getCtx()->getAvatar()->GetCalModel()->getMixer()->executeAction(m_nextMotion.motion->getMotion()->getAnimID(),
                        fade_in, fade_out);

                m_nextMotion.anim = FindAddedAnimInCal3d(CalAnimation::TYPE_ACTION);
                if (m_nextMotion.anim == NULL)
                {
                    cout << "ERROR: TimeLineExecutor::StartNextMotion - no action anim extracted from Cal3d after adding motion " << endl;
                }

            }
        }
        m_prevBlender = m_currBlender;
        m_animChanged = true;
    }
    ExchangeExecItems();
}

CalAnimation* TimeLineExecutor::FindAddedAnimInCal3d(int animType)
{
    CalAnimation* anim = NULL;
    CalMixer* mixer = getCtx()->getAvatar()->GetCalModel()->getMixer();
    
    if (animType == CalAnimation::TYPE_ACTION)
    {
        if (mixer->getAnimationActionList().size() > 0)
        {
            anim = mixer->getAnimationActionList().front();
        }
    }
    else if (animType == CalAnimation::TYPE_CYCLE)
    {
        if (mixer->getAnimationCycle().size() > 0)
        {
            anim = mixer->getAnimationCycle().front();
            
        }
    }

    return anim;
}

void TimeLineExecutor::StopLoopAnim()
{
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL 
                && m_currMotion.motion->getMotion() != NULL && m_currMotion.motion->isAnimLoop())
    {
        float fade_out = 0;
        if (m_currBlender > 0)
            fade_out = m_currBlender;
    
        std::cout << " StopLoopAnim-clearCycle " << m_currMotion.motion->toString() << " with fade out " << fade_out << std::endl;
        getCtx()->getAvatar()->GetCalModel()->getMixer()->clearCycle(m_currMotion.motion->getMotion()->getAnimID(), fade_out);
    }
}

void TimeLineExecutor::StopActionAnim()
{
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL 
                && m_currMotion.motion->getMotion() != NULL && !m_currMotion.motion->isAnimLoop())
    {
        float fade_out = 0;
        if (m_currBlender > 0)
            fade_out = m_currBlender;
    
        std::cout << " StopActionAnim-removeAction " << m_currMotion.motion->toString() << " [ no fade out used ]" << endl;
        getCtx()->getAvatar()->GetCalModel()->getMixer()->removeAction(m_currMotion.motion->getMotion()->getAnimID());
    }
}


void TimeLineExecutor::ExchangeExecItems()
{
    copyExecItem(m_currMotion, m_prevMotion);
    copyExecItem(m_nextMotion, m_currMotion);
    clearExecItem(m_nextMotion);
}

//////////////////////////// end of STATES ////////////////////////

void TimeLineExecutor::UpdateMotions(const double elapsedSeconds)
{

    if (!isTerminated() && CheckEpmtyFrame())  //check if there is no any animation in mixer
        cout << " WARN: empty frame !!!! " << endl;

    m_lastEvent = EXEC_EVENT_NONE;
    m_animChanged = false;
    m_animNewCycle = false;

    UpdateExecItem(m_prevMotion);
    UpdateExecItem(m_currMotion);


    IdentifyNextMotion();
    IdentifyBlenders();

    LimitCurrentBlender();

    CheckInterrupting();

    //if (getState() != EXEC_STATE_TERMINATED)
    //{
    //    CheckTermination();
    //}   

    int previousState = getState();

    UpdateState(getState());

    if (previousState!=getState())
    {
        m_lastEvent = EXEC_EVENT_STATE_CHANGED;
    }


    UpdateContext();

    //TODO: implement termination 

    // REMOVAL EXECUTED MOTIONS
    if ( m_lastEvent == EXEC_EVENT_STATE_CHANGED && getCtx()->remove_after_execution
            && previousState != EXEC_STATE_NOT_INITED && previousState != EXEC_STATE_WAIT && getTimeLine() != NULL)
    {
        if (!getTimeLine()->isEmpty())
        {
            RemoveExecutedMotions();  //TODO: uncomment if executor implemented
        }
    }

}

void TimeLineExecutor::CheckTermination()
{
}

void TimeLineExecutor::UpdateContext()
{
    
    getCtx()->prevAnim = m_prevMotion.anim;
    getCtx()->currAnim = m_currMotion.anim;
    getCtx()->prevAnimTime = m_prevMotion.animTime;
    getCtx()->prevAnimDuration = m_prevMotion.animDuration;
    getCtx()->currAnimTime = m_currMotion.animTime;
    getCtx()->currAnimDuration = m_currMotion.animDuration;
    getCtx()->prevOverlap = m_prevBlender;
    getCtx()->currOverlap = m_currBlender;
    getCtx()->exec_state = getState();
    getCtx()->exec_event = m_lastEvent;
    getCtx()->anim_changed = m_animChanged;
    getCtx()->anim_new_cycle = m_animNewCycle;

}


//void UpdateTermination()
//{
//    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL)
//    {
//        if (getCtx()->stop_immediate)
//        {
//            StopAnimImmediate();
//        }
//        else
//        {
//            setAnimToFinish(true);
//        }
//    }
//}


void TimeLineExecutor::UpdateExecItem(timeline_exec_item_t &item)
{
    if (item.motion != NULL)
    {
        if (item.anim != NULL)
        {
            //check if anim is still in Mixer
            if (IsAnimStillActive(item.anim))
            {
                item.lastTime = item.animTime;
                if (item.anim->getType() == CalAnimation::TYPE_CYCLE 
                        && item.anim->getState() == CalAnimation::STATE_SYNC)
                {
                    item.animTime = getCtx()->getAvatar()->GetCalModel()->getMixer()->getAnimationTime();
                    item.animDuration = getCtx()->getAvatar()->GetCalModel()->getMixer()->getAnimationDuration();
                    CheckLoopAnimStop(item);

                }
                else  //actions and cycles in ASYNC state
                {
                    item.animTime = item.anim->getTime();
                    item.animDuration = item.anim->getCoreAnimation()->getDuration();
//                    cout << " animTime: " << item.animTime << " dur " << item.animDuration << endl;
                }
            }
            else
            {
                MarkAnimStopped(item);
            }
        }
    }
}

void TimeLineExecutor::CheckInterrupting()
{
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL && m_currMotion.motion->isAnimLoop())
    {
        if (m_nextMotion.motion!= NULL && m_nextMotion.motion->isInterupting())
            m_currMotion.lastStep = true;
    }
}

void TimeLineExecutor::MarkAnimStopped(timeline_exec_item_t &item)
{
    item.anim = NULL;
    item.anim = 0;
    item.animDuration = 0;
}

void TimeLineExecutor::IdentifyNextMotion()
{
    TimeLineMotion *nextMotion = NULL;

    if (m_currMotion.motion == NULL)
    {
        if (!getTimeLine()->isEmpty())
        {
            nextMotion = GetSubMotionWithAnim((TimeLineMotion*)getTimeLine()->getFirstObject());
        }
    }
    else
    {
        TimeLineMotion *parentWithNext = GetParentWithNext(m_currMotion.motion);
        if (parentWithNext != NULL)
        {
            nextMotion = GetSubMotionWithAnim((TimeLineMotion*)parentWithNext->getNextObject());
        }
    }

    if (nextMotion != NULL)
    {
        m_nextMotion.motion = nextMotion;
    }
    else
    {
        clearExecItem(m_nextMotion);
    }
}

void TimeLineExecutor::CheckLoopAnimStop(timeline_exec_item_t &item)
{
        if (item.motion!= NULL && item.anim!= NULL && item.motion->isAnimLoop())
        {
            if (item.lastTime > item.animTime)  //end of current cycle 
            {
                if (!item.lastStep)
                {
                    m_animNewCycle = true;
                    item.loopStep += 1;

                    int loopNumber = item.motion->getLoopNumber();
                    if (loopNumber >= 0)
                    {
                        if (item.loopStep >= loopNumber)
                            item.lastStep = true;
                    }
                }
                else
                {
                    StopLoopAnim();
                    MarkAnimStopped(item);
                }
            }
        }
}


bool TimeLineExecutor::IsAnimStillActive(CalAnimation* anim)
{
    bool result = false;

    CalMixer* mixer = getCtx()->getAvatar()->GetCalModel()->getMixer();

    if (anim->getType() == CalAnimation::TYPE_ACTION)
    {
        if (mixer->getAnimationActionList().size() > 0)
        {
            std::list<CalAnimationAction *>::iterator iterator = mixer->getAnimationActionList().begin();
            while(iterator != mixer->getAnimationActionList().end())
            {
                if((*iterator) == anim )
                    result = true;
                iterator++;
            }
        }
    }
    else if (anim->getType() == CalAnimation::TYPE_CYCLE)
    {
        if (mixer->getAnimationCycle().size() > 0)
        {
            std::list<CalAnimationCycle *>::iterator iterator = mixer->getAnimationCycle().begin();
            while(iterator != mixer->getAnimationCycle().end())
            {
                if((*iterator) == anim )
                    result = true;
                iterator++;
            }
        }
    }

    return result;
}

TimeLineMotion* TimeLineExecutor::GetSubMotionWithAnim(TimeLineMotion* motion)
{
    TimeLineMotion* result = NULL;

    if (motion->getMotion() != NULL)
    {
        result = motion;
    }
    else
    {
        TimeLineMotion* firstSubObject = (TimeLineMotion* )motion->getFirstObject();
        if (firstSubObject != NULL)
        {
            if (firstSubObject->getMotion() != NULL)
                result = firstSubObject;
            else
                result = GetSubMotionWithAnim(firstSubObject);
        }
    }
    
    return result;
}


TimeLineMotion* TimeLineExecutor::GetParentWithNext(TimeLineMotion* motion)
{
    TimeLineMotion* result = NULL;

    if (motion->getNextObject() != NULL)
    {
        result = motion;
    }
    else
    {
        TimeLineMotion* parent = (TimeLineMotion* )motion->getParent();

        if (parent != NULL)
        {
            if (parent->getNextObject() != NULL)
                result = parent;
            else
                result = GetParentWithNext(parent);
        }
    }
    
    return result;
}

//top parent in hierarchy but lower than TimeLine
TimeLineMotion* TimeLineExecutor::GetTopParentInHierarchy(TimeLineMotion* motion)
{
    TimeLineMotion* result = NULL;

    if (motion != NULL)
    {
        TimeLineMotion* parent = (TimeLineMotion*)motion->getParent();

        if (parent != NULL)
        {
            if (parent == getTimeLine())
                result = motion;
            else
                result = GetTopParentInHierarchy(parent);
        }
    }
    else
    {
        result = NULL;
    }
    
    return result;
}

TimeLineMotion* TimeLineExecutor::GetParentWithBlender(TimeLineMotion* motion)
{
    TimeLineMotion* result = NULL;

    //if motion has parent and if it is last subobject of its parent
    if (motion->getParent() != NULL && motion->getParent()->GetLastSubObject() == motion)
    {
        TimeLineMotion* parentMotion = (TimeLineMotion*)motion->getParent();
        if (parentMotion->getBlender() != NULL)
        {
            result = parentMotion;
        }
        else
        {
            result = GetParentWithBlender(parentMotion);
        }
    }
    
    return result;
}



void TimeLineExecutor::IdentifyBlenders()
{
    m_currBlender = 0;
    m_nextBlender = 0;

    if (m_currMotion.motion != NULL)
    {
        if (m_currMotion.motion->getBlender() != NULL)
        {
            m_currBlender = m_currMotion.motion->getBlender()->getOverlap();
        }
        else 
        {
            TimeLineMotion* parentWithBlender = GetParentWithBlender(m_currMotion.motion);
            if (parentWithBlender != NULL)
                m_currBlender = parentWithBlender->getBlender()->getOverlap();
        }
    }

    if (m_nextMotion.motion != NULL)
    {
        if (m_nextMotion.motion->getBlender() != NULL)
        {
            m_nextBlender = m_nextMotion.motion->getBlender()->getOverlap();
        }
        else 
        {
            TimeLineMotion* parentWithBlender = GetParentWithBlender(m_nextMotion.motion);
            if (parentWithBlender != NULL)
                m_nextBlender = parentWithBlender->getBlender()->getOverlap();
        }
    }
}

void TimeLineExecutor::LimitCurrentBlender()
{
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL && m_currBlender > 0)
    {
        float animLeftTime = m_currMotion.animDuration - m_currMotion.animTime;
        if (m_currBlender > animLeftTime ) //&& animLeftTime > 0.1f)
        {
            //cout << " Blender " << m_currBlender << " linimted to " << animLeftTime << " because of anim left time " << endl;
            m_currBlender = animLeftTime;
        }
    }


}

void TimeLineExecutor::UpdateModifiers(const double elapsedSeconds)
{
    // apply modifiers for current and prev motion 

    // for curr motion
    TimeLineMotion* motionWithModifier = m_currMotion.motion;

    while (motionWithModifier != NULL)
    {
        UpdateModifiersForMotion(motionWithModifier, elapsedSeconds);
        motionWithModifier = (TimeLineMotion*)motionWithModifier->getParent();
    }

    //for prev motion
    if (m_prevMotion.motion != NULL)
    {
        motionWithModifier = m_prevMotion.motion;

        while (motionWithModifier != NULL)
        {
            if ( ! IsParent(m_currMotion.motion, motionWithModifier) )
            {
                UpdateModifiersForMotion(motionWithModifier, elapsedSeconds);
                motionWithModifier = (TimeLineMotion*)motionWithModifier->getParent();
            }
            else
            {
                break;
            }
        }
    }
}

void TimeLineExecutor::UpdateModifiersForMotion(TimeLineMotion* motion, float elapsedSeconds)
{
    if (motion->m_vModifiers.size() > 0)
    {
        for (int m=0; m <(int)motion->m_vModifiers.size(); m++)
        {
            motion->m_vModifiers[m]->Apply(elapsedSeconds, getCtx());
        }
    }

}

bool TimeLineExecutor::IsParent(TimeLineObject* motion, TimeLineObject* parent)
{
    bool result = false;

    if (motion !=NULL && motion->getParent() != NULL)
    {
        if (motion->getParent() == parent)
            result = true;
        else
            result = IsParent(motion->getParent(), parent);
    }
    
    return result;
}

void TimeLineExecutor::Dump()
{
    TimeLineContext* tlCtx = getCtx();

    string desc;

    if (tlCtx != NULL)
    {
        desc = tlCtx->getAvatar()->toString(); 
    }
    else
    {
        desc = "timeLineContex==NULL"; 
    }

    cout<< " TimeLineExecutor.Dump for " << desc << " state: " << _GET_STATE_NAME(getState()) << endl;
}

void TimeLineExecutor::RemoveExecutedMotions()
{
    TimeLineObject* firstMotion = getTimeLine()->getFirstObject();

    if (firstMotion != NULL)
    {
        TimeLineObject* prevMotionParent = GetTopParentInHierarchy(m_prevMotion.motion);
        TimeLineObject* currMotionParent = GetTopParentInHierarchy(m_currMotion.motion);
        TimeLineObject* nextMotionParent = GetTopParentInHierarchy(m_nextMotion.motion);

        //Start removing
        TimeLineObject* obj = firstMotion;
        
        while (obj != prevMotionParent && obj != currMotionParent && obj != nextMotionParent)
        {
            if (LOCAL_DEBUG) cout << "RemoveExecutedMotions: removal of " << obj->toString() << endl;
            TimeLineObject* obj_to_delete = obj;
            obj = obj->getNextObject();
            getTimeLine()->RemoveSubObject(obj_to_delete);
        }
    }
}

void TimeLineExecutor::RemoveUnexecutedMotions()
{
    if (m_currMotion.motion != NULL)
    {
        TimeLineObject* obj = m_currMotion.motion->getNextObject();
        while (obj != NULL)
        {
            if (LOCAL_DEBUG) cout << "RemoveMotionsAfterCurrent: removal of " << obj->toString() << endl;
            TimeLineObject* obj_to_delete = obj;
            obj = obj->getNextObject();
            obj_to_delete->getParent()->RemoveSubObject(obj_to_delete);  //TODO: consider also different levels of motions
        }
    }
}


bool TimeLineExecutor::CheckEpmtyFrame()
{
    CalMixer* mixer = getCtx()->getAvatar()->GetCalModel()->getMixer();
    return (mixer->getAnimationActionList().size() == 0 && mixer->getAnimationCycle().size() == 0) ;
}


/**
 * \brief Returns string representation of statesw2
 *
 * \param int state - identifier of message
 * \return std::string - string representation of message
 **/
std::string TimeLineExecutor::_GET_STATE_NAME(int state)
{
    std::string strState;

    switch(state)
    {
    case EXEC_STATE_NOT_INITED: strState= "EXEC_STATE_NOT_INITED"; break;
    case EXEC_STATE_WAIT: strState = "EXEC_STATE_WAIT"; break;
    case EXEC_STATE_SINGLE: strState = "EXEC_STATE_SINGLE"; break;
    case EXEC_STATE_OVERLAP: strState = "EXEC_STATE_OVERLAP"; break;
    case EXEC_STATE_FADE_IN: strState = "EXEC_STATE_FADE_IN"; break;
    case EXEC_STATE_FADE_OUT: strState = "EXEC_STATE_FADE_OUT"; break;
    case EXEC_STATE_TERMINATED: strState = "EXEC_STATE_TERMINATED"; break;
    default: strState = "<unknown>"; break;
    }
    
    return strState;
}

