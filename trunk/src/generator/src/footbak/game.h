/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GAME_H
#define _FT_GAME_H

#include "../framework/app/gamehandler.h"
#include "evolution/avatar.h"
#include "evolution/factory.h"
#include "../framework/evolution_impl/cal3dimpl.h"
#include "../framework/evolution_impl/cal3danimprovider.h"
#include "evolution/world.h"
#include "field/field.h"
#include "team.h"
#include "core/inputlistener.h"

namespace ft
{
	static const int LEFT_TEAM = 0;
	static const int RIGHT_TEAM = 1; 
	/**
	 * Manages main game flow
	 **/
	class Game: public GameHandler, public InputListener
    {
	public:
		Game(void);
		virtual void InitScene();
		virtual void OnUpdate(const double elapsedTime);
		void OnSpecial(int key, int x, int y);

	private:
		void InitStaticObjects();
        void InitAvatars();
        void SetCameraToActiveAvatar();
        void StartAISimulation();

        Avatar* CreateAvatarOnScene(const std::string& calCoreModel,const std::string& name);

        void InitWorld();
		void InitActionsForType(const std::string& avatarType);
		void InitGraphForType(const std::string& avatarType);


		Cal3dImpl m_evolutionImpl;
		Cal3dAnimProvider m_animProvider;

		World* m_world;

		Field* m_field;
		Team* m_teams[2];
    };

}
#endif //_FT_GAME_H
