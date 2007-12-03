/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AVATAR_FACTORY_H
#define _GEN_AVATAR_FACTORY_H

#include "scene/cal3dobjectfactory.h"
#include <map>

namespace ft
{
	/**
	 * Class AvatarFactory: is responsible for creation of avatar. It is specific type of ft::Cal3DObjectFactory
     * 
	 **/
    class AvatarFactory : public Cal3DObjectFactory
    {
    public:
        AvatarFactory (void) { /*empty*/ }
	    virtual ~AvatarFactory (void) { /*empty*/ }

        static AvatarFactory * getAvatarFactoryInstance();

    protected:
        virtual Cal3DObject* CreateMeshObjectInstance(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
        virtual Cal3dType* CreateCoreModel(const std::string &typeName);
        virtual Cal3dType* LoadCalCoreModel(const std::string modelName);

    private:
        static AvatarFactory* m_avatarFactoryInstance;

    };
};

#endif //_GEN_AVATAR_FACTORY_H