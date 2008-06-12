/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GIL_MANAGER_H
#define _FT_GIL_MANAGER_H

#include "gil_interface.h"

#include <string>

namespace ft { namespace gil {

	//!A class to register GIL interfaces.
	/*!
	 * This class manages interfaces registered from GIL implementation. Thers is only one instance of this class.
	 */
	class EVOLUTION_API GIL_Manager: public GIL_Interface
    {
	public:
		/**
		 * \brief Returns the only instance of ft::GIL_Manager (creates it at first call to this method)
		 *
		 * \return ft::GIL_Manager* - the only instance of GIL_Manager
		 **/
		static GIL_Manager* getInstance();

		/**
		 * \brief Registers specific handler in GIL Manager
		 * \param const std::string& handlerName - name of interface 
		 * \param GIL_Interface* iface - object to register as handler
		 */
		virtual bool RegisterHandler(const std::string& handlerName, GIL_Interface* iface) = 0; 

		/**
		 * \brief Removes handler from GIL
		 * \param const std::string& handlerName - name of interface to remove from GIL Manager
		 */
		virtual bool UnregisterHandler(const std::string& handlerName) = 0; 

	private:
		static GIL_Manager* m_instance;

    };
}  } //namespace ft::gil
#endif //_FT_GIL_MANAGER_H