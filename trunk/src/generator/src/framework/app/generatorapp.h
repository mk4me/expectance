/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_GENERATORAPP_H
#define _GEN_GENERATORAPP_H

#include "core/application.h"
#include "../avatar/avatar.h"

namespace ft
{
	/**
	 * Class Application: is responsible for lifecycle of Generator application
	 **/
    class GeneratorApp : public Application
    {
    public:
        virtual int Init();
        virtual void InitObjects();
        virtual bool InitModules();
        virtual bool CreateVisualizationManager();
        void InitStaticObjects();
        void InitAvatars();
        void SetCameraToActiveAvatar();
        void StartAISimulation();

        Avatar* CreateAvatarOnScene(const std::string& calCoreModel,const std::string& name);

    };
};

#endif //_GEN_GENERATORAPP_H

