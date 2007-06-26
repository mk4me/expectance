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
		const std::string& getIDCounter() const;
	protected:
		BaseObject& setID(const std::string &id);
	private:
		void calculateID();
		static unsigned long long ID_COUNTER;
		std::string m_idFull;
		std::string m_idCntr;
    };



};

#endif //_GEN_CONTROL_OBJECT_H