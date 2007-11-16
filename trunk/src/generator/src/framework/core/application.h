/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_APPLICATION_H
#define _GEN_APPLICATION_H

#include "../avatar/avatar.h"

namespace ft
{
	/**
	 * Class Application: is responsible for lifecycle of Generator application
	 **/
    class Application
    {
    public:
        static Application* getInstance();
        static void DestroyInstance();

        void InitConfig();
        bool InitModules();
        void InitAvatars();
        void InitStaticObjects();
        void StartAISimulation();

        void SetCameraToActiveAvatar();

        Avatar* CreateAvatarOnScene(const std::string& calCoreModel,const std::string& name);

    private:
        static Application* m_instance;

    };
};

#endif //_GEN_APPLICATION_H

