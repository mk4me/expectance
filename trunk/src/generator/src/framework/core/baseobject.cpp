/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "baseobject.h"
using namespace ft;

unsigned long long BaseObject::ID_COUNTER = 0;  // max 18446744073709551600;

/**
 * BaseObject: <describe the responsibilities and behavior of this method>
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
 * ~BaseObject: <describe the responsibilities and behavior of this method>
 *
 **/
BaseObject::~BaseObject(void) { /*empty*/}


/**
 * getClassName: <describe the responsibilities and behavior of this method>
 *
 * \return std::string <describe what is returned if appropriate>
 **/
std::string BaseObject::getClassName()
{
    return typeid(*this).name();
}

/**
 * getID: <describe the responsibilities and behavior of this method>
 *
 * \return const std::string & <describe what is returned if appropriate>
 **/
const std::string& BaseObject::getID() const
{
	return m_id;
}

/**
 * toString: <describe the responsibilities and behavior of this method>
 *
 * \return std::string <describe what is returned if appropriate>
 **/
std::string BaseObject::toString()
{
    std::string brace("<");
    return brace + getClassName() + "-" + getID() +">";
}

