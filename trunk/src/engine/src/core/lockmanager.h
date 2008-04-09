/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_CORE_LOCKMANAGER_H
#define _FT_CORE_LOCKMANAGER_H

#include "lock.h"

namespace ft
{
    /**
     * Class LockManager: keeps locks used for synchronization of multi-threading
     **/
    class ENGINE_API LockManager
    {
    public:
        static void setUpdateLock(Lock *updateLock) { m_updateLock = updateLock; }
        static Lock* getUpdateLock() { return m_updateLock; }

    protected:
        static Lock *m_updateLock;

    };
}


#endif //_FT_CORE_LOCKMANAGER_H
