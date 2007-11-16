/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "avatarfactory.h"
#include "../ai/aiavatar.h"
#include "../utility/debug.h"

using namespace ft;

AvatarFactory* AvatarFactory::m_avatarFactoryInstance = NULL;

/**
 * \brief Returns the only instance of ft::AvatarFactory(creates it at first call to this method)
 *
 * \return ft::AvatarFactory* the only instance of AvatarFactory
 **/
AvatarFactory* AvatarFactory::getAvatarFactoryInstance()
{
    if (m_avatarFactoryInstance == NULL)
    {
        DBG("AvatarFactory::getAvatarFactoryInstance(): instance of AvatarFactory created ");
        m_avatarFactoryInstance = new AvatarFactory();
    }

    return m_avatarFactoryInstance;
}


/**
 * \brief Overriden from ft::Cal3DObjectFactory
 **/
Cal3DObject* AvatarFactory::CreateMeshObjectInstance(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName)
{
    return new AIAvatar(calModel, calCoreModel, modelName);
}
