/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "cal3dtype.h"
#include "transformmanager.h"
#include "../utility/debug.h"

using namespace ft;

/**
 * \brief Constructor for Cal3d type
 *
 * \param const std::string - typeName
 **/
Cal3dType::Cal3dType(const std::string &typeName):
CalCoreModel(typeName)
{
    m_transform = NULL;
}

/**
 * \brief Destructor
 *
 **/
Cal3dType::~Cal3dType()
{
    //TODO: delete all motions of this cal3d object type
}

void Cal3dType::InitTransform(bool source_3dsmax)
{
    CalCoreBone* root = getCoreSkeleton()->getCoreBone(0);

    CalVector pos = root->getTranslation();
    CalQuaternion rot = root->getRotation();

    getTransform()->setOrigPosition(pos);
    // set offset as inversion of pos
    pos.x = -pos.x; pos.y = -pos.y; pos.z = -pos.z; 
    getTransform()->setPosOffset(pos);

    //CalVector dir = TransformManager::SCENE_FORWARD;
    //dir *= rot;

    //getTransform()->setOrigForward(dir);
    
    if (true)
        return;
}

/**
 * \brief Prints debug information describing this cal3dtype on output console
 *
 **/
void Cal3dType::Dump()
{
    //TODO: implement
}   


