/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_TEAM_H
#define _FT_TEAM_H

#include "evolution/avatar.h"
#include "field/field.h"
#include "scene/circle.h"

namespace ft
{
	static const int TEAM_SIZE = 2;
	/**
	 * Manages footballers belongs to one team
	 **/
	class Team
    {
	public:
		Team(void);
		~Team(void);
		void Update(const double elapsedTime, Field* field);

		void PrepareToGame();

        virtual bool AddPlayer(Avatar* player);
        virtual bool RemovePlayer(Avatar* player);

	private:
		void UpdateRestPositions(Field* field);

		std::vector<Avatar*> m_vPlayers;

		CalVector m_restPositions[TEAM_SIZE];
		Circle* m_restCircles[TEAM_SIZE];
		
    };

}
#endif //_FT_TEAM_H
