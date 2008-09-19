/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: abak
 */

#include "config.h"
#include "global.h"
#include <iostream>
#include <fstream>

using namespace ft;

Config* Config::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::Config (creates it at first call to this method)
 *
 * \return ft::Config * - the only instance of Config
 **/
Config* Config::getInstance()
{
    if (m_instance == NULL)
    {
        std::cout << "Config::getInstace(): instance of Config created " << std::endl;
        m_instance = new Config();
    }

    return m_instance;
}


/**
 * \brief Releases all resources related to this Config
 *
 **/
void Config::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * \brief Loads  settings from configuration file
 *
 * \return bool - true if settings loaded successfuly, false otherwise
 **/
bool Config::LoadConfigFile()
{

    std::string strFilename(FT_DATA_PATH + "application.cfg");

    std::ifstream file;
    file.open(strFilename.c_str(), std::ios::in);

    if(!file)
    {
        std::cerr << "Failed to open config file " << strFilename.c_str() << std::endl;
        return false;
    }

  int line;
  for(line = 0; ; line++)
  {
    // read the next model configuration line
    std::string strBuffer;
    std::getline(file, strBuffer);

    // stop if we reached the end of file
    if(file.eof())
    {
        break;
    }

    // check if an error happend while reading from the file
    if(!file)
    {
        std::cerr << "Error while reading config file " << strFilename.c_str() << std::endl;
      return false;
    }

    // find the first non-whitespace character
    std::string::size_type pos;
    pos = strBuffer.find_first_not_of(" \t");

    // check for empty lines
    if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

    // check for comment lines
    if(strBuffer[pos] == '#') 
    {
         continue;
    }

    // get the key
    std::string strKey;
    strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
    pos += strKey.size();

    // get the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos);
    if((pos == std::string::npos) || (strBuffer[pos] != '='))
    {
      std::cerr << strFilename.c_str() << "(" << line << "): Invalid syntax." << std::endl;
      return false;
    }

    // find the first non-whitespace character after the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos + 1);

    // get the data
    std::string strData;
    strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);
    
    AddEntry(strKey, strData);
  }

  std::cout << "config file " << strFilename.c_str() << " loaded successfully .." << std::endl;
  return true;
}

/**
 * \brief Reloads settings from confguration file
 *
 * \return bool - true if settings loaded successfuly, false otherwise
 **/
bool Config::ReloadConfigFile()
{
    std::cout << "Config::ReloadConfigFile()" << std::endl;
    ClearEntries();
    return LoadConfigFile();
}

/**
 * \brief Checks if given key is defined in settings
 *
 * \param const char * key - key to check
 * \return bool - true if key is defined, false otherwise
 **/
bool Config::IsKey(const char * key)
{
    bool result = false;
  	std::map<std::string,std::string>::iterator it = m_entries.map.find(key);
    if ( it!=m_entries.map.end()) 
        result = true;

    return result;
}

/**
 * \brief Returns string representation of setting value related to given key
 *
 * \param const char * key - key for setting
 * \return const char * - string representation of setting value
 **/
const char* Config::GetStrVal(const char * key)
{
    const char* result;
  	std::map<std::string,std::string>::iterator it = m_entries.map.find(key);
    if ( it!=m_entries.map.end()) 
        result = it->second.c_str();
    else
        result = "<KEY_NOT_FOUND>";

    return result;
}

/**
 * \brief Returns string representation of setting value related to given key
 *
 * \param const char * key - key for setting
 * \return const char * - string representation of setting value
 **/
const std::string Config::GetStrVal(const std::string& key)
{
	std::string result;
  	std::map<std::string,std::string>::iterator it = m_entries.map.find(key);
    if ( it!=m_entries.map.end()) 
        result = it->second;
    else
        result = "<KEY_NOT_FOUND>";

    return result;
}

/**
 * \brief Returns integer representation of setting value related to given key
 *
 * \param const char * key - key for setting
 * \return const char * - integer representation of setting value
 **/
int Config::GetIntVal(const char * key)
{
    const char * val = GetStrVal(key);
    return atoi (val);
}

/**
 * \brief Returns float representation of setting value related to given key
 *
 * \param const char * key - key for setting
 * \return const char * - integer representation of setting value
 **/
float Config::GetFloatVal(const char * key)
{
    const char * val = GetStrVal(key);
    return static_cast<float>(atof (val));
}

/**
 * \brief Add setting to Config
 *
 * \param std::string key - key for setting
 * \param std::string val - value for setting
 **/
void Config::AddEntry(std::string key, std::string val)
{
	std::map<std::string,std::string>::iterator it = m_entries.map.find(key);

    if ( it!=m_entries.map.end()) {
        std::cerr << "Error: Key " << key << " duplicated in config file .. " << std::endl;
	}

    m_entries.map.insert( std::make_pair( std::string(key), std::string(val) ) );
}

/**
 * \brief Clear all settings from Config
 *
 **/
void Config::ClearEntries()
{
    //TODO: abak:  is there need to delete each string separately
    m_entries.map.clear();
}

/**
 * \brief Method to test Config
 *
 **/
void Config::TEST_CONFIG()
{
    Config::getInstance()->ReloadConfigFile();
    
    std::string param_str = "param_str";
    std::string param_int = "param_int";
    std::string param_bad = "param_bad";

    std::cout << "Read from config <" << param_str << "> val: " << Config::getInstance()->GetStrVal(param_str.c_str()) 
        << std::endl;
    std::cout << "Read from config <" << param_int << "> increased by 1 val: " 
        << Config::getInstance()->GetIntVal(param_int.c_str()) + 1  << std::endl;
    std::cout << "Read from config <" << param_bad << "> val: " << Config::getInstance()->GetStrVal(param_bad.c_str()) 
        << std::endl;
}


