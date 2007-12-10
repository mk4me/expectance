/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */
#include "lcsmodifier.h"
#include "cal3d/coretrack.h"
#include "utility/vishelper.h"
#include "utility/mathutil.h"
#include <math.h>
#include "../app/gendebug.h"
#include "../avatar/avatartype.h"
#include "scene/transformmanager.h"

using namespace ft;

/// \brief constructor
LCSModifier::LCSModifier()
{
     m_vLastPos = CalVector(0,0,0);
     m_vLastPrevPos = CalVector(0,0,0);

     m_fAnimRot = 0;
     m_vLastAnimDir = CalVector(0,0,0);

    TRACE_TRANSLATION = false;
    TRACE_TRANSFORM = false;
    TRACE_ROOT_ROTATION = false;

    TRACE_FINAL_ORIENT = false;
    TRACE_FINAL_DIR = false;

    TRACE_AXIS = false;
    LOCAL_DEBUG = false;

    INTERPOLATION = true;
    REST_TRANS_CALC = false;
    ANIM_DIR_CALC = false;

    if (TRACE_TRANSLATION)
    {
        tracer_translation = new TraceLine(toString() + "Translation");
        SceneManager::getInstance()->AddObject(tracer_translation);
        tracer_translation->HideMarker();
		tracer_translation->setBufferSize(0);
        tracer_translation->setColor(VisualizationHelper::COLOR_BLUE);
    }	

    if (TRACE_ROOT_ROTATION)
    {
        tracer_root_orient = new TraceLine(toString() + "tracer_root_orient");
        SceneManager::getInstance()->AddObject(tracer_root_orient);
    }

    if (TRACE_TRANSFORM)
    {
        tracer_anim_orient = new TraceLine(toString() + "tracer_rot_Curr");
        SceneManager::getInstance()->AddObject(tracer_anim_orient);
        tracer_anim_orient->setRenderingOrder(ft_Rendering_Objects_Level);
    }

    if (TRACE_FINAL_ORIENT)
    {
        tracer_final_orient  = new TraceLine(toString() + "tracer_final_orient");
        SceneManager::getInstance()->AddObject(tracer_final_orient);
    }

    if (TRACE_FINAL_DIR)
    {
        tracer_final_dir  = new TraceLine(toString() + "tracer_final_dir");
        SceneManager::getInstance()->AddObject(tracer_final_dir);
        tracer_final_dir->setRenderingOrder(ft_Rendering_Objects_Level);
    }

    
    if (TRACE_AXIS)
    {
        tracer_X  = new TraceLine(toString() + "tracer_rot_X");
        tracer_Y  = new TraceLine(toString() + "tracer_rot_Y ");
        tracer_Z  = new TraceLine(toString() + "tracer_rot_Z");

        SceneManager::getInstance()->AddObject(tracer_X);
        SceneManager::getInstance()->AddObject(tracer_Y);
        SceneManager::getInstance()->AddObject(tracer_Z);

        tracer_X->setBlending(false);
        tracer_X->setRenderingOrder(ft_Rendering_Objects_Level);
        tracer_Y->setBlending(false);
        tracer_Y->setRenderingOrder(ft_Rendering_Objects_Level);
        tracer_Z->setBlending(false);
        tracer_Z->setRenderingOrder(ft_Rendering_Objects_Level);
    }
}

/// \brief destructor
LCSModifier::~LCSModifier(void)
{
    if (tracer_translation != NULL)
    {
        tracer_translation->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_translation);
    }

    if (tracer_root_orient!= NULL)
    {
        tracer_root_orient->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_root_orient);
    }

    if (tracer_final_orient != NULL)
    {
        tracer_final_orient->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_final_orient);
    }

    if (tracer_final_dir != NULL)
    {
        tracer_final_dir->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_final_dir);
    }
    
    if (TRACE_AXIS)
    {
        if (tracer_X!=NULL)
        {
            tracer_X->ClearTrace();
            SceneManager::getInstance()->RemoveObject(tracer_X);
        }

        if (tracer_Y != NULL)
        {
            tracer_Y->ClearTrace();
            SceneManager::getInstance()->RemoveObject(tracer_Y);
        }
        if (tracer_Z != NULL)
        {
            tracer_Z->ClearTrace();
            SceneManager::getInstance()->RemoveObject(tracer_Z);
        }
    }

    if (TRACE_TRANSFORM)
    {
        if (tracer_anim_orient != NULL)
        {
            tracer_anim_orient->ClearTrace();
            SceneManager::getInstance()->RemoveObject(tracer_anim_orient);
        }
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

    UpdateRotation(elapsedSeconds, timeLineContext);
    UpdateTranslation(elapsedSeconds, timeLineContext);

    CalBone *bone = timeLineContext->getAvatar()->GetCalModel()->getSkeleton()->getBone(0);
    bone->calculateState();
	
    if (TRACE_AXIS)
    {
        CalVector vCurrAvatarPosition = timeLineContext->getAvatar()->getPosition();
        VisualizationHelper::TraceRotation(tracer_X, CalVector(1,0,0), vCurrAvatarPosition, CalQuaternion(), 100, VisualizationHelper::COLOR_RED);
        VisualizationHelper::TraceRotation(tracer_Y, CalVector(0,1,0), vCurrAvatarPosition, CalQuaternion(), 100, VisualizationHelper::COLOR_GREEN);
        VisualizationHelper::TraceRotation(tracer_Z, CalVector(0,0,1), vCurrAvatarPosition, CalQuaternion(), 100, VisualizationHelper::COLOR_BLUE);
    }
}

