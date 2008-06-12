/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _FT_ENTITY_H
#define _FT_ENTITY_H

#include "../platform.h"

#include <typeinfo>
#include <string>

namespace ft
{

	/**
	 * Class BaseObject: base class for all objects that can be managed by Evolution package
	 **/
    class EVOLUTION_API Entity
    {
    public:
		/**
	     * \brief This flag defines if entity shiuld be described by short form of string representation
		 */
        static bool SHORT_DESC;

		/**
	     * \brief Default constructor
		 */
		Entity(void); 

		/**
	     * \brief Default destructor
		 */
	    virtual ~Entity(void);

		/**
	     * \brief Returns unique identifire of this entity
		 * \return const std::string& unique indentifier
		 */
		const std::string& getID() const;

		/**
	     * \brief Sets name of entity
		 * \param const std::string  - name of entity
		 */
        void setName(const std::string name) { m_name = name; }

		/**
	     * \brief Returns name of entity
		 * \return const std::string& - name of entity
		 */
        const std::string& getName() {return m_name; }

		/**
	     * \brief Returns class name of entity
		 * \return const std::string& - class name of entity
		 */
        std::string getClassName();

		/**
	     * \brief Returns string representation of entity
		 * \return const std::string& - string representation
		 */
        virtual std::string toString(); 

	protected:
		Entity& setID(const std::string &id);

	private:
		static unsigned long long ID_COUNTER;
		std::string m_id;
        std::string m_name;
    };
}

#endif //_FT_ENTITY_H
