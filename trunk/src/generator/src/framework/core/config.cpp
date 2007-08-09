/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */


#include "config.h"




using namespace ft;

Config* Config::m_instance = NULL;

/**
 * getInstance: <describe the responsibilities and behavior of this method>
 *
 * \return ft::Config * <describe what is returned if appropriate>
 **/
Config* Config::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("Config::getInstace(): instance of Config created ");
        m_instance = new Config();
    }

    return m_instance;
}


/**
 * DestroyInstance: <describe the responsibilities and behavior of this method>
 *
 **/
void Config::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * LoadConfigFile: <describe the responsibilities and behavior of this method>
 *
 * \return bool <describe what is returned if appropriate>
 **/
bool Config::LoadConfigFile()
{

    std::string strFilename(CONFIG_FILE);

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
 * ReloadConfigFile: <describe the responsibilities and behavior of this method>
 *
 * \return bool <describe what is returned if appropriate>
 **/
bool Config::ReloadConfigFile()
{
    DBG("Config::ReloadConfigFile()");
    ClearEntries();
    return LoadConfigFile();
}

/**
 * IsKey: <describe the responsibilities and behavior of this method>
 *
 * \param const char * key <argument description>
 * \return bool <describe what is returned if appropriate>
 **/
bool Config::IsKey(const char * key)
{
    bool result = false;
  	std::map<std::string,std::string>::iterator it = m_entries.find(key);
    if ( it!=m_entries.end()) 
        result = true;

    return result;
}

/**
 * GetStrVal: <describe the responsibilities and behavior of this method>
 *
 * \param const char * key <argument description>
 * \return const char * <describe what is returned if appropriate>
 **/
const char* Config::GetStrVal(const char * key)
{
    const char* result;
  	std::map<std::string,std::string>::iterator it = m_entries.find(key);
    if ( it!=m_entries.end()) 
        result = it->second.c_str();
    else
        result = "<KEY_NOT_FOUND>";

    return result;
}

/**
 * GetStrVal: <describe the responsibilities and behavior of this method>
 *
 * \param const std::string & key <argument description>
 * \return const std::string <describe what is returned if appropriate>
 **/
const std::string Config::GetStrVal(const std::string& key)
{
	std::string result;
  	std::map<std::string,std::string>::iterator it = m_entries.find(key);
    if ( it!=m_entries.end()) 
        result = it->second;
    else
        result = "<KEY_NOT_FOUND>";

    return result;
}

/**
 * GetIntVal: <describe the responsibilities and behavior of this method>
 *
 * \param const char * key <argument description>
 * \return int <describe what is returned if appropriate>
 **/
int Config::GetIntVal(const char * key)
{
    const char * val = GetStrVal(key);
    return atoi (val);
}


/**
 * AddEntry: <describe the responsibilities and behavior of this method>
 *
 * \param std::string key <argument description>
 * \param std::string val <argument description>
 **/
void Config::AddEntry(std::string key, std::string val)
{
	std::map<std::string,std::string>::iterator it = m_entries.find(key);

    if ( it!=m_entries.end()) {
        std::cerr << "Error: Key " << key << " duplicated in config file .. " << std::endl;
	}

    m_entries.insert( std::make_pair( std::string(key), std::string(val) ) );
}

/**
 * ClearEntries: <describe the responsibilities and behavior of this method>
 *
 **/
void Config::ClearEntries()
{
    //TODO: abak:  is there need to delete each string separately
    m_entries.clear();
}

/**
 * TEST_CONFIG: <describe the responsibilities and behavior of this method>
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


