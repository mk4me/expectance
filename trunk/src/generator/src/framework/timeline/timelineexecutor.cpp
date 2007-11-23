/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "timelineexecutor.h"

using namespace ft;
using namespace std;

/**
 * \brief Helper fuction which clears parameters of timeline_exec_item_t struct
 *
 * \param timeline_exec_item_t &item - struct whose paramters are to clear
 **/
void clearExecItem(timeline_exec_item_t &item)
{
    item.motion = NULL;
    item.anim = NULL;
    item.animTime = 0;
    item.animDuration = 0;
    item.coreAnimDuration = 0;
    item.lastStep= false;
    item.loopStep= 0;
    item.lastTime = 0;
}

/**
 * \brief Helper fuction which copies parameters of one timeline_exec_item_t struct to another
 *
 * \param timeline_exec_item_t &src - struct from which parameters will be copied
 * \param timeline_exec_item_t &dst - struct to which parameters of src will be copied
 **/
void copyExecItem(timeline_exec_item_t &src, timeline_exec_item_t &dst)
{
    dst.motion = src.motion;
    dst.anim = src.anim;
    dst.animTime = src.animTime;
    dst.animDuration = src.animDuration;
    dst.coreAnimDuration = src.coreAnimDuration;
    dst.lastStep = src.lastStep;
    dst.loopStep = src.loopStep;
    dst.lastTime = src.lastTime;
}



/// \brief constructor
TimeLineExecutor::TimeLineExecutor(void)
{
    LOCAL_DEBUG = true;
    CHECK_EMPTY_FRAMES = false;
    setState(EXEC_STATE_NOT_INITED);
    Reset();
}

/// \brief Releases all resources and objects related to this TimeLineExecutor
void TimeLineExecutor::Destroy(void)
{
}

/// \brief Resets all parameters of TimeLineExecutor
void TimeLineExecutor::Reset()
{
    clearExecItem(m_prevMotion);
    clearExecItem(m_currMotion);
    clearExecItem(m_nextMotion);

    m_prevBlender = 0;
    m_currBlender = 0;
    m_nextBlender = 0;

    m_lastEvent = EXEC_EVENT_NONE;
}

/**
 * \brief Inititiates parameters of this TimeLineExecutor
 *
 * \param TimeLine* timeLine - timeLine which is to execute by this TimeLineExecutor
 * \param TimeLineContext* timeLineContext - contex that provides parametrs of timeLine execution
 **/
void TimeLineExecutor::Initiate(TimeLine* timeLine, TimeLineContext* timeLineContext)
{
    setTimeLine(timeLine);
    setCtx(timeLineContext);
    ChangeState(EXEC_STATE_WAIT);
}

/**
 * \brief Sets new state of executor
 *
 * \param int state - new state
 **/
void TimeLineExecutor::setState(int state)
{ 
    m_state = state; 
}

/**
 * \brief Returns current state of executor
 *
 * \return int - current state of executor
 **/
int TimeLineExecutor::getState() 
{ 
    return m_state; 
}

/**
 * \brief Starts execution of timeLine
 **/
void TimeLineExecutor::Start()
{
    setTerminated(false);
}

/**
 * \brief Requests stop of timeLine
 **/
void TimeLineExecutor::StopRequest()
{
    setTerminated(true);
}

//////////////////// STATES ////////////////////

/**
 * \brief Change state to anothor
 *
 * \param int newState - new state
 **/
void TimeLineExecutor::ChangeState(int newState)
{
    int oldState = getState();

    if (Debug::TIMELINE>0)
        _dbg  << " TimeLineExecutor::ChangeState to  "<< _GET_STATE_NAME(newState) << "  (old:"
                << _GET_STATE_NAME(oldState) << ")" << endl;

    
    setState(newState);
    EntryState(newState, oldState);
    m_changedStateMarker = true;
}

/**
 * \brief Method called when executor enterns into new state
 *
 * \param int state - new state
 * \param int oldState - previous state
 **/
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
    case EXEC_STATE_TERMINATED: EntryTerminatedState(oldState); break;
    default: break;
    }
}

