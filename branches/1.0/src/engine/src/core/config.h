/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONFIG_H
#define _GEN_CONFIG_H


#include "../core/global.h"
#include "../utility/debug.h"
#include "../utility/stlhelper.h"
#include <string>

namespace ft
{
	/**
	 * Class Config: handles settings loaded from configuration file of Generator application
	 **/
    class ENGINE_API Config
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

#endif //_GEN_CONFIG_H
