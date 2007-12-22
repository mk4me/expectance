/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "physicsavatar.h"

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
    InitSpeedFactor();
}

/**
 * \brief Init speed factor and all scopes for factor changing
 **/
void PhysicsAvatar::InitSpeedFactor()
{
    setCurrSpeedFactor(1);
    setDestSpeedFactor(1);
    
    float minSpeed = 0.5f;
    if (Config::getInstance()->IsKey("default_min_speed_factor"))
    {
        minSpeed = Config::getInstance()->GetFloatVal("default_min_speed_factor");
    }
    setSpeedFactorMin(minSpeed);

    float maxSpeed = 1.5f;
    if (Config::getInstance()->IsKey("default_max_speed_factor"))
    {
        maxSpeed = Config::getInstance()->GetFloatVal("default_max_speed_factor");
    }
    setSpeedFactorMax(maxSpeed);
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
    setSpeedController(new SpeedController());
    tl->AddModifier(getSpeedController());
    
    return tl;
}

