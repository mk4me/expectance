/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AVATAR_FACTORY_H
#define _GEN_AVATAR_FACTORY_H


#include "../core/global.h"
#include <map>
#include "../motion/movableavatar.h"

namespace ft
{
	/**
	 * Class AvatarFactory: <describe the behavior and responsibility of this class>
	 **/
    class AvatarFactory 
    {
    public:
        AvatarFactory (void) { /*empty*/ }
	    virtual ~AvatarFactory (void) { /*empty*/ }

        static AvatarFactory * getInstance();
        static void DestroyInstance();

        Avatar* CreateAvatar(const std::string modelName, const std::string avatarName);

    private:

        static AvatarFactory* m_instance;
        
        std::map<std::string,CalCoreModel*> m_coreModels;

        CalCoreModel* LoadCalCoreModel(const std::string modelName);

        void ReleaseCalCoreModels();

        bool ParseModelConfiguration(const std::string& strFilename, CalCoreModel* coreModel);

        void InitCoreModelMaterials(CalCoreModel* coreModel);

        void InitModelMeshes(CalCoreModel* coreModel, CalModel* model);

        GLuint LoadTexture(const std::string& strFilename);
    };
};

#endif //_GEN_AVATAR_FACTORY_H