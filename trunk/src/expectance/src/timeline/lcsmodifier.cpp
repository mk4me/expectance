/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */
#include "lcsmodifier.h"
#include "cal3d/coretrack.h"
#include <math.h>
#include "../avatar/osgavatar.h"
#include "../utility/stringhelper.h"

#include "gil/gil_animexecution.h"

using namespace ft;

/// \brief constructor
LCSModifier::LCSModifier()
{
     m_vLastPos = CalVector(0,0,0);
     m_vLastPrevPos = CalVector(0,0,0);

     m_fLastAnimTime = 0;

	 m_tracer = new TraceLine();

     INTERPOLATION = true;
     REST_TRANS_CALC = true;
}

/// \brief destructor
LCSModifier::~LCSModifier(void)
{
}


/**
 * \brief Applies this modifier at current frame
 *
 * \param float elapsedSeconds - - time elapsed from previous frame
 * \param ft::TimeLineContext * timeLineContext - TimeLineContext of avatar to which this modifier is assigned
 **/
void LCSModifier::Apply(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    UpdateRotation(elapsedSeconds, timeLineContext);
    UpdateTranslation(elapsedSeconds, timeLineContext);
	
	OsgAvatar* avImpl = (OsgAvatar*)timeLineContext->getAvatarImpl();
	m_tracer->AddPoint(avImpl->getPosition());
}

////////////////// ROTATION ///////////////////////////////

void LCSModifier::UpdateRotation(float elapsedSeconds, TimeLineContext * timeLineContext)
{
	//TODO:  implement rotation calculation
	OsgAvatar* avImpl = (OsgAvatar*)timeLineContext->getAvatarImpl();
}

//////////////////// TRANSLATION ///////////////////////////////

