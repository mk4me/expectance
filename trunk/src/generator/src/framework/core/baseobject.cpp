/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "baseobject.h"
using namespace ft;

unsigned long long BaseObject::ID_COUNTER = 0;  // max 18446744073709551600;

/**
 * Constructor BaseObject
 *
 **/
BaseObject::BaseObject(void)
{
	ID_COUNTER++;
	std::stringstream sstr;
	sstr << ID_COUNTER;
	m_id = "BaseObject" + sstr.str();;

}
/**
 * Destructor for BaseObject
 *
 **/
BaseObject::~BaseObject(void) { /*empty*/}


/**
 * Returns class name for this BaseObject
 *
 * \return std::string - class name
 **/
std::string BaseObject::getClassName()
{
    return typeid(*this).name();
}

/**
 * Returns unique identifier of BaseObject 
 *
 * \return const std::string & - identifier
 **/
const std::string& BaseObject::getID() const
{
	return m_id;
}

/**
 * Returns string representation of this BaseObject
 *
 * \return std::string - string representation of this BaseObject
 **/
std::string BaseObject::toString()
{
    std::string brace("<");
    return brace + getClassName() + "-" + getID() +">";
}