/**
 * \brief Method called at each update from ft::UpdateManager during timeLine execution
 *
 * \param int state - curent state of executor
 **/
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
    case EXEC_STATE_TERMINATED: UpdateTerminatedState(); break;
    default: break;
    }
}

/**
 * \brief Method called when executor enters to EXEC_STATE_WAIT state
 *
 * \param int oldState - previous state
 **/
void TimeLineExecutor::EntryWaitState(int oldState)
{
    Reset();
}

/**
 * \brief Method called when executor enters to EXEC_STATE_SINGLE state
 *
 * \param int oldState - previous state
 **/
void TimeLineExecutor::EntrySingleState(int oldState)
{
    if (oldState != EXEC_STATE_FADE_IN && oldState != EXEC_STATE_OVERLAP)
    {
        StartNextMotion();
    }
}

/**
 * \brief Method called when executor enters to EXEC_STATE_OVERLAP state
 *
 * \param int oldState - previous state
 **/
void TimeLineExecutor::EntryOverlapState(int oldState)
{
    StopLoopAnim();
    StartNextMotion();
}

/**
 * \brief Method called when executor enters to EXEC_STATE_FADE_IN state
 *
 * \param int oldState - previous state
 **/
void TimeLineExecutor::EntryFadeInState(int oldState)
{
    StartNextMotion();
}

/**
 * \brief Method called when executor enters to EXEC_STATE_FADE_OUT state
 *
 * \param int oldState - previous state
 **/
void TimeLineExecutor::EntryFadeOutState(int oldState)
{
   if (m_currMotion.motion->isAnimLoop())
   {
       StopLoopAnim();
   }
   else
   {
       //action should finish automatically
   }
    
}

/**
 * \brief Method called when executor enters to EXEC_STATE_TERMINATED state
 *
 * \param int oldState - previous state
 **/
void TimeLineExecutor::EntryTerminatedState(int oldState)
{
    if (m_currMotion.motion !=NULL && m_currMotion.anim != NULL)
    {
        if (m_currMotion.motion->isAnimLoop())
            StopLoopAnim();
        else
            StopActionAnim(); //NOTE: there is no fade_out regerded for action at this case (not possible unexpected fade_out for action in Cla3d)
    }
    Reset();
}

/**
 * \brief Method called at each update from ft::UpdateManager when excutor is in EXEC_STATE_WAIT state
 **/
void TimeLineExecutor::UpdateWaitState()
{
    //return;  //uncomment it only for test without animation
    if (!isTerminated())
    {
        if (!getTimeLine()->isEmpty())
        {
            if (m_currBlender > 0)
                ChangeState(EXEC_STATE_FADE_IN);  //NOTE(abak): no possible case in practice - no blender defined in WAIT state
            else
                ChangeState(EXEC_STATE_SINGLE);
        }
    }
    else
    {
        ChangeState(EXEC_STATE_TERMINATED);
    }
}

/**
 * \brief Method called at each update from ft::UpdateManager when excutor is in EXEC_STATE_SINGLE state
 **/
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
                        ChangeState(EXEC_STATE_FADE_IN);  // NOTE(abak): is it resonable?
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

/**
 * \brief Method called at each update from ft::UpdateManager when excutor is in EXEC_STATE_OVERLAP state
 **/
void TimeLineExecutor::UpdateOverlapState()
{
    if (m_prevMotion.motion != NULL)
    {
//        _dbg << " UpdateOverlapState m_prevMotion.anim " << m_prevMotion.anim << endl;
        if (m_prevMotion.anim == NULL)
        {
            ChangeState(EXEC_STATE_SINGLE);
        }
    }
}

/**
 * \brief Method called at each update from ft::UpdateManager when excutor is in EXEC_STATE_FADE_IN state
 **/
