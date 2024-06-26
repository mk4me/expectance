/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "transformmanager.h"
#include "../core/config.h"

using namespace ft;
using namespace std;

// \brief Forward vector for objects on application scene
const CalVector TransformManager::SCENE_FORWARD = CalVector(1,0,0);

// \brief Up vector for objects on application scene
const CalVector TransformManager::SCENE_UP = CalVector(0,1,0);

// \brief Right vector for objects on application scene
const CalVector TransformManager::SCENE_RIGHT = CalVector(0,0,-1);

// \brief Left vector for objects on application scene
const CalVector TransformManager::SCENE_LEFT = CalVector(0,0,1);
// \ Rototion by 90 degree
const CalQuaternion TransformManager::Y_90 = QuatToCalQuat( Quat( DegToRad(90) , Vec(0,1,0)));


TransformManager* TransformManager::m_instance = NULL;

/**
 * \brief Constructor for TransformManager
 **/
TransformManager::TransformManager()
{
    TRACK_TRANSFORM = (Config::getInstance()->IsKey("track_transform")) && (Config::getInstance()->GetIntVal("track_transform")==1);
}

/**
 * \brief Destructor
 *
 **/
TransformManager::~TransformManager()
{
}

/**
 * \brief Returns the only instance of ft::TransformManager (creates it at first call to this method)
 *
 * \return ft::TransformManager* - the only instance of TransformManager
 **/
TransformManager* TransformManager::getInstance()
{
    if (m_instance == NULL)
    {
        if (Debug::SCENE>0)
            _dbg << "TransformManager::getInstace(): instance of TransformManager created " << endl;

        m_instance = new TransformManager();
    }

    return m_instance;
}

/**
 \brief Releases all resources related to TransformManager
 *
 **/
void TransformManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 \brief Creates new transform of given type
 \param const std::string& transformType - type of transform
 \return Tramsform* - created transform object
 **/
//Transform* TransformManager::CreateTransform(const std::string& transformType)
//{
//    Transform* transform = NULL;
//    
//    if (transformType.compare(TRANSFORM_FROM_3DSMAX) == 0)
//        transform = new Transform(TRANSFORM_FROM_3DSMAX);
//
//    if (transform == NULL && Debug::WARN)
//    {
//        _dbg << Debug::WARN_STR << " TransformManager::CreateTransform: no transform type " << transformType << endl;
//    }
//
//    return transform;
//}
