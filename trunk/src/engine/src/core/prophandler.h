/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_PROPHANDLER_H
#define _FT_PROPHANDLER_H

#include <string>

namespace ft
{
    /**
     * Class PropHandler: handles setting/getting specific property
     **/
    class ENGINE_API PropHandler
    {
    public:
        virtual void setPropertyStr(const std::string& key, std::string val) = 0;
        virtual void setPropertyInt(const std::string& key, int val) = 0;
        virtual void setPropertyFloat(const std::string& key, float val) = 0;

        virtual std::string getPropertyStr(const std::string& key) = 0;
        virtual int getPropertyInt(const std::string& key) = 0;
        virtual float getPropertyFloat(const std::string& key) = 0;

    };
}

#endif //_FT_PROPHANDLER_H