////////////////// ROTATION ///////////////////////////////

void LCSModifier::UpdateRotation(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    Transform* currTransform = NULL;
    if (timeLineContext->currAnim != NULL)
    {   
        currTransform = GetTransformForAnim(timeLineContext->currAnim, timeLineContext->getAvatar());
    }
    else
    {
        currTransform = GetTransformForType(timeLineContext->getAvatar());
    }

    CalQuaternion qGlobalRotOffset = timeLineContext->getAvatar()->getGlobalRotationOffset();

    if (ANIM_DIR_CALC)
    {
        //ApplyAnimDirectionToGlobalRotation(qGlobalRotOffset, currPos, timeLineContext);
    }

    CalQuaternion rootRotation; // = bone->getRotation();
    CalVector tmpPos;
    CalBone *bone = timeLineContext->getAvatar()->GetCalModel()->getSkeleton()->getBone(0);

    if (timeLineContext->currAnim != NULL)
    {
        timeLineContext->currAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->currAnimTime, tmpPos, rootRotation);
    }
    else
    {
        rootRotation = bone->getRotation();
    }
    
    if (currTransform != NULL)
    {
        rootRotation *= currTransform->getForwardDiff();
    }

    if (INTERPOLATION && timeLineContext->exec_state == EXEC_STATE_OVERLAP)
    {
        if (timeLineContext->prevAnim != NULL)
        {
            Transform* prevTransform = GetTransformForAnim(timeLineContext->prevAnim, timeLineContext->getAvatar());
            if (prevTransform!=NULL)
            {
                CalQuaternion prevRot;
                timeLineContext->prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->prevAnimTime, tmpPos, prevRot);
                prevRot *= prevTransform->getForwardDiff();
                float factor =  timeLineContext->currAnimTime/timeLineContext->prevOverlap; 
                rootRotation.blend( (1.0f-factor) ,prevRot);
            }
        }
        else
        {
            if (GenDebug::ERR)
                _dbg << GenDebug::ERR_STR << "LCSModifier::UpdateRotation: no prev anim for OVERLAP !!! " << endl;
        }
    }

    CalQuaternion rotToSet = qGlobalRotOffset * rootRotation;
    
    bone->setRotation(rotToSet); //ABAK

    timeLineContext->getAvatar()->setGlobalRotationOffset(qGlobalRotOffset);
    timeLineContext->getAvatar()->setOrientation(rotToSet);


    CalVector vOriginForward(1,0,0);      // original forward vector without affection of animation
    CalVector vDir = vOriginForward;
    vDir *= rotToSet;
    timeLineContext->getAvatar()->setDirection(vDir);

    //TRACER-s -------------------------------------------------------
    CalVector vCurrAvatarPosition = timeLineContext->getAvatar()->getPosition();
    if (TRACE_TRANSFORM)
    {
        CalQuaternion globalRot = timeLineContext->getAvatar()->getGlobalRotationOffset();
        CalVector originalForward;
        if (currTransform!=NULL)
        {
            originalForward = currTransform->getOrigForward();
            VisualizationHelper::TraceVector(tracer_anim_orient, originalForward, vCurrAvatarPosition , 150, VisualizationHelper::COLOR_SKYBLUE);
        }
    }

    if (TRACE_ROOT_ROTATION)
    {
        VisualizationHelper::TraceRotation(tracer_root_orient, vOriginForward, vCurrAvatarPosition, bone->getRotation(), 150, VisualizationHelper::COLOR_WHITE);
    }

    if (TRACE_FINAL_ORIENT)
    {
        VisualizationHelper::TraceRotation(tracer_final_orient, vOriginForward, vCurrAvatarPosition, timeLineContext->getAvatar()->getOrientation(), 170, VisualizationHelper::COLOR_YELLOW);
    }

    if (TRACE_FINAL_DIR)
    {
        VisualizationHelper::TraceVector(tracer_final_dir, timeLineContext->getAvatar()->getDirection(), vCurrAvatarPosition, 100, VisualizationHelper::COLOR_PURPLE);
    }
}

