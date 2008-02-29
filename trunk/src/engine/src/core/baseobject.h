/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_BASE_OBJECT_H
#define _GEN_BASE_OBJECT_H

#include "../core/platform.h"
#include "../core/global.h"
#include <typeinfo>

namespace ft
{

	/**
	 * Class BaseObject: base class for all objects that can be managed by Generator application
	 **/
    class ENGINE_API BaseObject 
    {
    public:
        static bool SHORT_DESC;

		BaseObject(void); 
	    virtual ~BaseObject(void);
		const std::string& getID() const;

        void setName(const std::string name) { m_name = name; }
        const std::string& getName() {return m_name; }

        std::string getClassName();
        virtual std::string toString(); 
	protected:
		BaseObject& setID(const std::string &id);
	private:
		static unsigned long long ID_COUNTER;
		std::string m_id;
        std::string m_name;
    };
}

#endif //_GEN_CONTROL_OBJECT_H
