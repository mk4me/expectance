/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_PROPHANDLER_H
#define _FT_PROPHANDLER_H

#include <string>

namespace ft
{
    static const std::string UNDEFINED_PROPERTY = "<UNDEFINED_PROPERTY>";
    /**
     * Class PropHandler: handles setting/getting specific property
     **/
    class ENGINE_API PropHandler
    {
    public:
        virtual void setPropertyStr(const std::string& key, std::string val) { };
        virtual void setPropertyInt(const std::string& key, int val) { };
        virtual void setPropertyFloat(const std::string& key, float val) { };

        virtual std::string getPropertyStr(const std::string& key) { return UNDEFINED_PROPERTY; }
        virtual int getPropertyInt(const std::string& key) { return -1; }
        virtual float getPropertyFloat(const std::string& key) { return -1; }

    };
}

#endif //_FT_PROPHANDLER_H