void TimeLineExecutor::UpdateFadeInState()
{
    if (m_currMotion.motion != NULL)
    {
        if (m_currMotion.anim != NULL)
        {
            float animLeftTime = m_currMotion.animDuration - m_currMotion.animTime;
            if (m_prevBlender > 0 && m_prevBlender >= animLeftTime)
            {
                ChangeState(EXEC_STATE_SINGLE);
            }
        }
        else
        {
            if (Debug::ERR)
                _dbg << Debug::ERR_STR <<"TimeLineExecutor::UpdateFadeInState():  m_currMotion.anim == NULL during FADE_IN state !!!" << endl;
        }
    }

}

/**
 * \brief Method called at each update from ft::UpdateManager when excutor is in EXEC_STATE_FADE_OUT state
 **/
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
        if (Debug::ERR)
            _dbg << Debug::ERR_STR << "TimeLineExecutor::UpdateFadeOutState() " << " null motion for " 
                            << _GET_STATE_NAME(getState()) << " !!! " << endl;
    }
}

/**
 * \brief Method called at each update from ft::UpdateManager when excutor is in EXEC_STATE_TERMINATED state
 **/
void TimeLineExecutor::UpdateTerminatedState()
{
    if (!isTerminated())
    {
        ChangeState(EXEC_STATE_WAIT);
    }
}

/**
 * \brief Starts next motion on the timeLine
 **/
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
        
        //check if there is loop_number==0 for loop animation  (correction for bug #021)
        //if loop_number==0 the effect will be as if loop_number was 1 (however warning will be thrown)
        
        if (m_nextMotion.motion->isAnimLoop() && (m_nextMotion.motion->getLoopNumber() == 0))
        {
            if (Debug::WARN)
                _dbg << Debug::WARN << "TimeLineExecutor::StartNextMotion() :  loop_number for loop animation is 0." 
                    << " To avoid inconsistency and potential problems the loop number will be set to 1!!!! " << endl;
        }
        

        if (!m_nextMotion.motion->getMotion()->isNullAnim())
        {
            if (m_nextMotion.motion->isAnimLoop())
            {
                if (Debug::TIMELINE>1)
                    _dbg << " StartAnim-blendCycle "  << m_nextMotion.motion->toString() << " with fade_in " << fade_in << std::endl;

                getCtx()->getAvatar()->GetCalModel()->getMixer()->blendCycle(m_nextMotion.motion->getMotion()->getAnimID(),
                        1.0f, fade_in);

                m_nextMotion.anim = FindAddedAnimInCal3d(CalAnimation::TYPE_CYCLE);
                if (m_nextMotion.anim == NULL)
                {
                    if (Debug::ERR)
                        _dbg << Debug::ERR_STR << "TimeLineExecutor::StartNextMotion - no cycle anim extracted from Cal3d after adding motion " << endl;
                }
                else
                {
                    m_animChanged = true;

                    if (Debug::TIMELINE>1)
                        _dbg << "New loop animation " << m_nextMotion.anim->getCoreAnimation()->getFilename() << " length: " 
                            << m_nextMotion.anim->getCoreAnimation()->getDuration() << endl;
                }

            }
            else
            {
                if (Debug::TIMELINE>1)
                    _dbg << " StartAnim-executeAction with  " << m_nextMotion.motion->toString() << " fade_in " << fade_in << " fade_out " << fade_out << std::endl;

                getCtx()->getAvatar()->GetCalModel()->getMixer()->executeAction(m_nextMotion.motion->getMotion()->getAnimID(),
                        fade_in, fade_out);

                m_nextMotion.anim = FindAddedAnimInCal3d(CalAnimation::TYPE_ACTION);
                if (m_nextMotion.anim == NULL)
                {
                    if (Debug::ERR)
                        _dbg << Debug::ERR_STR << "TimeLineExecutor::StartNextMotion - no action anim extracted from Cal3d after adding motion " << endl;
                }
                else
                {
                    m_animChanged = true;

                    if (Debug::TIMELINE>1)
                        _dbg << "New action animation " << m_nextMotion.anim->getCoreAnimation()->getFilename() << " length: " 
                            << m_nextMotion.anim->getCoreAnimation()->getDuration() << endl;

                }
            }
        }
        m_prevBlender = m_currBlender;
    }
    ExchangeExecItems();
}

