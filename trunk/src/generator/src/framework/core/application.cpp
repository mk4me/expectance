/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "application.h"
#include "config.h"
#include "controlmanager.h"
#include "../avatar/avatarfactory.h"
#include "../scene/scenemanager.h"
#include "../utility/debug.h"

using namespace ft;

Application* Application::m_instance = NULL;

Application* Application::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("Application::getInstace(): instance of Application created ");
        m_instance = new Application();
    }

    return m_instance;
}

void Application::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
    DBG("Application::getInstace(): instance of Application destroyed  ");
}

void Application::InitConfig()
{
    DBG("Application::InitConfig().");
    Config::getInstance()->LoadConfigFile();  //with creation of singleton in getInstance()
    Config::TEST_CONFIG();


}

void Application::InitModules()
{
    DBG("Application::InitModules().");

    ControlManager::getInstance();  //enforced creation of singleton
    AvatarFactory::getInstance();  //enforced creation of singleton
    SceneManager::getInstance();  //enforced creation of singleton
}

void Application::InitSceneObjects()
{
    DBG("Application::InitSceneObjects().");
}
