/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "controlmanager.h"
#include "../utility/debug.h"

using namespace ft;

ControlManager* ControlManager::m_instance = NULL;

ControlManager* ControlManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("ControlManager::getInstace(): instance of ControlManager created ");
        m_instance = new ControlManager();
    }

    return m_instance;
}

void ControlManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}


void ControlManager::OnUpdate(float elapsedTime)
{
}

void ControlManager::SendMessage(const Message& msg)
{
}

void ControlManager::AddControlObject(ControlObject* object)
{
}

void ControlManager::RemoveControlObject(ControlObject* object)
{
}
