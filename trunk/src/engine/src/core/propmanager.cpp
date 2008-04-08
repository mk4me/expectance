/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "propmanager.h"

using namespace ft;

PropManager* PropManager::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::PropManager(creates it at first call to this method)
 *
 * \return ft::PropManager * - the only instance of PropManager
 **/
PropManager* PropManager::getInstance()
{
    if (m_instance == NULL)
    {
        std::cout << "PropManager::getInstace(): instance of PropManager created " << std::endl;
        m_instance = new PropManager();
    }

    return m_instance;
}


/**
 * \brief Releases all resources related to this PropManager
 *
 **/
void PropManager::DestroyInstance()
{
    if (m_instance != NULL)
    {
        m_instance->ClearEntries();
        delete m_instance;
    }
}

/**
 * \brief Clear all properties
 *
 **/
void PropManager::ClearEntries()
{
   	std::map<std::string,Property*>::iterator it=m_entries.map.begin();
	for( ; it != m_entries.map.end(); ++it ) 
    {
        delete it->second;
    }

    m_entries.map.clear();
}

/**
 * \brief Returns Property object from m_entries
 * \param const std::string& key - key for given property
 * \return ft::Property* - property if exists, NULL otherwsie
 **/
Property* PropManager::GetProperty(const std::string& key)
{
    Property* prop;
  	std::map<std::string,Property*>::iterator it = m_entries.map.find(key);
    if ( it!=m_entries.map.end()) 
        prop = it->second;
    else
        prop = NULL;

    return prop;
}

/**
 * \brief Sets string value for property
 * \param const std::string& key - key for given property
 * \param std::string val - value for given property
 **/
void PropManager::setPropertyStr(const std::string& key, std::string val)
{
    Property* prop = GetProperty(key);
    if (prop == NULL)
    {
        prop = new Property();
        prop->str_val = val;
        m_entries.map.insert( std::make_pair( std::string(key), prop ) );
    }
    else
    {
        if (prop->handler != NULL)
        {
            prop->handler->setPropertyStr(key, val);
        }
        else
        {
            prop->str_val = val;
        }
    }
}

/**
 * \brief Sets int value for property
 * \param const std::string& key - key for given property
 * \param int val - value for given property
 **/
void PropManager::setPropertyInt(const std::string& key, int val)
{
    Property* prop = GetProperty(key);
    if (prop == NULL)
    {
        if (Debug::ERR)
            _dbg << Debug::ERR_STR <<"PropManager::setPropertyInt:  no property found for int, key: " << key << std::endl;
    }
    else
    {
        if (prop->handler != NULL)
        {
            prop->handler->setPropertyInt(key, val);
        }
        else
        {
            if (Debug::ERR)
                _dbg << Debug::ERR_STR <<"PropManager::setPropertyInt:  no handler found for int, key: " << key << endl;

        }
    }
}

/**
 * \brief Sets float value for property
 * \param const std::string& key - key for given property
 * \param float val - value for given property
 **/
void PropManager::setPropertyFloat(const std::string& key, float val)
{
    Property* prop = GetProperty(key);
    if (prop == NULL)
    {
        if (Debug::ERR)
            _dbg << Debug::ERR_STR <<"PropManager::setPropertyFloat:  no property found for float, key: " << key << std::endl;
    }
    else
    {
        if (prop->handler != NULL)
        {
            prop->handler->setPropertyFloat(key, val);
        }
        else
        {
            if (Debug::ERR)
                _dbg << Debug::ERR_STR <<"PropManager::setPropertyFloat:  no handler found for float, key: " << key << endl;
        }
    }
}

/**
 * \brief Returns string value for property
 * \param const std::string& key - key for given property
 * \return std::string& - value for given property
 **/
std::string PropManager::getPropertyStr(const std::string& key)
{
    std::string result = UNDEFINED_PROPERTY;

    Property* prop = GetProperty(key);
    if (prop != NULL)
    {
        if (prop->handler != NULL)
        {
            result = prop->handler->getPropertyStr(key);
        }
        else
        {
            result = prop->str_val;
        }
    }
    return result;
}

/**
 * \brief Returns int value for property
 * \param const std::string& key - key for given property
 * \return int - value for given property
 **/
int PropManager::getPropertyInt(const std::string& key)
{
    int result = -1;

    Property* prop = GetProperty(key);
    if (prop != NULL && prop->handler != NULL)
    {
        result = prop->handler->getPropertyInt(key);
    }
    else
    {
        if (Debug::ERR)
            _dbg << Debug::ERR_STR <<"PropManager::getPropertyInt:  no handler found for int, key: " << key << endl;
    }
    return result;
}

/**
 * \brief Returns float value for property
 * \param const std::string& key - key for given property
 * \return float - value for given property
 **/
float PropManager::getPropertyFloat(const std::string& key)
{
    int result = -1;

    Property* prop = GetProperty(key);
    if (prop != NULL && prop->handler != NULL)
    {
        result = prop->handler->getPropertyFloat(key);
    }
    else
    {
        if (Debug::ERR)
            _dbg << Debug::ERR_STR <<"PropManager::getPropertyFloat:  no handler found for float, key: " << key << endl;
    }
    return result;
}

/**
 * \brief Removes property of given key
 * \param const std::string& key - key for given property
 * \return bool - true if property removed in fact
 **/
bool PropManager::RemoveProperty(const std::string& key)
{
    bool result = true;

 	std::map<std::string,Property*>::iterator it = m_entries.map.find(key);
	if ( it!=m_entries.map.end()) { 
        m_entries.map.erase(it);
        delete it->second;
		result = true;
	}
	return result;
}

/**
 * \brief Adds hadler for given key
 * \param const std::string& key - key for given property
 * \param PropHandler* handler - handler to set
 * \return bool - true if handler added successfuly 
 **/
bool PropManager::AddHandler(const std::string& key, PropHandler* handler)
{
    bool result = false;
    Property* prop = GetProperty(key);
    if (prop == NULL || prop->handler == NULL)
    {
        if (prop == NULL)
        {
            prop = new Property();
        }
        prop->handler = handler;
        result = true;
    }
    else
    {
        if (Debug::ERR)
            _dbg << Debug::ERR_STR <<"PropManager::AddHandler: handler key: " << key << " already exists " << endl;
    }
    return result;
}

/**
 * \brief Removes hadler for given key
 * \param const std::string& key - key for given property
 * \return bool - true if handler removed in fact
 **/
bool PropManager::RemoveHandler(const std::string& key)
{
    bool result = false;
    Property* prop = GetProperty(key);
    if (prop != NULL && prop->handler != NULL)
    {
        prop->handler = NULL;
        result = true;
    }

    return result;
}

/**
 * \brief Dump all properties
 **/
void PropManager::DumpProperties()
{
    _dbg << "PropManager::DumpProperties() " << std::endl;
    _dbg << "- properties: " << std::endl;

   	std::map<std::string,Property*>::iterator it=m_entries.map.begin();
	for( ; it != m_entries.map.end(); ++it ) 
    {
        string value_str = (it->second->handler == NULL) ? it->second->str_val : " managed by handler ";
        _dbg << " - - key " << it->first << " = " << it->second->str_val << std::endl;
    }
}
