/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "transform.h"
#include "transformmanager.h"
#include "../utility/debug.h"
#include "../utility/vishelper.h"
#include "../utility/mathutil.h"

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

void Transform::Init(const CalVector& origPos, const CalQuaternion& origQuat, bool source_3dsmax)
{
    setOrigPosition(origPos);
    // set offset as inversion of pos
    CalVector offset(-origPos.x,-origPos.y,-origPos.z); 
    setPosOffset(offset);
    
    //calculate orig forward
    CalVector vOrigForward(TransformManager::SCENE_FORWARD);
    CalQuaternion qDiffForward;

    if (source_3dsmax)
    {
        // rotation around Y is indicated by roation around Z in 3dsmax model (sign for angle is inversed
        // as in 3ds is right-hand coordinate system nut in cal3d is left-hand systsem)

        qDiffForward = QuatToCalQuat(     Quat( -CalQuatToQuat(origQuat).Zangle() , Vec(0,1,0))    );
        vOrigForward *= qDiffForward;
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