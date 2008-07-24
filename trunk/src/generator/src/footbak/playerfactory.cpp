/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "playerfactory.h"
#include "../framework/app/gendebug.h"
#include "player.h"

using namespace ft;

PlayerFactory* PlayerFactory::m_playerFactoryInstance = NULL;

/**
 * \brief Returns the only instance of ft::PlayerFactory(creates it at first call to this method)
 *
 * \return ft::PlayerFactory* the only instance of PlayerFactory
 **/
PlayerFactory* PlayerFactory::getPlayerFactoryInstance()
{
    if (m_playerFactoryInstance == NULL)
    {
        if (GenDebug::MODEL_LOADING)
            _dbg << "PlayerFactory::getPlayerFactoryInstance(): instance of PlayerFactory created " << endl;

        m_playerFactoryInstance = new PlayerFactory();
    }

    return m_playerFactoryInstance;
}


/**
 * \brief Overriden from ft::Cal3DObjectFactory
 **/
Cal3DObject* PlayerFactory::CreateMeshObjectInstance(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName)
{
    return new Player(calModel, calCoreModel, modelName);
}
