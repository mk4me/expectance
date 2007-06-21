/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_CONFIG_H
#define _GEN_CONFIG_H

#define CONFIG_FILE "data/application.cfg"

#include "global.h"
#include "../utility/debug.h"
#include <string>
#include <map>

//#include <string>

namespace ft
{
    class Config
    {
    public:
        static Config* getInstance();
        static void DestroyInstance();

        bool LoadConfigFile();
        bool ReloadConfigFile();

        bool IsKey(const char * key);
        const char* GetStrVal(const char * key);
        int GetIntVal(const char * key);

        static void TEST_CONFIG();  // to test if Config works properly

    private:
        static Config* m_instance;
        std::map<std::string,std::string> m_entries;

        void AddEntry(std::string key, std::string val);
        void ClearEntries();
    };
};

#endif //_GEN_CONFIG_H
