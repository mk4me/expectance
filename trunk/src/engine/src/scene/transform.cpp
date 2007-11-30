/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "transform.h"
#include "../utility/debug.h"

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
    m_vForwardDiff = CalVector();
}

/**
 * \brief Destructor
 *
 **/
Transform::~Transform()
{
}

void Transform::Dump()
{
    if (Debug::SCENE)
    {
        //TODO: implement dump for transform
    }
}