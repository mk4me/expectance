/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _FT_CONFIG_H
#define _FT_CONFIG_H

#include "../utility/stlhelper.h"
#include <string>

namespace ft
{
	/**
	 * Class Config: handles settings loaded from configuration file of Generator application
	 **/
    class Config
    {
    public:
        static Config* getInstance();
        static void DestroyInstance();

        bool LoadConfigFile();
        bool ReloadConfigFile();

        bool IsKey(const char * key);
        const char* GetStrVal(const char * key);
		const std::string GetStrVal(const std::string& key);
        int GetIntVal(const char * key);
        float GetFloatVal(const char * key);

        static void TEST_CONFIG();  // to test if Config works properly

    protected:
        static Config* m_instance;
    
        STL_WrappedMap<std::string,std::string> m_entries;

        void AddEntry(std::string key, std::string val);
        void ClearEntries();
    };
}

#endif //_FT_CONFIG_H
