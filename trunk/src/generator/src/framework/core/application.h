/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_APPLICATION_H
#define _GEN_APPLICATION_H

#include "../avatar/avatarfactory.h"

namespace ft
{
    class Application
    {
    public:
        static Application* getInstance();
        static void DestroyInstance();

        void InitConfig();
        void InitModules();
        void InitSceneObjects();

        Avatar* CreateAvatarOnScene(const std::string& calCoreModel,const std::string& name);

    private:
        static Application* m_instance;

    };
};

#endif //_GEN_APPLICATION_H

