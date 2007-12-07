/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "transform.h"
#include "transformmanager.h"
#include "../utility/debug.h"
#include "../utility/vishelper.h"
#include "../utility/mathutil.h"
#include "cal3d/coretrack.h"
#include "cal3d/corekeyframe.h"

using namespace ft;

/**
 * \brief Constructor for Transform
 **/
Transform::Transform(const std::string type)
{
    m_type = type;

    m_vOrigPosition = CalVector();
    m_vPosOffset = CalVector();
    m_vOrigForward = CalVector();
    m_qForwardDiff = CalQuaternion();

    m_forward_trace = NULL;

    if (TransformManager::getInstance()->TRACK_TRANSFORM)
    {
        m_forward_trace = new TraceLine(toString() + "m_forward_trace");
        VisualizationManager::getInstance()->AddObject(m_forward_trace);
        m_forward_trace->HideMarker();
    }	
}

/**
 * \brief Destructor
 *
 **/
Transform::~Transform()
{
    if (m_forward_trace != NULL)
    {
        m_forward_trace->ClearTrace();
        VisualizationManager::getInstance()->RemoveObject(m_forward_trace);
    }
}

void Transform::InitForType(const CalVector& origPos, const CalQuaternion& origQuat, bool source_3dsmax)
{
    Init(origPos, CalQuatToQuat(origQuat).Zangle(), source_3dsmax);
}

void Transform::InitForAnim(CalCoreAnimation* coreAnim, bool source_3dsmax)
{
    CalVector pos;
    CalQuaternion rot;
    (coreAnim->getCoreTrack(0))->getState(0.0f,pos,rot);

    float forwardAngle = CalculateAnimForward(coreAnim, 25, true);
    Init(pos, forwardAngle, source_3dsmax);
}

void Transform::Init(const CalVector& origPos, float forwardAngle, bool source_3dsmax)
{
    setOrigPosition(origPos);
 
    // set offset as inversion of pos with y=0
    // it can be used to use data as started from point (0,0,0)
    CalVector offset(-origPos.x, 0,-origPos.z); 
    setPosOffset(offset);
    
    //calculate orig forward
    CalVector vOrigForward(TransformManager::SCENE_FORWARD);
    CalQuaternion qDiffForward;

    if (source_3dsmax)
    {
        // rotation around Y is indicated by roation around Z in 3dsmax model (sign for angle is inversed
        // as in 3ds is right-hand coordinate system nut in cal3d is left-hand systsem) - it means that qDiffForward
        // will represent rotation which must be added to each rotation from data to achieve 0

        qDiffForward = QuatToCalQuat(   Quat( forwardAngle , Vec(0,1,0))    );
        
        //set orinal forward
        CalQuaternion quat = QuatToCalQuat(     Quat( -forwardAngle , Vec(0,1,0))    );
        vOrigForward *= quat;
    }
    
    setOrigForward(vOrigForward);
    setForwardDiff(qDiffForward);


    if (Debug::TRANSFORM>0)
    {
        Dump();
    }

    if (TransformManager::getInstance()->TRACK_TRANSFORM)
    {
        Trace(origPos);
    }
}

/**
 * \brief Calculates forward direction for animation
 *
 * \param CalCoreAnimation* coreAnim - animation for which forward will be calculated
 * \param int iterCount - how may frames should be used to calculate forward
 * \param bool from_begin - true if forward should be calculated for the begining of anim, false if for the end of anim 
 * \return float - calculated direction angle (as rotation from scene forward)
 **/
float Transform::CalculateAnimForward(CalCoreAnimation* coreAnim, int iterCount, bool from_begin)
{
    float angle = 0;

    int frameCount = coreAnim->getCoreTrack(0)->getCoreKeyframeCount();
    iterCount = (iterCount > frameCount) ? frameCount : iterCount;

    int n = (from_begin)? 0 : frameCount-1;

    for (int i = 0; i<iterCount; i++)
    {

        CalCoreKeyframe* frame = coreAnim->getCoreTrack(0)->getCoreKeyframe(n);
        float rotY = CalQuatToQuat(frame->getRotation()).Zangle();
        angle += rotY;

        float deg = RadToDeg(rotY);

        n = (from_begin)? n+1 : n-1;
    }

    angle /= iterCount;
    float degAvg = RadToDeg(angle);

    return angle;
}

void Transform::Trace(const CalVector& pos)
{
    if (TransformManager::getInstance()->TRACK_TRANSFORM)
    {
        VisualizationHelper::TraceVector(m_forward_trace, m_vOrigForward, pos, 100, VisualizationHelper::COLOR_YELLOW);
    }
}

void Transform::Dump()
{
    _dbg << "Transform::Dump() " << toString() << std::endl;
    UTIL_DumpVect(m_vOrigPosition, " - data position");
    UTIL_DumpVect(m_vPosOffset, " - pos offset");
    UTIL_DumpVect(m_vOrigForward, " - data forward");
    UTIL_DumpQuat(m_qForwardDiff, " - forwads diff");
}