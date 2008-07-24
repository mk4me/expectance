/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_PLAYER_FACTORY_H
#define _GEN_PLAYER_FACTORY_H

#include "../framework/avatar/avatarfactory.h"
#include <map>

namespace ft
{
	/**
	 * Class PlayerFactory: is responsible for creation of player. It is specific type of ft::AvatarFactory
     * 
	 **/
    class PlayerFactory : public AvatarFactory 
    {
    public:
        PlayerFactory (void) { /*empty*/ }
	    virtual ~PlayerFactory (void) { /*empty*/ }

        static PlayerFactory * getPlayerFactoryInstance();

    protected:
        virtual Cal3DObject* CreateMeshObjectInstance(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);

    private:
        static PlayerFactory* m_playerFactoryInstance;

    };
}

#endif //_GEN_PLAYER_FACTORY_H