void LCSModifier::UpdateTranslation(float elapsedSeconds, TimeLineContext * timeLineContext)
{
	OsgAvatar* avImpl = (OsgAvatar*)timeLineContext->getAvatarImpl();

    Transform* currTransform = NULL;
    Transform* prevTransform = NULL;
    if (timeLineContext->currAnim != NULL)
    {   
		CalAnimation* currAnim = ((Cal3dAnimExecution*)timeLineContext->currAnim)->getAnimation();
        currTransform = GetTransformForAnim(currAnim, avImpl);
    }
    else
    {
        //currTransform = GetTransformForType(avImpl);  TODO : implement this
    }

    CalBone *bone = avImpl->getOsgModel()->getCalModel()->getSkeleton()->getBone(0);
    
    CalVector currPos;
    CalQuaternion tmpRot;    
    CalVector vPrevPos;

    CalVector vCycleRest;
    CalVector vOverlapRest;
    
    if (timeLineContext->currAnim != NULL)
    {
		CalAnimation* currAnim = ((Cal3dAnimExecution*)timeLineContext->currAnim)->getAnimation();
        CalCoreAnimation* coreAnim = currAnim->getCoreAnimation();
        coreAnim->getCoreTrack(0)->getState(timeLineContext->currAnimTime, currPos, tmpRot);
        if (currTransform != NULL)
        {
			currPos += currTransform->getPosOffset();
        }
 
        if (INTERPOLATION && timeLineContext->exec_state == TimeLineContext::EXEC_STATE_OVERLAP)
        {
            if (timeLineContext->prevAnim != NULL)
            {
				CalAnimation* prevAnim = ((Cal3dAnimExecution*)timeLineContext->prevAnim)->getAnimation();
                prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->prevAnimTime, vPrevPos , tmpRot);
                prevTransform = GetTransformForAnim(prevAnim, avImpl);
                if (prevTransform != NULL)
                {
                    vPrevPos += prevTransform->getPosOffset();
                }
                if (timeLineContext->anim_changed)
                {
                    m_vLastPrevPos = vPrevPos;
                }
            }
            else
            {
//                if (GenDebug::ERR)
//                    _dbg << GenDebug::ERR_STR << "LCSModifier::UpdateTranslation(): no prev anim for OVERLAP !!! " << endl;
            }
        }

        if (timeLineContext->anim_new_cycle)
        {
            if (REST_TRANS_CALC )
            {
                coreAnim->getCoreTrack(0)->getState(timeLineContext->currAnimDuration, vCycleRest, tmpRot);
                CalVector vEnd;
                coreAnim->getCoreTrack(0)->getState(m_fLastAnimTime, vEnd, tmpRot);
                vCycleRest -= vEnd;

                CalVector vCurr;
                coreAnim->getCoreTrack(0)->getState(timeLineContext->currAnimTime, vCurr, tmpRot);
                CalVector vZero;
                coreAnim->getCoreTrack(0)->getState(0, vZero, tmpRot);
                vCurr -= vZero;
                vCycleRest += vCurr;
            }
        }

        // Rest calculation for anim_changed is calculated only for OVERLAP state
        if (timeLineContext->anim_changed && timeLineContext->exec_state == TimeLineContext::EXEC_STATE_OVERLAP)
        {
            if (REST_TRANS_CALC )
            {
				CalAnimation* prevAnim = ((Cal3dAnimExecution*)timeLineContext->prevAnim)->getAnimation();
                prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->prevAnimTime, vOverlapRest, tmpRot);
                CalVector vPrevious;
                prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(m_fLastAnimTime, vPrevious, tmpRot);
                vOverlapRest -= vPrevious;
            }
        }

        m_fLastAnimTime = timeLineContext->currAnimTime;
        
        if (timeLineContext->anim_changed || timeLineContext->anim_new_cycle)
        {
            m_vLastPos = currPos;
        }
    }
    else
    {
        //if (LOCAL_DEBUG) _dbg << " Original pos " << endl;
        currPos = bone->getTranslation();
        if (currTransform != NULL)
        {
            currPos += currTransform->getPosOffset();
        }
        m_vLastPos = currPos;
    }

    CalVector diff = currPos - m_vLastPos;

    if (REST_TRANS_CALC)
    {
        vCycleRest.z = 0;
        diff += vCycleRest;
    }



	osg::Quat globalRot = avImpl->getGlobalRotation();

	CalQuaternion qGlobalRotation =  QuatToCalQuat(avImpl->getGlobalRotation());//avImpl->getGlobalRotationOffset();
    if (currTransform)
    {
        qGlobalRotation *= currTransform->getForwardDiff();
    }

	CalQuaternion rot (qGlobalRotation);
	rot.invert(); // probably different coordinate system than on OSG 

	if (diff.length() > 0)
		diff *= rot;

    float z_to_set = currPos.z;

    if (INTERPOLATION && timeLineContext->exec_state == TimeLineContext::EXEC_STATE_OVERLAP)
            //&& timeLineContext->prevAnimTime <= timeLineContext->prevAnimDuration)
    {
        //position
        float factor =  timeLineContext->currAnimTime/timeLineContext->prevOverlap; 
        CalVector prevDiff = vPrevPos - m_vLastPrevPos;

        vOverlapRest.z = 0;
        prevDiff += vOverlapRest;

		
        CalQuaternion qGlobalRotation = QuatToCalQuat(avImpl->getGlobalRotation());
        if (prevTransform != NULL)
        {
            qGlobalRotation *= prevTransform->getForwardDiff();
        }

		CalQuaternion gloablRot (qGlobalRotation);
		gloablRot.invert();   // probably different coordinate system than on OSG 
        prevDiff *= gloablRot;

        diff = (1.0f - factor) * prevDiff + factor * diff;
        z_to_set = (1.0f - factor) * vPrevPos.z + factor * currPos.z;

        m_vLastPrevPos = vPrevPos;
    }


	osg::Vec3d pos = avImpl->getPosition();

    CalVector vCurrAvatarPosition(pos.x(), pos.y(), pos.z());
    vCurrAvatarPosition += diff;
    vCurrAvatarPosition.z = z_to_set;   

    m_vLastPos = currPos;

    //bone->setTranslation(vCurrAvatarPosition);  //abak: y must be similiar to y in skeleton (why?)
	//bone->calculateState();

	osg::Vec3d newPos = osg::Vec3d(vCurrAvatarPosition.x, vCurrAvatarPosition.y, vCurrAvatarPosition.z);
	avImpl->setPosition(newPos);
	
	// reset influence of animation on translation because all translation is set manually inside this method
	bone->setTranslation(CalVector());
	bone->calculateState();
    
 }

 Transform* LCSModifier::GetTransformForAnim(CalAnimation* anim, OsgAvatar* avatar)
{
    Transform* resultTransform = NULL;

	OsgAvatarType* type = avatar->getAvatarType();
    Animation* mot = type->GetMotion( StringHelper::GetFileNameForPath(anim->getCoreAnimation()->getFilename()));

    if (mot != NULL && mot->getAnimData() != NULL && ((MotionData*)mot->getAnimData())->getTransform() != NULL)
    {
		resultTransform  = ((MotionData*)mot->getAnimData())->getTransform();
    }
    else
    {
//        if (GenDebug::ERR)
//            _dbg << GenDebug::ERR_STR << "LCSModifier::GetTransformForAnim: empty motion or transform for curr anim " << anim->getCoreAnimation()->getFilename() << endl;
    }

    return resultTransform;
}

