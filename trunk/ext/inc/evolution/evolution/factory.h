/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_FACTORY_H
#define _FT_FACTORY_H

#include <string>

#include "avatar.h"
#include "world.h"

namespace ft
{
	/** 
	 * An Factory class - creates objects for the avatar's world
	 **/
    class EVOLUTION_API Factory
    {
    public:
		
		/**
		 * \brief Returns the only instance of ft::Factory (creates it at first call to this method)
		 * \return ft::Factory* - the only instance of Factory
		 **/
		static Factory* getInstance();

		/**
		 * \brief Creates new avatar that can be managed by ft::World
		 * \param const std::string& id - id of avatar (should be unique)
		 * \param const std::string& typePath - string that identifies type of avatar
		 * \return ft::Avatar*  - new avatar or NULL if creation failed
		 **/
		virtual Avatar* createAvatar(const std::string& id, const std::string& typePath) = 0;

		/**
		 * \brief Creates new instance of World
		 * \return ft::World* - new instance of World
		 **/
		virtual World* CreateWorld() = 0;

	private:
		static Factory* m_instance;
    };

}
#endif //_FT_FACTORY_H
