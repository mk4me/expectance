/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_DEFAULTGAMEHANDLER_H
#define _FT_DEFAULTGAMEHANDLER_H

#include "gamehandler.h"
#include "evolution/avatar.h"
#include "evolution/factory.h"
#include "../evolution_impl/cal3dimpl.h"
#include "evolution/world.h"

namespace ft
{
	/**
	 * A default game handler for generator
	 **/
	class DefaultGameHandler: public GameHandler
    {
	public:
		virtual void InitScene();
		virtual void OnUpdate(const double elapsedTime);

	private:
		void InitStaticObjects();
        void InitAvatars();
        void SetCameraToActiveAvatar();
        void StartAISimulation();

        Avatar* CreateAvatarOnScene(const std::string& calCoreModel,const std::string& name);

		Cal3dImpl m_evolutionImpl;
		World* m_world;


    };

}
#endif //_FT_DEFAULTGAMEHANDLER_H
