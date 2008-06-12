/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GIL_FACTORY_H
#define _FT_GIL_FACTORY_H

#include "gil_interface.h"
#include "gil_avatar.h"

namespace ft { namespace gil {

	//! The factory of GIL interfaces.
	/*!
	 * This class is responsible for creation interfaces that represents objects under GIL.
	 */
	class EVOLUTION_API GIL_Factory : public GIL_Interface
    {
	public:
		
		/** String format for GIL_Factory inteface */
		static const std::string iface_name;

		/**
		 * \brief Creates implementation of avatar under GIL
		 * \param const std::string& id - unique id of avatar
		 * \param const std::string& type - type of new avatar
		 */
		virtual GIL_Avatar* CreateAvatar(const std::string& id, const std::string& type) = 0;
		
    };
}  } //namespace ft::gil
#endif //_FT_GIL_FACTORY_H