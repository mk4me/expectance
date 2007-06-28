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
	m_id = "BaseObject" + sstr.str();;

}
BaseObject::~BaseObject(void) { /*empty*/}


std::string BaseObject::getClassName()
{
    return typeid(*this).name();
}

const std::string& BaseObject::getID() const
{
	return m_id;
}

std::string BaseObject::toString()
{
    std::string brace("<");
    return brace + getClassName() + "-" + getID() +">";
}

