/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "application.h"
#include "config.h"
#include "UpdateManager.h"
#include "../avatar/avatarfactory.h"
#include "../scene/visualizationmanager.h"
#include "../timeline/timelinefactory.h"
#include "../utility/debug.h"

using namespace ft;

Application* Application::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::Application (creates it at first call to this method)
 *
 * \return ft::Application * - the only instance of Application
 **/
Application* Application::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("Application::getInstace(): instance of Application created ");
        m_instance = new Application();
    }

    return m_instance;
}

/**
 * \brief Releases all resources related to this Application
 *
 **/
void Application::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
    DBG("Application::getInstace(): instance of Application destroyed  ");
}

/**
 * \brief Loads configuration file for application
 *
 **/
void Application::InitConfig()
{
    DBG("Application::InitConfig().");
    Config::getInstance()->LoadConfigFile();  //with creation of singleton in getInstance()
    Config::TEST_CONFIG();
}

/**
 * \brief Initializes main modules of application
 *
 **/
bool Application::InitModules()
{
    DBG("Application::InitModules().");

    UpdateManager::getInstance()->Init();  //enforced creation of singleton
    AvatarFactory::getInstance();   //enforced creation of singleton
    if (!VisualizationManager::getInstance()->Init()) //enforced creation of singleton
	{
		DBG("VisualizationManager::Init error");
		return 0;
	}
    TimeLineFactory::getInstance(); //enforced creation of singleton

	return true;
}

/**
 * \brief Initializes avatars on scene
 *
 **/
void Application::InitAvatars()
{
    DBG("Application::InitSceneObjects().");

    MovableAvatar* av = dynamic_cast<MovableAvatar*>(Application::getInstance()->CreateAvatarOnScene("cally.cfg", "FirstAvatar"));
    if (av != NULL)  
    {
        av->Init();
        av->Dump();
    }
    //MovableAvatar* av2 = dynamic_cast<MovableAvatar*>(Application::getInstance()->CreateAvatarOnScene("cally.cfg", "SecondAvatar"));
    //if (av != NULL)  
    //{
    //    av2->Init();
    //    av2->Dump();
    //}

  

     ft::UpdateManager::getInstance()->Dump();
}


/**
 * \brief Creates single avatar on the scene
 *
 * \param const std::string & calCoreModel - type of CalModel for avatar that is created
 * \param const std::string & name - name of avatar
 * \return ft::Avatar * - created avatar or NULL if creation failed
 **/
Avatar* Application::CreateAvatarOnScene(const std::string& calCoreModel,const  std::string& name)
{
  Avatar* avatar = ft::AvatarFactory::getInstance()->CreateAvatar(calCoreModel, name);
  if (avatar != NULL)
  {
	  VisualizationManager::getInstance()->AddObject(avatar); 
      UpdateManager::getInstance()->AddUpdateObject(avatar);
  }

  return avatar;
}