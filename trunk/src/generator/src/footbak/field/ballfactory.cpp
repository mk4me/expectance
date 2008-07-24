/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "ballfactory.h"
#include "../../framework/app/gendebug.h"
#include "ball.h"

using namespace ft;

BallFactory* BallFactory::m_ballFactoryInstance = NULL;

/**
 * \brief Returns the only instance of ft::BallFactory(creates it at first call to this method)
 *
 * \return ft::BallFactory* the only instance of BallFactory
 **/
BallFactory* BallFactory::getBallFactoryInstance()
{
    if (m_ballFactoryInstance == NULL)
    {
        if (GenDebug::MODEL_LOADING)
            _dbg << "BallFactory::getBallFactoryInstance(): instance of BallFactory created " << endl;

        m_ballFactoryInstance = new BallFactory();
    }

    return m_ballFactoryInstance;
}


/**
 * \brief Overriden from ft::Cal3DObjectFactory
 **/
Cal3DObject* BallFactory::CreateMeshObjectInstance(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName)
{
    return new Ball(calModel, calCoreModel, modelName);
}
