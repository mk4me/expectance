/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "baseobject.h"
#include <typeinfo>

using namespace ft;

unsigned long long BaseObject::ID_COUNTER = 0;  // max 18446744073709551600;
bool BaseObject::SHORT_DESC = TRUE;

/**
 * \brief Constructor 
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
 * \brief Destructor 
 *
 **/
BaseObject::~BaseObject(void) { /*empty*/}


/**
 * \brief Returns class name for this BaseObject
 *
 * \return std::string - class name
 **/
std::string BaseObject::getClassName()
{
    return typeid(*this).name();
}

/**
 * \brief Returns unique identifier of BaseObject 
 *
 * \return const std::string & - identifier
 **/
const std::string& BaseObject::getID() const
{
	return m_id;
}

/**
 * \brief Returns string representation of this BaseObject
 *
 * \return std::string - string representation of this BaseObject
 **/
std::string BaseObject::toString()
{
    if (BaseObject::SHORT_DESC)
    {
        return "<" + m_id + ">";
    }
    else
    {
        std::string brace("<");
        return brace + getClassName() + "-" + getID() +">";
    }
}

