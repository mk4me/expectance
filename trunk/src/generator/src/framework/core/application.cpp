/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "application.h"
#include "config.h"
#include "UpdateManager.h"
#include "../control/controlmanager.h"
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

    ControlManager::getInstance()->Init(); //enforced creation of singleton
    UpdateManager::getInstance()->AddUpdateObject(ControlManager::getInstance());
	UpdateManager::getInstance()->AddUpdateObject(CameraManager::getInstance()); //synchronize cameramanager from global timer
	return true;
}

/**
 * \brief Initializes avatars on scene
 *
 **/
void Application::InitAvatars()
{
    DBG("Application::InitSceneObjects().");

    int avatar_number = -1;
    if (Config::getInstance()->IsKey("avatars_number"))
    {
        avatar_number = Config::getInstance()->GetIntVal("avatars_number");
    }
    else
    {
        avatar_number = 1;
    }

    ControlAvatar* av;
    CalVector vStartPos(0,0,0);
    float x_off = -150, z_off = 0;

    std::string avNamePostfix;
    for (int i=0; i<avatar_number; i++)
    {
        avNamePostfix = i;
        av = dynamic_cast<ControlAvatar*>(Application::getInstance()->CreateAvatarOnScene("cally.cfg", "Avatar" + avNamePostfix));
        if (av != NULL)  
        {
            av->Init();
            //av->Dump();

            if (i>0)
            {
                vStartPos.x += x_off;
                vStartPos.z += z_off;
            }

            av->setStartPosition(vStartPos);
            ControlManager::getInstance()->AddControlAvatar(av);
            if (i==0)
            {
                ControlManager::getInstance()->setActiveAvatar(0);
            }
        }
    }

     ft::UpdateManager::getInstance()->Dump();

     UpdateManager::getInstance()->SendMessage(new Message(MSG_START_SIMULATION), true);
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