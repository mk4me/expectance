/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "avatarfactory.h"
#include "../app/gendebug.h"
#include "calavatartype.h"
#include "calavatar.h"

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
    return new CalAvatar(calModel, calCoreModel, modelName);
}

Cal3dType* AvatarFactory::CreateCoreModel(const std::string &typeName)
{
    return new CalAvatarType(typeName);
}

Cal3dType* AvatarFactory::LoadCalCoreModel(const std::string modelName)
{
    Cal3dType* coreModel = Cal3DObjectFactory::LoadCalCoreModel(modelName);

    if (coreModel != NULL)
    {
        ((CalAvatarType*)coreModel)->InitMotions();
    }
    return coreModel;
}
