/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_BALL_FACTORY_H
#define _GEN_BALL_FACTORY_H

#include "scene/cal3dobjectfactory.h"
#include <map>

namespace ft
{
	/**
	 * Class BallFactory: is responsible for creation of ball
     * 
	 **/
    class BallFactory : public Cal3DObjectFactory
    {
    public:
        static BallFactory * getBallFactoryInstance();

    protected:
        virtual Cal3DObject* CreateMeshObjectInstance(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
    private:
        static BallFactory* m_ballFactoryInstance;

    };
}

#endif //_GEN_BALL_FACTORY_H
