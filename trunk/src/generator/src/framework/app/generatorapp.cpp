/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "generatorapp.h"
#include "../control/controlmanager.h"
#include "../ai/aimanager.h"
#include "../avatar/avatarfactory.h"
#include "gendebug.h"


using namespace ft;

// \brief OVERRIDEN from ft::Application
int GeneratorApp::Init()
{

    cout << GenDebug::ERR << " hej " << endl;

    int result = Application::Init();
 
    StartAISimulation();
    return result;
}

/**
 * \brief Initializes main modules of application 
 *  OVERRIDEN from ft::Application
 **/
bool GeneratorApp::InitModules()
{
    GenDebug::LoadLevelsFromConfig(); //additional flags from GenDebug

    if (!Application::InitModules())
        return false;

    AvatarFactory::getAvatarFactoryInstance();   //enforced creation of singleton
    ControlManager::getInstance()->Init(); //enforced creation of singleton
    AIManager::getInstance()->Init();  //enforced creation of singleton

    UpdateManager::getInstance()->AddUpdateObject(ControlManager::getInstance());
    UpdateManager::getInstance()->AddUpdateObject(AIManager::getInstance());

    return true;
}


/**
 * \brief OVERRIDEN from ft::Application
 *
 **/
void GeneratorApp::InitObjects()
{
    Application::InitObjects();

    InitAvatars();
    SetCameraToActiveAvatar();
    InitStaticObjects();
}

/**
 * \brief Initializes static objects on scene
 *
 **/
void GeneratorApp::InitStaticObjects()
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
 * \brief Initializes avatars on scene
 *
 **/
void GeneratorApp::InitAvatars()
{
    if (GenDebug::APP>0)
        _dbg << "Application::InitSceneObjects()." << endl;

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
        av = dynamic_cast<AIAvatar*>(CreateAvatarOnScene("cally", _nameHelper));
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
 * \brief Sets active 'ThirdPersonCamera' to active avatar if any is set
 */
void GeneratorApp::SetCameraToActiveAvatar()
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
Avatar* GeneratorApp::CreateAvatarOnScene(const std::string& calCoreModel,const  std::string& name)
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

void GeneratorApp::StartAISimulation()
{
    AIManager::getInstance()->StartThinking();  //TODO: (abak 2007-11-18) uncomment it if AI is working correctly
}