/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AVATAR_FACTORY_H
#define _GEN_AVATAR_FACTORY_H

#include "../scene/meshobjectfactory.h"
#include <map>

namespace ft
{
	/**
	 * Class AvatarFactory: is responsible for creation of avatar. It is specific type of ft::MeshObjectFactory
     * 
	 **/
    class AvatarFactory : public MeshObjectFactory
    {
    public:
        AvatarFactory (void) { /*empty*/ }
	    virtual ~AvatarFactory (void) { /*empty*/ }

        static AvatarFactory * getAvatarFactoryInstance();

    protected:
        MeshObject* CreateMeshObjectInstance(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName);

    private:
        static AvatarFactory* m_avatarFactoryInstance;

    };
};

#endif //_GEN_AVATAR_FACTORY_H