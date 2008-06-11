/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_PROPMAN_H
#define _FT_PROPMAN_H

#include "../core/global.h"
#include "../utility/debug.h"
#include "../utility/stlhelper.h"
#include "property.h"
#include <string>

namespace ft
{
    /**
     * Class PropManager: manages properties handling and broadcasting them to handlers
     **/
    class ENGINE_API PropManager
    {
    public:
        static PropManager* getInstance();
        static void DestroyInstance();

        void setPropertyStr(const std::string& key, std::string val);
        void setPropertyInt(const std::string& key, int val);
        void setPropertyFloat(const std::string& key, float val);

        std::string getPropertyStr(const std::string& key);
        int getPropertyInt(const std::string& key);
        float getPropertyFloat(const std::string& key);

        bool RemoveProperty(const std::string& key);


        bool AddHandler(const std::string& key, PropHandler* handler);
        bool RemoveHandler(const std::string& key);

        void DumpProperties();

        protected:
            static PropManager* m_instance;

            STL_WrappedMap<std::string,Property*> m_entries;

            void ClearEntries();
            Property* GetProperty(const std::string& key);
    };
}

#endif //_FT_PROPMAN_H
