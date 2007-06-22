/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "baseobject.h"
using namespace ft;

unsigned long long BaseObject::ID_COUNTER = 0;  // max 18446744073709551600;

BaseObject::BaseObject(void)
{
	ID_COUNTER++;
	std::stringstream sstr;
	sstr << ID_COUNTER;
	m_idCntr = sstr.str();
	m_idFull = typeid(*this).name() + m_idCntr;

}
BaseObject::~BaseObject(void) { /*empty*/}


const std::string& BaseObject::getID() const
{
	return m_idFull;
}

const std::string& BaseObject::getIDCounter() const 
{
	return m_idCntr;
}


BaseObject& BaseObject::setID(const std::string& id)
{
	if (!id.empty())
	{
		m_idFull = id;	
	}
	else
	{
		m_idFull = getID();
	}
	return *this;

}
