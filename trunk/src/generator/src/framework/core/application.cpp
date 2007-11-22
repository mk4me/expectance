/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "application.h"
#include "config.h"
#include "UpdateManager.h"
#include "../control/controlmanager.h"
#include "../ai/aimanager.h"
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
        if (Debug::APP>0)
            cout << "Application::getInstace(): instance of Application created " << endl;;

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

    if (Debug::APP>0)
        cout << "Application::getInstace(): instance of Application destroyed  " << endl;
}

/**
 * \brief Loads configuration file for application
 *
 **/
void Application::InitConfig()
{
    if (Debug::APP>0)
        cout << "Application::InitConfig()." << endl;

    Config::getInstance()->LoadConfigFile();  //with creation of singleton in getInstance()
    Config::TEST_CONFIG();
}

/**
 * \brief Initializes main modules of application
 *
 **/
bool Application::InitModules()
{
    if (Debug::APP>0)
        cout << "Application::InitModules()." << endl;

    Debug::LoadLevelsFromConfig();
    UpdateManager::getInstance()->Init();  //enforced creation of singleton
    Cal3DObjectFactory::getMeshObjectFactoryInstance();   //enforced creation of singleton
    AvatarFactory::getAvatarFactoryInstance();   //enforced creation of singleton
    if (!VisualizationManager::getInstance()->Init()) //enforced creation of singleton
	{
        if (Debug::APP>0)
    		cout << "VisualizationManager::Init error" << endl;

		return 0;
	}
    TimeLineFactory::getInstance(); //enforced creation of singleton

    ControlManager::getInstance()->Init(); //enforced creation of singleton
    AIManager::getInstance()->Init();  //enforced creation of singleton

    UpdateManager::getInstance()->AddUpdateObject(ControlManager::getInstance());
	UpdateManager::getInstance()->AddUpdateObject(CameraManager::getInstance()); //synchronize cameramanager from global timer
    UpdateManager::getInstance()->AddUpdateObject(AIManager::getInstance());

	return true;
}

/**
 * \brief Initializes avatars on scene
 *
 **/
void Application::InitAvatars()
{
    if (Debug::APP>0)
        cout << "Application::InitSceneObjects()." << endl;

    int avatar_number = -1;
    if (Config::getInstance()->IsKey("avatars_number"))
    {
        avatar_number = Config::getInstance()->GetIntVal("avatars_number");
    }
    else
    {
        avatar_number = 1;
    }

    AIAvatar* av;
    CalVector vStartPos(0,0,0);
    float x_off = -150, z_off = 0;

    string _nameHelper;
    for (int i=0; i<avatar_number; i++)
    {
		_nameHelper.empty();
		_nameHelper = "Avatar" + StringHelper::itos(i);
        av = dynamic_cast<AIAvatar*>(Application::getInstance()->CreateAvatarOnScene("cally", _nameHelper));
        if (av != NULL)  
        {
            av->Init();
            //av->Dump();

            vStartPos.x += x_off;
            vStartPos.z += z_off;

            //av->setStartPosition(vStartPos);
            av->setPosition(vStartPos);
            ControlManager::getInstance()->AddAvatar(av);
            if (i==0)
            {
                ControlManager::getInstance()->setActiveAvatar(0);
            }
        }
    }

    UpdateManager::getInstance()->SendMessage(new Message(MSG_START_SIMULATION), true);
}

/**
 * \brief Initializes static objects on scene
 *
 **/
void Application::InitStaticObjects()
{
    Cal3DObject *table = Cal3DObjectFactory::getMeshObjectFactoryInstance()->CreateMeshObject("table","table01");
    if (table != NULL)
    {
      VisualizationManager::getInstance()->AddObject(table); 
      UpdateManager::getInstance()->AddUpdateObject(table);
      CalVector vStartPos(0,0,0);
      table->setPosition(vStartPos);
    }

    table = Cal3DObjectFactory::getMeshObjectFactoryInstance()->CreateMeshObject("table","table02");
    if (table != NULL)
    {
      VisualizationManager::getInstance()->AddObject(table); 
      UpdateManager::getInstance()->AddUpdateObject(table);
      CalVector vStartPos(200,0,200);
      table->setPosition(vStartPos);
    }


}

/**
 * \brief Sets active 'ThirdPersonCamera' to active avatar if any is set
 */
void Application::SetCameraToActiveAvatar()
{
    Avatar* av = ControlManager::getInstance()->getActiveAvatar();
    if (av != NULL)
    {
        CameraManager::getInstance()->setCurrentSceneObjectID(av->getID());
    }
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
  Avatar* avatar = (Avatar*)ft::AvatarFactory::getAvatarFactoryInstance()->CreateMeshObject(calCoreModel, name);
  if (avatar != NULL)
  {
	  VisualizationManager::getInstance()->AddObject(avatar); 
      UpdateManager::getInstance()->AddUpdateObject(avatar);
      AIManager::getInstance()->AddAvatar((AIAvatar*)avatar);
  }

  return avatar;
}

void Application::StartAISimulation()
{
    AIManager::getInstance()->StartThinking();  //TODO: (abak 2007-11-18) uncomment it if AI is working correctly
}