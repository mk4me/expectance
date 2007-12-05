/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "cal3dtype.h"
#include "transformmanager.h"
#include "../utility/debug.h"
#include "../utility/vishelper.h"

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
    if (Debug::TRANSFORM>0)
        _dbg << "Cal3dType::InitTransform" << " for " << this->getName() << std::endl;

    Transform* transform = new Transform(TRANSFORM_FROM_3DSMAX);

    CalCoreBone* root = getCoreSkeleton()->getCoreBone(0);
    CalVector pos = root->getTranslation();
    CalQuaternion rot = root->getRotation();

    transform->Init(pos, rot, source_3dsmax);

    if (transform->getTraceLine() != NULL)
    {
        transform->getTraceLine()->setColor(VisualizationHelper::COLOR_SKYBLUE);
    }

    setTransform(transform);
  
}

/**
 * \brief Prints debug information describing this cal3dtype on output console
 *
 **/
void Cal3dType::Dump()
{
    //TODO: implement
}   


