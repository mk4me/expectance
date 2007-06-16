/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "scenemanager.h"
#include "../utility/debug.h"

using namespace ft;

SceneManager* SceneManager::m_instance = NULL;

SceneManager* SceneManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("SceneManager::getInstace(): instance of SceneManager created ");
        m_instance = new SceneManager();
    }

    return m_instance;
}

void SceneManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}