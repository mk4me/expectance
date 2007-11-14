/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "aiavatar.h"

using namespace ft;
using namespace std;

/// \brief Constructor
/// \param CalModel * calModel - model in Cal3d which is referenced by this avatar
/// \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
/// \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
AIAvatar::AIAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName)
:ActionAvatar(calModel, calCoreModel, modelName)
{
    //implement
}

// \brief Destructor
AIAvatar::~AIAvatar()
{
    //empty
}

/// \brief Releases all resources and objects related to this Avatar
void AIAvatar::Destroy(void)
{
    ActionAvatar::Destroy();
    //implement additional stuff
}

/// \brief Initiates AI part for Avatar
void AIAvatar::Init()
{
    ActionAvatar::Init();
}


/// \brief Resets all parameters related to AI part of this avatar
void AIAvatar::Reset()
{
    ActionAvatar::Reset();
    cout << toString() << " AIAvatar::Reset() " << std::endl;
}