//////////////////// TRANSLATION ///////////////////////////////

void LCSModifier::UpdateTranslation(float elapsedSeconds, TimeLineContext * timeLineContext)
{
    Transform* currTransform = NULL;
    Transform* prevTransform = NULL;
    if (timeLineContext->currAnim != NULL)
    {   
        currTransform = GetTransformForAnim(timeLineContext->currAnim, timeLineContext->getAvatar());
    }
    else
    {
        currTransform = GetTransformForType(timeLineContext->getAvatar());
    }

    CalBone *bone = timeLineContext->getAvatar()->GetCalModel()->getSkeleton()->getBone(0);
    
    CalVector currPos;
    CalQuaternion tmpRot;    
    CalVector vPrevPos;
    

    if (timeLineContext->currAnim != NULL)
    {
        
        CalCoreAnimation* coreAnim = timeLineContext->currAnim->getCoreAnimation();
        coreAnim->getCoreTrack(0)->getState(timeLineContext->currAnimTime, currPos, tmpRot);
        if (currTransform != NULL)
        {
            currPos += currTransform->getPosOffset();
        }
 
        if (INTERPOLATION && timeLineContext->exec_state == EXEC_STATE_OVERLAP)
        {
            if (timeLineContext->prevAnim != NULL)
            {
                timeLineContext->prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->prevAnimTime, vPrevPos , tmpRot);
                prevTransform = GetTransformForAnim(timeLineContext->prevAnim, timeLineContext->getAvatar());
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
                if (GenDebug::ERR)
                    _dbg << GenDebug::ERR_STR << "LCSModifier::UpdateTranslation(): no prev anim for OVERLAP !!! " << endl;
            }
        }

        if (timeLineContext->anim_changed || timeLineContext->anim_new_cycle)
        {
            m_vLastPos = currPos;
        }
    }
    else
    {
        if (LOCAL_DEBUG) _dbg << " Original pos " << endl;
        currPos = bone->getTranslation();
        if (currTransform != NULL)
        {
            currPos += currTransform->getPosOffset();
        }
        m_vLastPos = currPos;
    }


    CalVector diff = currPos - m_vLastPos;

    CalQuaternion qGlobalRotation = timeLineContext->getAvatar()->getGlobalRotationOffset();

    if (currTransform)
    {
        qGlobalRotation *= currTransform->getForwardDiff();
    }

    diff *= qGlobalRotation;

    float y_to_set = currPos.y;

    if (INTERPOLATION && timeLineContext->exec_state == EXEC_STATE_OVERLAP)
            //&& timeLineContext->prevAnimTime <= timeLineContext->prevAnimDuration)
    {
        //position
        float factor =  timeLineContext->currAnimTime/timeLineContext->prevOverlap; 
        CalVector prevDiff = vPrevPos - m_vLastPrevPos;

        CalQuaternion qGlobalRotation = timeLineContext->getAvatar()->getGlobalRotationOffset();
        if (prevTransform != NULL)
        {
            qGlobalRotation *= prevTransform->getForwardDiff();
        }
        prevDiff *= qGlobalRotation;

        diff = (1.0f - factor) * prevDiff + factor * diff;
        y_to_set = (1.0f - factor) * vPrevPos.y + factor * currPos.y;

        m_vLastPrevPos = vPrevPos;
    }


    CalVector vCurrAvatarPosition = timeLineContext->getAvatar()->getPosition();
    vCurrAvatarPosition += diff;
    vCurrAvatarPosition.y = y_to_set;   

    m_vLastPos = currPos;

    bone->setTranslation(vCurrAvatarPosition);  //abak: y must be similiar to y in skeleton (why?)
	timeLineContext->getAvatar()->setPosition(vCurrAvatarPosition);

    
    //TRACER-s -------------------------------------------------------
    if (TRACE_TRANSLATION)
    {
        tracer_translation->AddPoint(vCurrAvatarPosition);
    }
 }

/**
 * \brief Applies direction of current animation to global rotation offset
 * \param CalQuaternion& qGlobalRotOffset - global rotation offset (will be modified in this methos)
 * \param CalVector currPos - current posistion of root bone
 * \param TimeLineContext * timeLineContext - current timeLineContext
 */
