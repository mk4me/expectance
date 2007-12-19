/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "physicsavatar.h"
#include "speedcontroller.h"

using namespace ft;
using namespace std;

/// \brief Constructor
/// \param CalModel * calModel - model in Cal3d which is referenced by this avatar
/// \param Cal3dType * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
/// \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
PhysicsAvatar::PhysicsAvatar(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName)
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

/**
 * \brief Creates and initializes TimeLine object for this avatar
 *
 * \return ft::TimeLine * - TimeLine for testing framework
 **/
TimeLine* PhysicsAvatar::InitTimeLine()
{
    TimeLine* tl = MovableAvatar::InitTimeLine();
    tl->AddModifier(new SpeedController());
    return tl;
}

