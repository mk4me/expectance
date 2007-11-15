/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */
#include "lcsmodifier.h"
//#include "cal3d/coreanimation.h"
#include "cal3d/coretrack.h"
#include "../utility/vishelper.h"
#include <math.h>

using namespace ft;

/// \brief constructor
LCSModifier::LCSModifier()
{
     m_vLastPos = CalVector(0,0,0);
     m_vLastPrevPos = CalVector(0,0,0);

     m_fAnimRot = 0;
     m_vLastAnimDir = CalVector(0,0,0);

    TRACE_TRANSLATION = false;
    TRACE_ANIM_ORIENT = false;
    TRACE_ROOT_ROTATION = false;
    TRACE_FINAL_ORIENT = false;
    TRACE_FINAL_DIR = false;
    TRACE_AXIS = false;
    LOCAL_DEBUG = false;

    INTERPOLATION = true;
    REST_TRANS_CALC = true;
    ANIM_DIR_CALC = false;

    if (TRACE_TRANSLATION)
    {
        tracer_translation = new TraceLine(toString() + "Translation");
        VisualizationManager::getInstance()->AddObject(tracer_translation);
        tracer_translation->HideMarker();
        tracer_translation->setColor(VisualizationHelper::COLOR_BLUE);
    }	

    if (TRACE_ROOT_ROTATION)
    {
        tracer_root_orient = new TraceLine(toString() + "tracer_root_orient");
        VisualizationManager::getInstance()->AddObject(tracer_root_orient);
    }

    if (TRACE_ANIM_ORIENT)
    {
        tracer_anim_orient  = new TraceLine(toString() + "tracer_rot_Curr");
        VisualizationManager::getInstance()->AddObject(tracer_anim_orient);
    }

    if (TRACE_FINAL_ORIENT)
    {
        tracer_final_orient  = new TraceLine(toString() + "tracer_final_orient");
        VisualizationManager::getInstance()->AddObject(tracer_final_orient);
    }

    if (TRACE_FINAL_DIR)
    {
        tracer_final_dir  = new TraceLine(toString() + "tracer_final_dir");
        VisualizationManager::getInstance()->AddObject(tracer_final_dir);
    }

    
    if (TRACE_AXIS)
    {
        tracer_X  = new TraceLine(toString() + "tracer_rot_X");
        tracer_Y  = new TraceLine(toString() + "tracer_rot_Y ");
        tracer_Z  = new TraceLine(toString() + "tracer_rot_Z");

        VisualizationManager::getInstance()->AddObject(tracer_X);
        VisualizationManager::getInstance()->AddObject(tracer_Y);
        VisualizationManager::getInstance()->AddObject(tracer_Z);
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

    if (tracer_root_orient!= NULL)
    {
        tracer_root_orient->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(tracer_root_orient);
    }

    if (tracer_final_orient != NULL)
    {
        tracer_final_orient->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(tracer_final_orient);
    }

    if (tracer_final_dir != NULL)
    {
        tracer_final_dir->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(tracer_final_dir);
    }
    
    if (TRACE_AXIS)
    {
        if (tracer_X!=NULL)
        {
            tracer_X->ClearTrace();
            VisualizationManager::getInstance()->RemoveObject(tracer_X);
        }

        if (tracer_Y != NULL)
        {
            tracer_Y->ClearTrace();
            VisualizationManager::getInstance()->RemoveObject(tracer_Y);
        }
        if (tracer_Z != NULL)
        {
            tracer_Z->ClearTrace();
            VisualizationManager::getInstance()->RemoveObject(tracer_Z);
        }
    }

    if (TRACE_ANIM_ORIENT)
    {
        if (tracer_anim_orient != NULL)
        {
            tracer_anim_orient->ClearTrace();
            VisualizationManager::getInstance()->RemoveObject(tracer_anim_orient);
        }
    }
}

/**
 * \brief Helper method which substracts some rotation component from Quaternion
 * \param CalQuaternion& rot - rotation from which substraction will be done
 * \param CalQuaternion& val - rotation component whic will be substracted from 'rot'
 * \return CalQuaternion& - result of substraction
 */
CalQuaternion&  UTIL_diff_quats(CalQuaternion& rot, CalQuaternion& val)
{
    //NOTE: inverted components     
    Quat restRotQuat = CalQuatToQuat(rot); 
    //restRotQuat.normalise();
    Quat valQuat = CalQuatToQuat(val);
    //valQuat.normalise();
    restRotQuat /= valQuat;
    rot = QuatToCalQuat(restRotQuat);

    return rot;
}

/**
 * \brief Helper method which tunes arccos argument to correct scope (-1,1)
 * \param float arg - correct argument for arccos fucntion
 */
float UTIL_GetRightArgForArcCos(float arg)
{
    if (arg > 1.0f)
        arg = 1.0f;
    else if (arg < -1.0f)
        arg = -1.0f;
    return arg;
}

/**
 * \brief Helper method which determine sign of angle for rotation of oldDir to newDir
 *      NOTE that passed vector arguments should be normalized
 * \param CalVector& oldDir - vertor which is the base for angle sign determiation
 * \param CalVector& newDir - vector for which angle sign in relation to oldDir
 * \return float - value -1 or 1 representing sign
 */
float UTIL_GetSignForDirChange(CalVector& oldDir, CalVector& newDir)
{
    CalVector rightDir = CalVector(0,1,0)%oldDir;
    rightDir.normalize();
    float dot = newDir * rightDir;
    float angle = radToDeg( acos(UTIL_GetRightArgForArcCos(dot)) );
    if (angle < 90.0f)
        return -1;
    else
        return 1;
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

    //  ROTATION ////////////////////

    CalQuaternion qCurrAnimOrient;
    CalQuaternion qCurrAvatarOrientation;

    CalQuaternion qGlobalRotOffset = timeLineContext->getAvatar()->getGlobalRotationOffset();

    CalQuaternion rootRotation;
    CalQuaternion tmpRot;

     
    // TRANSLATION //////////////////////
    CalVector vCurrAvatarPosition = timeLineContext->getAvatar()->getPosition();


    CalVector currPos;
    CalVector restTrans(0,0,0); // used in case of changing animation or new cycle in loop anim
    CalVector vPrevPos(0,0,0); //used only for OVERLAP state

    if (timeLineContext->currAnim != NULL)
    {

        CalVector translation;

        CalCoreAnimation* coreAnim = timeLineContext->currAnim->getCoreAnimation();
        CalCoreTrack* track = coreAnim->getCoreTrack(0);
        track->getState(timeLineContext->currAnimTime, currPos, tmpRot);

        float animTime = timeLineContext->currAnimTime;

        if (timeLineContext->anim_changed)
        {
            if (LOCAL_DEBUG) cout << " Anim changed .... anim time " << animTime  << endl;

            if (timeLineContext->prevAnim != NULL)
            {
                if (LOCAL_DEBUG) cout << " Prev != NULL " << endl;
                //calculate rest time
                timeLineContext->prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->prevAnimTime, restTrans, tmpRot);
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
            track->getState(animEndTime, restTrans, tmpRot);
            restTrans -= m_vLastPos;
            
            //position
            CalVector vStartOffset = currPos;
            CalVector startAnimPos;
            track->getState(0, startAnimPos, tmpRot);
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
                timeLineContext->prevAnim->getCoreAnimation()->getCoreTrack(0)->getState(timeLineContext->prevAnimTime, vPrevPos , tmpRot);
                if (timeLineContext->anim_changed)
                {
                    m_vLastPrevPos = vPrevPos;
                }
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
        //position
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
//                counter = (counter+1)%5;
            }
    }

    if (REST_TRANS_CALC)
    {
       diff += restTrans;
    }

     // ANIM DIRECTION calculating

    if (ANIM_DIR_CALC)
    {
        ApplyAnimDirectionToGlobalRotation(qGlobalRotOffset, currPos, timeLineContext);
    }

    diff *= qGlobalRotOffset; //ABAK

    vCurrAvatarPosition += diff;
   
    vCurrAvatarPosition.y = y_to_set;   

    m_vLastPos = currPos;

    //CalQuaternion rotToSet = qGlobalRotOffset * rootRotation;

    rootRotation = bone->getRotation();
    CalQuaternion rotToSet = qGlobalRotOffset * rootRotation;
    bone->setRotation(rotToSet); //ABAK

    //if (counter == 0)
    //{
    //    Quat rotQuat = CalQuatToQuat(rootRotation);
    //    cout << " x " << radToDeg(rotQuat.Xangle()) <<  " y " << radToDeg(rotQuat.Yangle())
    //        << " z " << radToDeg(rotQuat.Zangle()) << endl;
    //        // << " z " << rotQuat.Zangle() << endl;
    //}
    //counter = (counter+1)%10;

    bone->setTranslation(vCurrAvatarPosition);
    bone->calculateState();
	
	//set current parameters lcs to scene object
	timeLineContext->getAvatar()->setPosition(vCurrAvatarPosition);


    CalVector vOriginForward(1,0,0);      // original forward vector without affection of animation

    timeLineContext->getAvatar()->setGlobalRotationOffset(qGlobalRotOffset);
    timeLineContext->getAvatar()->setOrientation(rotToSet);

    CalVector vDir = vOriginForward;
    vDir *= rotToSet;
    timeLineContext->getAvatar()->setDirection(vDir);

    //TRACER-s -------------------------------------------------------

    if (TRACE_TRANSLATION)
    {
        tracer_translation->AddPoint(vCurrAvatarPosition);
    }

    if (TRACE_ANIM_ORIENT)
    {
        CalVector lastAnimDirTmp = m_vLastAnimDir;
        lastAnimDirTmp *= qGlobalRotOffset;;
        VisualizationHelper::TraceVector(tracer_anim_orient, lastAnimDirTmp, vCurrAvatarPosition, 100, VisualizationHelper::COLOR_SKYBLUE);
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
        VisualizationHelper::TraceVector(tracer_final_dir, timeLineContext->getAvatar()->getDirection(), vCurrAvatarPosition, 100, VisualizationHelper::COLOR_PINK);
    }


    if (TRACE_AXIS)
    {
        // X - axis 
        VisualizationHelper::TraceRotation(tracer_X, CalVector(1,0,0), vCurrAvatarPosition, CalQuaternion(), 100, VisualizationHelper::COLOR_RED);
        //Y - axis 
        VisualizationHelper::TraceRotation(tracer_Y, CalVector(0,1,0), vCurrAvatarPosition, CalQuaternion(), 100, VisualizationHelper::COLOR_GREEN);
        //Z - axis 
        VisualizationHelper::TraceRotation(tracer_Z, CalVector(0,0,1), vCurrAvatarPosition, CalQuaternion(), 100, VisualizationHelper::COLOR_BLUE);
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
//        cout << " ---------- anim changed  ----  m_fAnimRot " << radToDeg(m_fAnimRot) << endl;

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
                //    cout << " fCurrAnimRot " << radToDeg(fCurrAnimRot) << " m_fAnimRot " << radToDeg(m_fAnimRot) << endl;
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

/**
 * \brief Adds helper trace line for rendering
 * \param TraceLine *traceLine - traceline object (should be created and added to ft::VisualizationManager before)
 * \param  CalVector vBaseDir - base vector from which given rotation is defined
 * \param CalVector pos - current position of origin of traceline
 * \param CalQuaternion rot - rotation to visualize
 * \param float line_length - lenght of traceline to draw
 * \param CalVector vColor - collor of traceline
 */
//void LCSModifier::TraceRotation(TraceLine *traceLine, CalVector vBaseDir, CalVector pos, CalQuaternion rot, float line_length, CalVector vColor)
//{
//    traceLine->ClearTrace();
//
//    traceLine->setColor(vColor);
//    traceLine->AddPoint(pos);
//
//    vBaseDir *= line_length;
//    vBaseDir *= rot;
//
//    traceLine->AddPoint(pos + vBaseDir);
//}

/**
 * \brief Adds helper trace line for rendering
 * \param TraceLine *traceLine - traceline object (should be created and added to ft::VisualizationManager before)
 * \param  CalVector vVector - vector to visualize
 * \param CalVector pos - current position of origin of traceline
 * \param float line_length - lenght of traceline to draw
 * \param CalVector vColor - collor of traceline
 */
//void LCSModifier::TraceVector(TraceLine *traceLine, CalVector vVector, CalVector pos, float line_length, CalVector vColor)
//{
//    traceLine->ClearTrace();
//
//    if (vVector.length() >0)
//    {
//        traceLine->setColor(vColor);
//        traceLine->AddPoint(pos);
//
//        vVector.normalize();
//        vVector*= line_length;
//
//        traceLine->AddPoint(pos + vVector);
//    }
//}



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

    if (TRACE_ANIM_ORIENT)
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