void LCSModifier::ApplyAnimDirectionToGlobalRotation(CalQuaternion& qGlobalRotOffset, CalVector& currPos, TimeLineContext * timeLineContext)
{
    if (timeLineContext->anim_changed || timeLineContext->anim_new_cycle || timeLineContext->anim_stopped)
    {
//        _dbg << " ---------- anim changed  ----  m_fAnimRot " << radToDeg(m_fAnimRot) << endl;

        if (m_fAnimRot != 0) // means that (timeLineContext->prevAnim != NULL)
        {
            CalQuaternion tmpQuat = QuatToCalQuat(Quat(m_fAnimRot, Vec(0,1,0)));
            tmpQuat *= qGlobalRotOffset;
            qGlobalRotOffset = tmpQuat;
        }
        m_fAnimRot = 0.0f;
        m_vLastAnimDir = CalVector();
    } else if (timeLineContext->currAnim != NULL)
    {
        //calculate current anim dir
        CalVector currPosTmp = currPos;
        currPosTmp.y = m_vLastPos.y;
        CalVector vCurrAnimDir = currPosTmp - m_vLastPos;
        vCurrAnimDir.normalize();

        if (m_vLastAnimDir.length() > 0 && vCurrAnimDir.length() > 0)
        {
                float fCurrAnimRot = 0;

                float dot = vCurrAnimDir*m_vLastAnimDir;
                dot = UTIL_GetRightArgForArcCos(dot);

                fCurrAnimRot = acos(dot);
               
                m_fAnimRot += UTIL_GetSignForDirChange(m_vLastAnimDir, vCurrAnimDir) * fCurrAnimRot;

                //if (counter == 0 || fCurrAnimRot > 0.1f)
                //{
                //    _dbg << " fCurrAnimRot " << radToDeg(fCurrAnimRot) << " m_fAnimRot " << radToDeg(m_fAnimRot) << endl;
                //}
                //counter = (counter+1)%30;

        }

        m_vLastAnimDir = vCurrAnimDir;
    }
}

/**
 * \brief Calculates orientation of root bone around Y axis
 * \param CalBone *rootBone - root bone
 * \return CalQuaternion - rotation around Y axis
 */
CalQuaternion LCSModifier::CalculateCurrentRootOrientAroundY(CalBone *rootBone)
{
    static CalQuaternion qGlobalRot;

    qGlobalRot = rootBone->getRotation();

    // extract rotation around Z axis (which means around Y in anim data)
    float angleZ = CalQuatToQuat(qGlobalRot).Zangle();

    //NOTE: constructor Quat(angle, axis) ignores sign for angle so it must be workarounded below
    // to achieve proper direction of rotation (proper sign of angle)
    if (angleZ > 0)
        qGlobalRot = QuatToCalQuat( Quat(angleZ, Vec(0,1,0)) );
    else
        qGlobalRot = QuatToCalQuat( Quat(angleZ, Vec(0,-1,0)) );

    return qGlobalRot;
}


/// \brief Resets parameters of this modifier
void LCSModifier::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);

    if (TRACE_TRANSLATION)
    {
        tracer_translation->ClearTrace();
    }

    if (TRACE_ROOT_ROTATION)
    {
        tracer_root_orient->ClearTrace();
    }

    if (TRACE_TRANSFORM)
    {
        tracer_anim_orient->ClearTrace();
    }

    if (TRACE_FINAL_ORIENT)
    {
        tracer_final_orient->ClearTrace();
    }

    if (TRACE_FINAL_DIR)
    {
        tracer_final_dir->ClearTrace();
    }

    if (TRACE_AXIS)
    {
        tracer_X->ClearTrace();
        tracer_Y->ClearTrace();
        tracer_Z->ClearTrace();
    }

     m_vLastPos = CalVector(0,0,0);
     m_vLastPrevPos = CalVector(0,0,0);

     m_fAnimRot = 0;
     m_vLastAnimDir = CalVector(0,0,0);
}

Transform* LCSModifier::GetTransformForAnim(CalAnimation* anim, Avatar* avatar)
{
    Transform* transform = NULL;
    AvatarType* type = (AvatarType*)avatar->GetCalCoreModel();
    Motion* mot = type->GetMotion( anim->getCoreAnimation()->getFilename());

    if (mot != NULL && mot->getTransform() != NULL)
    {
        transform = mot->getTransform();
    }
    else
    {
        if (GenDebug::ERR)
            _dbg << GenDebug::ERR_STR << "LCSModifier::GetTransformForAnim: empty motion or transform for curr anim " << anim->getCoreAnimation()->getFilename() << endl;
    }

    return transform;
}

Transform* LCSModifier::GetTransformForType(Avatar* avatar)
{
    AvatarType* type = (AvatarType*)avatar->GetCalCoreModel();
    return type->getTransform();
}
