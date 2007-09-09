/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
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
	currRotatation *= timeLineContext->getAvatar()->m_vRotation;
    bone->setRotation(currRotatation);

        
    //SET TRANSLATION //////////////////////
    CalVector currPos;
    int currAnimID_from_context = timeLineContext->getCurrAnimID();
//    float currAnimTime = timeLineContext->getCurrAnimTime();

    if (!m_translationInited)
    {
        counter = 0;
        currAnimID = -1;
        currAnimTime = 0;
    }

    float currAnimDuration = -1;

    if (currAnimID == -1 && currAnimID_from_context != -1)
    {
        currAnimID = currAnimID_from_context;
        currAnimTime = 0;
    }

    CalCoreAnimation *pCoreAnimation;

    if (currAnimID != -1)
    {
        pCoreAnimation = timeLineContext->getAvatar()->GetCalCoreModel()->getCoreAnimation(currAnimID);
        currAnimDuration = pCoreAnimation->getDuration();
    }

    currAnimTime += elapsedSeconds;

    bool wasAnimChange = false;

    if ( currAnimDuration!=-1 &&  currAnimDuration<currAnimTime  )
    {
        if (LOCAL_DEBUG)
        {
            cout << "--------NEW ANIM -------- loop: " <<  timeLineContext->isCurrAnimLoop()
                << " anim duration " << currAnimDuration << " animTime " << currAnimTime << endl;

        }

        std::string avName = timeLineContext->getAvatar()->getName();

        currAnimID = currAnimID_from_context;
        if (timeLineContext->isCurrAnimLoop())
        {
            if (LOCAL_DEBUG)
                cout << "- cycle size " << timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationCycle().size();

            if (timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationCycle().size() > 0)
            {
                //currAnimTime = timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationCycle().front()->getTime();
                currAnimTime = timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationTime();
                if (LOCAL_DEBUG)
                    cout << "- currAnimTime " << currAnimTime << endl;
            }
            else
                currAnimTime = 0;
        }
        else
        {
            if (LOCAL_DEBUG)
                cout << "- action list size " << timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationActionList().size();

            if (timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationActionList().size() > 0)
                currAnimTime = timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationActionList().front()->getTime();
            else
                currAnimTime = 0;
        }
        wasAnimChange = true;
        if (LOCAL_DEBUG)
            cout << " new anim time " << currAnimTime << endl;
    }


    if (currAnimID != -1 && currAnimTime != -1)
    {
        pCoreAnimation = timeLineContext->getAvatar()->GetCalCoreModel()->getCoreAnimation(currAnimID);
        currAnimDuration = pCoreAnimation->getDuration();
        CalCoreTrack* track = pCoreAnimation->getCoreTrack(0);
        CalVector translation;
        CalQuaternion rotation;
        track->getState(currAnimTime, currPos, rotation);
        if (wasAnimChange)
            m_vLastPos = currPos;

        if (LOCAL_DEBUG)
        {
            if (counter == 0)
                cout << "anim_id: " << currAnimID << " trans: my x "<< currPos.x << " z " << currPos.z << " cal x " << bone->getTranslation().x 
                            << " z " << bone->getTranslation().z << 
                            " anims " <<  timeLineContext->getAvatar()->GetCalModel()->getMixer()->getAnimationActionList().size() << endl;
            counter = (counter+1)%100;
        }
    }
    else
    {
        if (LOCAL_DEBUG)
            cout << "********** original trans " << endl;
        currPos = bone->getTranslation();
    }
    
        //std::cout << " z : " << currPos.z << std::endl;

    if (!m_translationInited)
    {
                    if (TRACE)
                    {
                        tracer_translation->ClearTrace();
                        tracer_curr_pos->ClearTrace();
                    }

        m_vTranslation = timeLineContext->getAvatar()->getStartPosition(); // - currPos;
        
        m_vTranslation.y = currPos.y;
        m_vLastPos = currPos;

        m_translationInited = true;

                    if (TRACE)
                    {
                        tracer_translation->AddPoint(m_vTranslation + CalVector(0,70,0));
                        tracer_translation->AddPoint(m_vTranslation + CalVector(0,80,0));

                        tracer_curr_pos->AddPoint(currPos + CalVector(0,70,0));
                        tracer_curr_pos->AddPoint(currPos + CalVector(0,80,0));
                    }
    }

    CalVector diff = currPos - m_vLastPos;

    diff *= timeLineContext->getAvatar()->m_vRotation;

    float diff_lenght = diff.length();

    if ( diff_lenght < 50)
    {
        m_vTranslation += diff;
        m_vTranslation.y = currPos.y;   //TODO:abak:  it should be applied also for diff_length>=50

                        if (TRACE)
                        {
                            tracer_translation->AddPoint(m_vTranslation);
                            tracer_curr_pos->AddPoint(currPos + CalVector(0,50,0));
                        }
        
    }


    m_vLastPos = currPos;
    bone->setTranslation(m_vTranslation);

    bone->calculateState();
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
    //m_vRotation = CalQuaternion();
}

