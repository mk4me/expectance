/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */
#include "lcsmodifier.h"
//#include "cal3d/coreanimation.h"
#include "cal3d/coretrack.h"

using namespace ft;

/// \brief constructor
LCSModifier::LCSModifier()
{
    m_vTranslation = CalVector(0,0,0);
	m_vLastPos = CalVector(0,0,0);
    m_translationInited = false;
    //m_vRotation = CalQuaternion();

    TRACE = false;
    LOCAL_DEBUG = false;
    INTERPOLATION = true;
    REST_TRANS_CALC = true;

    if (TRACE)
    {
        tracer_translation = new TraceLine(toString() + "Translation");
        tracer_curr_pos = new TraceLine(toString() + "AnimPosition");
        VisualizationManager::getInstance()->AddObject(tracer_translation);
        VisualizationManager::getInstance()->AddObject(tracer_curr_pos);
    }	
}

/// \brief destructor
LCSModifier::~LCSModifier(void)
{
    if (tracer_translation != NULL)
    {
        tracer_translation->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(tracer_translation);
    }
    if (tracer_curr_pos != NULL)
    {
        tracer_curr_pos->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(tracer_curr_pos);
    }

}

/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void LCSModifier::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    TimeLineModifier::Apply(elapsedSeconds, timeLineContext);

    // Here apply this modifier
    CalSkeleton *skel = timeLineContext->getAvatar()->GetCalModel()->getSkeleton();
    CalBone *bone = skel->getBone(0);

    //SET ROTATION ////////////////////
    CalQuaternion currRotatation = bone->getRotation();
	currRotatation *= timeLineContext->getAvatar()->getOrientation();
	//currRotatation *= timeLineContext->getAvatar()->m_stepOrientation;
    bone->setRotation(currRotatation);

        
    //SET TRANSLATION //////////////////////
    CalVector currPos;
    CalVector restTrans(0,0,0); // used in case of changing animation or new cycle in loop anim
    CalVector vPrevPos(0,0,0); //used only for OVERLAP state

    if (!m_translationInited)
    {
        m_vTranslation = timeLineContext->getAvatar()->getStartPosition(); 
        m_translationInited = true;
    }

    if (timeLineContext->currAnim != NULL)
    {

        CalVector translation;
        CalQuaternion rotation;
        CalCoreAnimation* coreAnim = timeLineContext->currAnim->getCoreAnimation();
        CalCoreTrack* track = coreAnim->getCoreTrack(0);
        track->getState(timeLineContext->currAnimTime, currPos, rotation);

        float animTime = timeLineContext->currAnimTime;

        if (timeLineContext->anim_changed)
        {
            if (LOCAL_DEBUG) cout << " Anim changed .... anim time " << animTime  << endl;

            if (timeLineContext->prevAnim != NULL)
            {
                if (LOCAL_DEBUG) cout << " Prev != NULL " << endl;
                //calculate rest time
                timeLineContext->prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->prevAnimTime, restTrans, rotation);
                restTrans -= m_vLastPos;

                if (LOCAL_DEBUG) cout << " resTrans " << restTrans.length() << endl;

            }
            else
            {
                //TODO:  implement calculation of restTrans for case without OVERLAP
            }
         }
         else if (timeLineContext->anim_new_cycle)
         {
            float animEndTime = timeLineContext->currAnimDuration; //TODO: chceck if its correct
            //calculate rest time for new cycle
            track->getState(animEndTime, restTrans, rotation);
            restTrans -= m_vLastPos;
            
            CalVector vStartOffset = currPos;
            CalVector startAnimPos;
            track->getState(0, startAnimPos, rotation);
            vStartOffset -= startAnimPos;
         
            restTrans += vStartOffset;

            //if (LOCAL_DEBUG) cout << " resTrans " << restTrans.length() << endl;
         }

        if (INTERPOLATION && timeLineContext->exec_state == EXEC_STATE_OVERLAP)
            //&& timeLineContext->prevAnimTime <= timeLineContext->prevAnimDuration)
        {
            //cout << " Anim overlaped .... " << endl;
            if (timeLineContext->prevAnim != NULL)
            {
                timeLineContext->prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->prevAnimTime, vPrevPos , rotation);
                if (timeLineContext->anim_changed)
                    m_vLastPrevPos = vPrevPos;
            }
            else
            {
                cout << " ERR: LCSModifier::Apply(): no prev anim for OVERLAP !!! " << endl;
            }
        }

        if (timeLineContext->anim_changed || timeLineContext->anim_new_cycle)
        {
            m_vLastPos = currPos;
        }
    }
    else
    {
        if (LOCAL_DEBUG) cout << " Original pos " << endl;
        currPos = bone->getTranslation();
    }

    CalVector diff = currPos - m_vLastPos;

    float y_to_set = currPos.y;

    if (INTERPOLATION && timeLineContext->exec_state == EXEC_STATE_OVERLAP)
            //&& timeLineContext->prevAnimTime <= timeLineContext->prevAnimDuration)
    {
        CalVector prevDiff = vPrevPos - m_vLastPrevPos;
        float factor =  timeLineContext->currAnimTime/timeLineContext->prevOverlap; 
        diff = (1.0f - factor) * prevDiff + factor * diff;
        
        y_to_set = (1.0f - factor) * vPrevPos.y + factor * currPos.y;

        m_vLastPrevPos = vPrevPos;

            if (LOCAL_DEBUG) 
            {
                if (counter == 0 || timeLineContext->anim_changed)
                {
                    //if (timeLineContext->anim_changed)
                    //{
                    //    cout << " prev time " << timeLineContext->prevAnimTime << " prev dur " << timeLineContext->prevAnimDuration << endl;
                    //}
                    //cout << " animTime " << timeLineContext->currAnimTime << " dur " << timeLineContext->prevOverlap 
                    //    << " factor " << factor << " diff " << diff.length() <<endl;
                    //cout << " prevAnimTime " << timeLineContext->prevAnimTime << " prev dur " << timeLineContext->prevAnimDuration << endl;
                }
                counter = (counter+1)%5;
            }
    }

    if (REST_TRANS_CALC)
       diff += restTrans; 

    diff *= timeLineContext->getAvatar()->getOrientation();
    //diff *= timeLineContext->getAvatar()->m_stepOrientation;

    
    m_vTranslation += diff;
    m_vTranslation.y = y_to_set;   


                        if (TRACE)
                        {
                            tracer_translation->AddPoint(m_vTranslation);
                            tracer_curr_pos->AddPoint(currPos + CalVector(0,50,0));
                        }


    m_vLastPos = currPos;

    bone->setTranslation(m_vTranslation);
    bone->calculateState();
	
	//set current parameters lcs to scene object
	timeLineContext->getAvatar()->setPosition(m_vTranslation);
	//!timeLineContext->getAvatar()->setOrientation(currRotatation);

}


/// \brief Resets parameters of this modifier
void LCSModifier::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);

    m_translationInited = false;

    if (TRACE)
    {
        tracer_translation->ClearTrace();
        tracer_curr_pos->ClearTrace();
    }
     m_vTranslation = CalVector(0,0,0);
     m_vLastPos = CalVector(0,0,0);
     m_vLastPrevPos = CalVector(0,0,0);
    //m_vRotation = CalQuaternion();
}