/**
 * \brief Extracts last animation object added to Cal3d
 * \param int animType - action or cycle
 * \return CalAnimation* - animation object extracted from Cal3d or NULL if extraction failed
 **/
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

/**
 * \brief Stops currently played loop anim with fade_out defined by current blender
 **/
void TimeLineExecutor::StopLoopAnim()
{
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL 
                && m_currMotion.motion->getMotion() != NULL && m_currMotion.motion->isAnimLoop())
    {
        float fade_out = 0;
        if (m_currBlender > 0)
            fade_out = m_currBlender;
    
        if (Debug::TIMELINE>1)
            _dbg << " StopLoopAnim-clearCycle " << m_currMotion.motion->toString() << " with fade out " << fade_out << std::endl;

        getCtx()->getAvatar()->GetCalModel()->getMixer()->clearCycle(m_currMotion.motion->getMotion()->getAnimID(), fade_out);
    }
}

/**
 * \brief Stops currently played action anim
 **/
void TimeLineExecutor::StopActionAnim()
{
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL 
                && m_currMotion.motion->getMotion() != NULL && !m_currMotion.motion->isAnimLoop())
    {
        float fade_out = 0;
        if (m_currBlender > 0)
            fade_out = m_currBlender;
    
        if (Debug::TIMELINE>1)
            _dbg << " StopActionAnim-removeAction " << m_currMotion.motion->toString() << " [ no fade out used ]" << endl;

        getCtx()->getAvatar()->GetCalModel()->getMixer()->removeAction(m_currMotion.motion->getMotion()->getAnimID());
    }
}

/**
 * \brief Exchenges paramters of prevMotion, currMotion and nextMotion of executor
 **/
void TimeLineExecutor::ExchangeExecItems()
{
    copyExecItem(m_currMotion, m_prevMotion);
    copyExecItem(m_nextMotion, m_currMotion);
    clearExecItem(m_nextMotion);
}

//////////////////////////// end of STATES ////////////////////////

/**
 * \brief Method called at each update from ft::UpdateManager during execution of timeLine
 * \param const double elapsedSeconds - time elapsed sonce last update
 **/
void TimeLineExecutor::UpdateMotions(const double elapsedSeconds)
{

    if (CHECK_EMPTY_FRAMES && !isTerminated() && CheckEpmtyFrame())  //check if there is no any animation in mixer
    {
        if (Debug::WARN)
            _dbg << Debug::WARN_STR << "empty frame !!!! " << endl;
    }

    m_lastEvent = EXEC_EVENT_NONE;
    m_animChanged = false;
    m_animNewCycle = false;
    m_animStopped = false;

    //test of anim stopped
    bool currAnimStartedLastTime = false;
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL)
    {
        currAnimStartedLastTime = true;
    }


    UpdateExecItem(m_prevMotion);
    UpdateExecItem(m_currMotion);


    IdentifyNextMotion();
    UpdateExecItem(m_nextMotion);

    IdentifyBlenders();

    LimitCurrBlenderForNextAnim();
    LimitCurrBlenderForCurrAnim();

    CheckInterrupting();

    m_changedStateMarker = false;
    int previousState = getState();

    UpdateState(getState());

    if (m_changedStateMarker)
    {
        m_lastEvent = EXEC_EVENT_STATE_CHANGED;
    }

    //check if anim stopped at this update
    if (currAnimStartedLastTime && ((m_currMotion.motion == NULL || m_currMotion.anim == NULL)))
    {
        m_animStopped = true;
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

/**
 * \brief Updates timeLineContext reeted to this executor. It is call at each update from ft::UpdateManager
 **/
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
    getCtx()->anim_stopped = m_animStopped;
    getCtx()->anim_new_cycle = m_animNewCycle;

}

/**
 * \brief Updates parameters of timeline_exec_item_t struct according to runtime state of Cal3d
 * \param timeline_exec_item_t &item - struct to update
 **/
