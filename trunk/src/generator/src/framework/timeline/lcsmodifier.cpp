/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */
#include "lcsmodifier.h"
//#include "cal3d/coreanimation.h"
#include "cal3d/coretrack.h"
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
        tracer_curr_pos = new TraceLine(toString() + "AnimPosition");
        VisualizationManager::getInstance()->AddObject(tracer_translation);
        VisualizationManager::getInstance()->AddObject(tracer_curr_pos);
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

    if (tracer_curr_pos != NULL)
    {
        tracer_curr_pos->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(tracer_curr_pos);
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

//SOME help methods
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

// checks scope if is between -1 and 1
float UTIL_GetRightArgForArcCos(float arg)
{
    if (arg > 1.0f)
        arg = 1.0f;
    else if (arg < -1.0f)
        arg = -1.0f;
    return arg;
}

// returns sign of angle for rotation of oldDir to newDir
// given vectors should be normalized
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
        tracer_curr_pos->AddPoint(currPos + CalVector(0,50,0));
    }

    if (TRACE_ANIM_ORIENT)
    {
        CalVector lastAnimDirTmp = m_vLastAnimDir;
        lastAnimDirTmp *= qGlobalRotOffset;;
        TraceVector(tracer_anim_orient, lastAnimDirTmp, vCurrAvatarPosition, 100, CalVector(255,255,255));
    }

    if (TRACE_ROOT_ROTATION)
    {
        TraceRotation(tracer_root_orient, vOriginForward, vCurrAvatarPosition, bone->getRotation(), 150, CalVector(0,100,100));
    }

    if (TRACE_FINAL_ORIENT)
    {
        TraceRotation(tracer_final_orient, vOriginForward, vCurrAvatarPosition, timeLineContext->getAvatar()->getOrientation(), 170, CalVector(255,255,0));
    }

    if (TRACE_FINAL_DIR)
    {
        TraceVector(tracer_final_dir, timeLineContext->getAvatar()->getDirection(), vCurrAvatarPosition, 100, CalVector(128, 0, 128));
    }


    if (TRACE_AXIS)
    {
        // X - axis 
        TraceRotation(tracer_X, CalVector(1,0,0), vCurrAvatarPosition, CalQuaternion(), 100, CalVector(255,0,0));
        //Y - axis 
        TraceRotation(tracer_Y, CalVector(0,1,0), vCurrAvatarPosition, CalQuaternion(), 100, CalVector(0,255,0));
        //Z - axis 
        TraceRotation(tracer_Z, CalVector(0,0,1), vCurrAvatarPosition, CalQuaternion(), 100, CalVector(0,0,255));
    }
}

void LCSModifier::ApplyAnimDirectionToGlobalRotation(CalQuaternion& qGlobalRotOffset, CalVector currPos, TimeLineContext * timeLineContext)
{
    if (timeLineContext->anim_changed || timeLineContext->anim_new_cycle)
        //|| TODO: add anim stopped
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
    //else if () // no current anim
    //{
        //if anim stopped
            //if (m_fAnimRot!=0)
                //qGlobalRotOffset *= QuatToCalQuat( Quat(m_fAnimRot, Vec(0,1,0)));
    //}
}

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

void LCSModifier::TraceRotation(TraceLine *traceLine, CalVector vBaseDir, CalVector pos, CalQuaternion rot, float line_length, CalVector vColor)
{
    traceLine->ClearTrace();

    traceLine->setColor(vColor);
    traceLine->AddPoint(pos);

    vBaseDir *= line_length;
    vBaseDir *= rot;

    traceLine->AddPoint(pos + vBaseDir);
}

void LCSModifier::TraceVector(TraceLine *traceLine, CalVector vVector, CalVector pos, float line_length, CalVector vColor)
{
    traceLine->ClearTrace();

    if (vVector.length() >0)
    {
        traceLine->setColor(vColor);
        traceLine->AddPoint(pos);

        vVector.normalize();
        vVector*= line_length;

        traceLine->AddPoint(pos + vVector);
    }
}



/// \brief Resets parameters of this modifier
void LCSModifier::Reset(TimeLineContext * timeLineContext)
{
    TimeLineObject::Reset(timeLineContext);

    if (TRACE_TRANSLATION)
    {
        tracer_translation->ClearTrace();
        tracer_curr_pos->ClearTrace();
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

