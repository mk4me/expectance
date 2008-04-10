/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_CORE_LOCK_H
#define _FT_CORE_LOCK_H

//#define FT_SYNC_DISABLED

#include "../core/platform.h"

namespace ft
{
    /**
     * Class Lock: represents abstract method for synchronization of multi-threading
     **/
    class ENGINE_API Lock
    {
    public:
        Lock(void (*enterImplFunc)(), void (*leaveImplFunc)() ) { 
                m_enterImplFunc = enterImplFunc; 
                m_leaveImplFunc = leaveImplFunc;
            }

        ~Lock() {}

        void enter() { 
            #ifndef FT_SYNC_DISABLED
                m_enterImplFunc(); 
            #endif
        }
        
        void leave() { 
            #ifndef FT_SYNC_DISABLED
                m_leaveImplFunc(); 
            #endif
        }

    protected:
            void (*m_enterImplFunc)();
            void (*m_leaveImplFunc)();
    };
}

#endif //_FT_CORE_LOCK_H