void TimeLineExecutor::UpdateExecItem(timeline_exec_item_t &item)
{
    if (item.motion != NULL)
    {
        //get params from core animation
        if (item.motion->getMotion() != NULL)
        {
            CalCoreAnimation* coreAnim = getCtx()->getAvatar()->GetCalCoreModel()->getCoreAnimation(item.motion->getMotion()->getAnimID());
            if (coreAnim != NULL)
            {
                item.coreAnimDuration = coreAnim->getDuration();
            }
        }

        //get runtime param-s of animation
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
//                    _dbg << " animTime: " << item.animTime << " dur " << item.animDuration << endl;
                }
            }
            else
            {
                MarkAnimStopped(item);
            }
        }
    }
}

/**
 * \brief Check if currently played loop anim should be interrupted
 **/
void TimeLineExecutor::CheckInterrupting()
{
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL && m_currMotion.motion->isAnimLoop())
    {
        if (m_nextMotion.motion!= NULL && m_nextMotion.motion->isInterupting())
            m_currMotion.lastStep = true;
    }
}

/**
 * \brief Sets parmeters of timeline_exec_item_t struct to values proper for stopped animation
 * \param timeline_exec_item_t &item - struct to fill with proper parameters
 **/
void TimeLineExecutor::MarkAnimStopped(timeline_exec_item_t &item)
{
    item.anim = NULL;
    item.anim = 0;
    item.animDuration = 0;
}

/**
 * \brief Indetifies next motion on currently being executed timeLine
 **/
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

/**
 * \brief Checks if given timeline_exec_item_t structs defines loop motion which should be stopped
 * \param timeline_exec_item_t &item - struct to check
 **/
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

/**
 * \brief Checks if given animation is still being played in Cal3d
 * \param CalAnimation* anim - animation object to check
 * \return bool - true if animation is still being played, false otherwise
 **/
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

/**
 * \brief Gets submotion of motion which has defined animation 
 * \param TimeLineMotion* motion - motion whose subobjects will be checked
 * \return TimeLineMotion* - submotion with anim or NULL if there is no such submotion
 **/
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

/**
 * \brief Gets parent motion (searching all higher motions in hierarchy) which contains next motion on the same level
 * \param TimeLineMotion* motion - motion whose parents will be checked
 * \return TimeLineMotion* - parent motion which has next motion on the same level
 **/
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

/**
 * \brief Gets top parent motion (direct child of timeLine) for given motion
 * \param TimeLineMotion* motion - motion whose top parent will be searched
 * \return TimeLineMotion* - top parent of given
 **/
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

/**
 * \brief Gets parent motion (searching all higher motions in hierarchy) which has defined blender
 * \param TimeLineMotion* motion - motion whose parents will be checked
 * \return TimeLineMotion* - parent motion which has defined blender
 **/
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

/**
 * \brief Identifies blender for current motion and blender for next motion
 **/
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

/**
 * \brief Checks if current blender is not longer then next animation. If so it cut current blemder 
 *         to be shorter from next motion about some margin
 **/
void TimeLineExecutor::LimitCurrBlenderForNextAnim()
{
    if (m_nextMotion.motion != NULL && m_currBlender > 0)
    {
        float nextAnimCoreDuration = m_nextMotion.coreAnimDuration;
        float fMARGIN = 0.3f; // to ensure that animDuration will be higher than fade_in

        float oldBlenderVal = m_currBlender; //only for DEBUG

        if (nextAnimCoreDuration < (m_currBlender + fMARGIN))
        {
            m_currBlender = nextAnimCoreDuration -  fMARGIN;
            if (m_currBlender < 0)
                m_currBlender = 0;
            //_dbg << " Blender " << oldBlenderVal << " linimted to " << m_currBlender << " because of too short next anim -> " << nextAnimCoreDuration << endl;
        }
    }
}

/**
 * \brief Checks if current blender is not longer then time left to the end of current animation. If so it cut current blender 
 *         to be the same as left time
 **/
