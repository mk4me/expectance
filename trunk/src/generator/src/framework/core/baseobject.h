/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_BASE_OBJECT_H
#define _GEN_BASE_OBJECT_H

#include "../core/global.h"
#include <typeinfo>

namespace ft
{

    class BaseObject 
    {

    public:
		BaseObject(void); 
	    virtual ~BaseObject(void);
		const std::string& getID() const;

        void setName(const std::string name) { m_name = name; }
        const std::string& getName() {return m_name; }

        std::string getClassName();
	protected:
		BaseObject& setID(const std::string &id);
	private:
		static unsigned long long ID_COUNTER;
		std::string m_id;
        std::string m_name;
    };
};

#endif //_GEN_CONTROL_OBJECT_H