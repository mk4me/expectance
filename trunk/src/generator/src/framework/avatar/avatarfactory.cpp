/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "avatarfactory.h"
#include "../ai/aiavatar.h"
#include "../app/gendebug.h"
#include "avatartype.h"

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
        if (GenDebug::MODEL_LOADING)
            _dbg << "AvatarFactory::getAvatarFactoryInstance(): instance of AvatarFactory created " << endl;

        m_avatarFactoryInstance = new AvatarFactory();
    }

    return m_avatarFactoryInstance;
}


/**
 * \brief Overriden from ft::Cal3DObjectFactory
 **/
Cal3DObject* AvatarFactory::CreateMeshObjectInstance(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName)
{
    return new AIAvatar(calModel, calCoreModel, modelName);
}

Cal3dType* AvatarFactory::CreateCoreModel(const std::string &typeName)
{
    return new AvatarType(typeName);
}

Cal3dType* AvatarFactory::LoadCalCoreModel(const std::string modelName)
{
    Cal3dType* coreModel = Cal3DObjectFactory::LoadCalCoreModel(modelName);

    if (coreModel != NULL)
    {
        ((AvatarType*)coreModel)->InitMotions();
    }
    return coreModel;
}
