/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "avatar.h"
#include "../utility/debug.h"

using namespace ft;

/**
 * \brief Constructor for avatar
 *
 * \param CalModel * calModel - model in Cal3d which is referenced by this avatar
 * \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
 * \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
 **/
Avatar::Avatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName):
Cal3DObject(calModel, calCoreModel, modelName)
{
    //empty
}

/**
 * \brief Destructor
 *
 **/
Avatar::~Avatar()
{
    //empty
}

/// \brief Releases all resources and objects related to this Avatar
void Avatar::Destroy(void)
{
    Cal3DObject::Destroy();
}