void TimeLineExecutor::LimitCurrBlenderForCurrAnim()
{
    if (m_currMotion.motion != NULL && m_currMotion.anim != NULL && m_currBlender > 0)
    {
        float animLeftTime = m_currMotion.animDuration - m_currMotion.animTime;
        if (m_currBlender > animLeftTime ) //&& animLeftTime > 0.1f)
        {
            //_dbg << " Blender " << m_currBlender << " linimted to " << animLeftTime << " because of anim left time " << endl;
            m_currBlender = animLeftTime;
        }
    }
}

/**
 * \brief Updates modifiers for current and previous motions (an their parent motions)
 * \param const double elapsedSeconds - time elapsed since last update
 **/
void TimeLineExecutor::UpdateModifiers(const double elapsedSeconds)
{
    // apply modifiers for current and prev motion 

    if (getState() != EXEC_STATE_WAIT && getState() != EXEC_STATE_TERMINATED)
    {
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
    else
    {
        UpdateModifiersForMotion(getTimeLine(), elapsedSeconds);
    }

}

/**
 * \brief Updates modifiers for given motion
 * \param TimeLineMotion* motion - motion whose modifiers will be updated
 * \param const double elapsedSeconds - time elapsed since last update
 **/
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

/**
 * \brief Checks if given motion is a parent of another motion (regarding all hierarchy)
 * \param TimeLineMotion* motion - motion for which a parent is checked
  * \param TimeLineMotion* parent - motion which will be checked to be a parent
 **/
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

/**
 * \brief Dumps significant information about this executor and its current state
 **/
void TimeLineExecutor::Dump()
{
    if (Debug::TIMELINE>0)
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

        _dbg<< " TimeLineExecutor.Dump for " << desc << " state: " << _GET_STATE_NAME(getState()) << endl;

        string currMotionStr;
        if (m_currMotion.motion != NULL)
        {
            if (m_currMotion.motion->getMotion() != NULL)
                currMotionStr = m_currMotion.motion->getMotion()->getAnimName();
            else
                currMotionStr = m_currMotion.motion->getID();
        }
        else
            currMotionStr = "NULL";

        _dbg<< " TimeLineExecutor.Dump, curr motion: " << currMotionStr << endl;
    }
}

/**
 * \brief Removes motions from timeLine that were already executed
 **/
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
            if (Debug::TIMELINE>0)
                _dbg << "RemoveExecutedMotions: removal of " << obj->toString() << endl;

            TimeLineObject* obj_to_delete = obj;
            obj = obj->getNextObject();
            getTimeLine()->RemoveSubObject(obj_to_delete);
        }
    }
}

/**
 * \brief Removes motions that were not executed yet
 **/
void TimeLineExecutor::RemoveUnexecutedMotions()
{
    if (m_currMotion.motion != NULL)
    {
        TimeLineObject* obj = m_currMotion.motion->getNextObject();
        while (obj != NULL)
        {
            if (Debug::TIMELINE>0)
                _dbg << "RemoveMotionsAfterCurrent: removal of " << obj->toString() << endl;

            TimeLineObject* obj_to_delete = obj;
            obj = obj->getNextObject();
            obj_to_delete->getParent()->RemoveSubObject(obj_to_delete);  //TODO: consider also different levels of motions
        }
    }
}

/**
 * \brief Check if avatar to which this excutor is connected has not an empty animation list at current update
 **/
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
    case EXEC_STATE_NOT_INITED: strState= "NOT_INITED"; break;
    case EXEC_STATE_WAIT: strState = "WAIT"; break;
    case EXEC_STATE_SINGLE: strState = "SINGLE"; break;
    case EXEC_STATE_OVERLAP: strState = "OVERLAP"; break;
    case EXEC_STATE_FADE_IN: strState = "FADE_IN"; break;
    case EXEC_STATE_FADE_OUT: strState = "FADE_OUT"; break;
    case EXEC_STATE_TERMINATED: strState = "TERMINATED"; break;
    default: strState = "<unknown>"; break;
    }
    
    return strState;
}

