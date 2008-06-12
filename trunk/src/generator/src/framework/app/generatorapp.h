/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_GENERATORAPP_H
#define _GEN_GENERATORAPP_H

#include "core/application.h"
#include "../avatar/calavatar.h"
#include "platform.h"
#include "evolution/world.h"
#include "../evolution_impl/cal3dimpl.h"

namespace ft
{
	/**
	 * Class Application: is responsible for lifecycle of Generator application
	 **/
    class GENERATOR_API GeneratorApp : public Application, public UpdateObject
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

        void printSomeText();  //to test python iface

		virtual void OnUpdate(const double elapsedTime);  // updates by UpdateManager

	private:
		Cal3dImpl m_evolutionImpl;
		World* m_world;

    };
}

#endif //_GEN_GENERATORAPP_H

