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

    DRAW_ROTATION_CURVES = ((Config::getInstance()->IsKey("lcs_rotation_curves")) && (Config::getInstance()->GetIntVal("lcs_rotation_curves")==1));
    DRAW_CURVES_TRANSLATION = ((Config::getInstance()->IsKey("lcs_translation_curves")) && (Config::getInstance()->GetIntVal("lcs_translation_curves")==1));
    DRAW_CURVES_TRANSLATION_DETAILED = ((Config::getInstance()->IsKey("lcs_translation_detailed_curves")) && (Config::getInstance()->GetIntVal("lcs_translation_detailed_curves")==1));

    TRACE_TRANSLATION = false;
    TRACE_TRANSFORM = false;
    TRACE_ROOT_ROTATION = ((Config::getInstance()->IsKey("lcs_trace_root_rotation")) && (Config::getInstance()->GetIntVal("lcs_trace_root_rotation")==1));

    TRACE_TRANSFORM_END = false;
    TRACE_FINAL_DIR = false;

    TRACE_AXIS = ((Config::getInstance()->IsKey("lcs_trace_axis")) && (Config::getInstance()->GetIntVal("lcs_trace_axis")==1));
    LOCAL_DEBUG = false;

    INTERPOLATION = true;
    REST_TRANS_CALC = false;
    ANIM_DIR_CALC = (Config::getInstance()->IsKey("lcs_anim_orient_influence")) && (Config::getInstance()->GetIntVal("lcs_anim_orient_influence")==1);		
    ANIM_DIR_CALC_FOR_CYCLES = false;

    if (DRAW_ROTATION_CURVES)
    {
		curve_anim_rotation = new DataCollector(toString() + "curve_anim_rotation");
        VisualizationManager::getInstance()->AddDataObject(curve_anim_rotation);
        curve_anim_rotation->HidePoints();       curve_anim_rotation->setColor(VisualizationHelper::COLOR_YELLOW);
        curve_anim_rotation->setDrawScale(20);        

		curve_global_rotation = new DataCollector(toString() + "curve_global_rotation");
        VisualizationManager::getInstance()->AddDataObject(curve_global_rotation);
        curve_global_rotation->HidePoints();       curve_global_rotation->setColor(VisualizationHelper::COLOR_WHITE);
        curve_global_rotation->setDrawScale(20);        

        
		curve_final_rotation = new DataCollector(toString() + "curve_final_rotation");
        VisualizationManager::getInstance()->AddDataObject(curve_final_rotation);
        curve_final_rotation->HidePoints();        curve_final_rotation->setColor(VisualizationHelper::COLOR_SKYBLUE);
        curve_final_rotation->setDrawScale(20);
    }

    if(DRAW_CURVES_TRANSLATION_DETAILED)
    {
		curve_trans_diff_X = new DataCollector(toString() + "curve_trans_diff_X");
        VisualizationManager::getInstance()->AddDataObject(curve_trans_diff_X);
        curve_trans_diff_X->HidePoints();       curve_trans_diff_X->setColor(VisualizationHelper::COLOR_RED);
        curve_trans_diff_X->setDrawScale(5);
        //curve_trans_X->setDrawOffset(-90);

   		curve_trans_diff_Z = new DataCollector(toString() + "curve_trans_diff_Z");
        VisualizationManager::getInstance()->AddDataObject(curve_trans_diff_Z);
        curve_trans_diff_Z->HidePoints();       curve_trans_diff_Z->setColor(VisualizationHelper::COLOR_GREEN);
        curve_trans_diff_Z->setDrawScale(5);

		curve_trans_diff_Y = new DataCollector(toString() + "curve_trans_diff_Y");
        VisualizationManager::getInstance()->AddDataObject(curve_trans_diff_Y);
        curve_trans_diff_Y->HidePoints();       curve_trans_diff_Y->setColor(VisualizationHelper::COLOR_BLUE);
        curve_trans_diff_Y->setDrawOffset(5);        
    }

    if(DRAW_CURVES_TRANSLATION)
    {
		curve_trans_diff = new DataCollector(toString() + "curve_trans_diff");
        VisualizationManager::getInstance()->AddDataObject(curve_trans_diff);
        curve_trans_diff->HidePoints();       curve_trans_diff->setColor(VisualizationHelper::COLOR_SKYBLUE);
        curve_trans_diff->setDrawScale(5);

		curve_trans_Y = new DataCollector(toString() + "curve_trans_Y");
        VisualizationManager::getInstance()->AddDataObject(curve_trans_Y);
        curve_trans_Y->HidePoints();       curve_trans_Y->setColor(VisualizationHelper::COLOR_YELLOW);
        curve_trans_Y->setDrawOffset(-50);        //curve_trans_Y->setDrawScale(0.8f);
    }


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
        tracer_root_orient->setRenderingOrder(ft_Rendering_Objects_Level);
    }

    if (TRACE_TRANSFORM)
    {
        tracer_transform = new TraceLine(toString() + "tracer_transform");
        SceneManager::getInstance()->AddObject(tracer_transform);
        tracer_transform->setRenderingOrder(ft_Rendering_Objects_Level);
    }

    if (TRACE_TRANSFORM_END)
    {
        tracer_transform_end  = new TraceLine(toString() + "tracer_transform_end");
        SceneManager::getInstance()->AddObject(tracer_transform_end);
        tracer_transform_end->setRenderingOrder(ft_Rendering_Objects_Level);
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

    if(DRAW_CURVES_TRANSLATION_DETAILED)
    {
        curve_trans_diff_X->Clear();
        VisualizationManager::getInstance()->RemoveDataObject(curve_trans_diff_X);
        curve_trans_diff_Y->Clear();
        VisualizationManager::getInstance()->RemoveDataObject(curve_trans_diff_Y);
        curve_trans_diff_Z->Clear();
        VisualizationManager::getInstance()->RemoveDataObject(curve_trans_diff_Z);
    }

    if(DRAW_CURVES_TRANSLATION)
    {
        curve_trans_diff->Clear();
        VisualizationManager::getInstance()->RemoveDataObject(curve_trans_diff);
        curve_trans_Y->Clear();
        VisualizationManager::getInstance()->RemoveDataObject(curve_trans_Y);
    }
    
    if (DRAW_ROTATION_CURVES)
    {
		curve_anim_rotation->Clear();
        VisualizationManager::getInstance()->RemoveDataObject(curve_anim_rotation);
		curve_global_rotation->Clear();
        VisualizationManager::getInstance()->RemoveDataObject(curve_global_rotation);
        curve_final_rotation->Clear();
        VisualizationManager::getInstance()->RemoveDataObject(curve_final_rotation);
    }
    
    if (TRACE_ROOT_ROTATION)
    {
        tracer_root_orient->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_root_orient);
    }

    if (TRACE_TRANSFORM)
    {
        tracer_transform->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_transform);
    }

    if (TRACE_TRANSFORM_END)
    {
        tracer_transform_end->ClearTrace();
        SceneManager::getInstance()->RemoveObject(tracer_transform_end);
    }

    if (TRACE_FINAL_DIR)
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

    if (ANIM_DIR_CALC)
    {
        ApplyAnimDirectionToGlobalRotation(timeLineContext);
    }

    CalQuaternion qGlobalRotOffset = timeLineContext->getAvatar()->getGlobalRotationOffset();

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
    if (DRAW_ROTATION_CURVES)
    {
        float animRot = CalQuatToQuat(rootRotation).Zangle();
        float globalRot = CalQuatToQuat(qGlobalRotOffset).Yangle();
        globalRot = UTIL_IsValidFloat(globalRot) ? globalRot : 0;
        float finalRot = CalQuatToQuat(rotToSet).Zangle();
        curve_anim_rotation->AddValue(animRot);
        curve_global_rotation->AddValue(globalRot);
        curve_final_rotation->AddValue(finalRot);
    }

    CalVector vCurrAvatarPosition = timeLineContext->getAvatar()->getPosition();
    if (TRACE_TRANSFORM)
    {
        CalQuaternion globalRot = timeLineContext->getAvatar()->getGlobalRotationOffset();
        CalVector originalForward;
        if (currTransform!=NULL)
        {
            originalForward = currTransform->getOrigForward();
            VisualizationHelper::TraceVector(tracer_transform, originalForward, vCurrAvatarPosition , 150, VisualizationHelper::COLOR_YELLOW);
        }
    }

    if (TRACE_ROOT_ROTATION)
    {
        VisualizationHelper::TraceRotation(tracer_root_orient, vOriginForward, vCurrAvatarPosition, bone->getRotation(), 150, VisualizationHelper::COLOR_WHITE);
    }

    if (TRACE_TRANSFORM_END)
    {
       CalQuaternion endForward;
       if (currTransform!=NULL)
       {
           endForward = currTransform->getEndForwardDiff();
       }
 
       VisualizationHelper::TraceRotation(tracer_transform_end, TransformManager::SCENE_FORWARD, vCurrAvatarPosition, endForward, 170, VisualizationHelper::COLOR_SKYBLUE);
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
    CalVector animDiff(diff);

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

    if(DRAW_CURVES_TRANSLATION)
    {
        curve_trans_diff->AddValue(diff.length());
        curve_trans_Y->AddValue(vCurrAvatarPosition.y);
    }

    if(DRAW_CURVES_TRANSLATION_DETAILED)
    {
        curve_trans_diff_X->AddValue(diff.x);
        curve_trans_diff_Y->AddValue(diff.y);
        curve_trans_diff_Z->AddValue(diff.z);
    }

    

    if (TRACE_TRANSLATION)
    {
        tracer_translation->AddPoint(vCurrAvatarPosition);
    }
 }

 /**
 * \brief Applies direction of current animation to global rotation offset
 * \param TimeLineContext * timeLineContext - current timeLineContext
 */
 void LCSModifier::ApplyAnimDirectionToGlobalRotation(TimeLineContext * timeLineContext)
 {
     CalQuaternion qGlobalRotOffset = timeLineContext->getAvatar()->getGlobalRotationOffset();

     bool anim_dir_for_cycles = (ANIM_DIR_CALC_FOR_CYCLES && timeLineContext->anim_new_cycle);

     if (timeLineContext->anim_changed || timeLineContext->anim_stopped || anim_dir_for_cycles)
     {
         if (timeLineContext->exec_state != EXEC_STATE_OVERLAP)
         {
            if (timeLineContext->prevMotion != NULL)
            {
                
                Transform* transform = NULL;

                if (timeLineContext->anim_changed || timeLineContext->anim_stopped)
                    transform = timeLineContext->prevMotion->getTransform();

                if (timeLineContext->anim_new_cycle)
                    transform = timeLineContext->currMotion->getTransform();

                if (transform != NULL)
                {
                    CalQuaternion animDiff = transform->getAnimEndDiff();
                    qGlobalRotOffset *= animDiff;
                    //cout << " Change anim direction .. " << RadToDeg(CalQuatToQuat(animDiff).Yangle()) <<  endl;
                }
            }
         }
         else
         {
             Transform* transform = NULL;
             transform = timeLineContext->prevMotion->getTransform();
             
             if (transform != NULL)
             {
                 if (transform != NULL)
                 {
                     m_qAnimDirOverlapStart=qGlobalRotOffset;
                     m_qAnimDirOverlapDest = m_qAnimDirOverlapStart * transform->getAnimEndDiff();
                 }
                 else
                 {
                     m_qAnimDirOverlapStart = qGlobalRotOffset;
                     m_qAnimDirOverlapDest = qGlobalRotOffset;
                 }

                 //CalQuaternion animDiff = transform->getAnimEndDiff();
                 //cout << " Change anim direction <START_OVERLAP>.. " << RadToDeg(CalQuatToQuat(animDiff).Yangle()) <<  endl;
             }
         }
     }

     if (timeLineContext->exec_state == EXEC_STATE_OVERLAP || timeLineContext->exec_state == EXEC_STATE_FADE_OUT)
     {
        //add interpolated part
         Transform* transform = timeLineContext->prevMotion->getTransform();
         if (transform != NULL)
         {
             float factor = (timeLineContext->prevAnimDuration - timeLineContext->prevAnimTime)/timeLineContext->prevOverlap;
             CalQuaternion rot = m_qAnimDirOverlapStart;
             rot.blend((1-factor), m_qAnimDirOverlapDest);
             qGlobalRotOffset = rot;
         }
     }
     timeLineContext->getAvatar()->setGlobalRotationOffset(qGlobalRotOffset);
 }

/// \brief Resets parameters of this modifier
void LCSModifier::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);

    if(DRAW_CURVES_TRANSLATION_DETAILED)
    {
        curve_trans_diff_X->Clear();
        curve_trans_diff_Y->Clear();
        curve_trans_diff_Z->Clear();
    }

    if(DRAW_CURVES_TRANSLATION)
    {
        curve_trans_diff->Clear();
        curve_trans_Y->Clear();
    }

    if (DRAW_ROTATION_CURVES)
    {
        curve_final_rotation->Clear();
        curve_anim_rotation->Clear();
        curve_global_rotation->Clear();
    }

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
        tracer_transform->ClearTrace();
    }

    if (TRACE_TRANSFORM_END)
    {
        tracer_transform_end->ClearTrace();
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
