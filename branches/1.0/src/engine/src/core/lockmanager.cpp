/*
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "lockmanager.h"
#include "global.h"

using namespace ft;

Lock LockManager::EMPTY_LOCK(NULL,NULL);
Lock* LockManager::m_updateLock = &(LockManager::EMPTY_LOCK);
