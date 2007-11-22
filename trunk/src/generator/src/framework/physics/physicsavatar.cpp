/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "physicsavatar.h"

using namespace ft;
using namespace std;

/// \brief Constructor
/// \param CalModel * calModel - model in Cal3d which is referenced by this avatar
/// \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
/// \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
PhysicsAvatar::PhysicsAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName)
:MovableAvatar(calModel, calCoreModel, modelName)
{
    //implement
}

// \brief Destructor
PhysicsAvatar::~PhysicsAvatar()
{
    //empty
}

/// \brief Releases all resources and objects related to this Avatar
void PhysicsAvatar::Destroy(void)
{
    MovableAvatar::Destroy();
    //implement additional stuff
}

/// \brief Initiates physical part for Avatar
void PhysicsAvatar::Init()
{
    MovableAvatar::Init();
}


/// \brief Resets all parameters related to physical aspects of this avatar
void PhysicsAvatar::Reset()
{
    _dbg << toString() << " PhysicsAvatar::Reset() " << std::endl;
    MovableAvatar::Reset();
}

