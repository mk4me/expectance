/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "cal3dtype.h"
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

void Cal3dType::InitTransform()
{
    //implement
}

/**
 * \brief Prints debug information describing this cal3dtype on output console
 *
 **/
void Cal3dType::Dump()
{
    //TODO: implement
}   


