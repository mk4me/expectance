/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "generatorapp.h"
#include "../control/controlmanager.h"
#include "core/globalmsgsender.h"
#include "../avatar/avatarfactory.h"
#include "genvisualizationmanager.h"
#include "gendebug.h"
#include "scene/transformmanager.h"
#include "core/updatemanager.h"
#include "scene/scenemanager.h"
#include "../physics/cdmanager.h"
#include "../physics/speedcontroller.h"
#include "../physics/magnetcontroller.h"

#include "evolution/factory.h"
#include "evolution/avatar.h"
#include "../timeline/lcsmodifier.h"
#include "../ai/goals/randommovegoal.h"
#include "../ai/goals/limitedareagoal.h"
#include "../ai/constraints/limitedareaconstraint.h"

using namespace ft;

void GeneratorApp::printSomeText()  //to test python iface
{
    std::cout << " Text from GeneratorApp in engine project " << std::endl;
}

// \brief OVERRIDEN from ft::Application
int GeneratorApp::Init()
{
	m_evolutionImpl.Init();
	m_world = ft::Factory::getInstance()->CreateWorld();
    
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
    //AIManager::getInstance()->Init();  //enforced creation of singleton
	CollisionDetectionManager::getInstance();

    UpdateManager::getInstance()->AddUpdateObject(ControlManager::getInstance());
//    UpdateManager::getInstance()->AddUpdateObject(AIManager::getInstance());
	UpdateManager::getInstance()->AddUpdateObject(CollisionDetectionManager::getInstance());
	
	UpdateManager::getInstance()->AddUpdateObject(this);
    return true;
}

bool  GeneratorApp::CreateVisualizationManager()
{
    VisualizationManager* man = GenVisualizationManager::createInstanceFirstTime();  //enforced creation of singleton
    man = dynamic_cast<GenVisualizationManager* >(man);  // additional check of type
    return (man!=NULL);
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
      SceneManager::getInstance()->AddObject(table); 
      UpdateManager::getInstance()->AddUpdateObject(table);
      GlobalMsgSender::getInstance()->AddMsgListener(table);
      CalVector vStartPos(0,0,0);
      table->setPosition(vStartPos);
    }

    table = Cal3DObjectFactory::getMeshObjectFactoryInstance()->CreateMeshObject("table","table02");
    if (table != NULL)
    {
      SceneManager::getInstance()->AddObject(table); 
      UpdateManager::getInstance()->AddUpdateObject(table);
      GlobalMsgSender::getInstance()->AddMsgListener(table);
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

    Avatar* av;
    CalVector vStartPos(0,0,0);
    float x_off = -150, z_off = 0;

    string _nameHelper;
    for (int i=0; i<avatar_number; i++)
    {
		_nameHelper.empty();
		_nameHelper = "Avatar" + StringHelper::itos(i);

		string modelName;
		if (i%2==0)
			modelName = "freebie";
		else
			modelName = "cally";

        av = dynamic_cast<Avatar*>(CreateAvatarOnScene(modelName, _nameHelper));
        if (av != NULL)  
        {
            //av->Init();
            //av->Dump();

            vStartPos.x += x_off;
            vStartPos.z += z_off;

            //av->setStartPosition(vStartPos);
			CalAvatar* avImpl = (CalAvatar*)av->getImplementation();
			avImpl->setPosition(vStartPos);
            avImpl->setGlobalRotationOffset(TransformManager::Y_90);
            ControlManager::getInstance()->AddAvatar(av);
			
			LCSModifier* lcs = new LCSModifier();
			av->AddController(lcs);

			SpeedController* speedCtl = new SpeedController();
			av->AddController(speedCtl);

			av->AddController(avImpl->getFootDetector());

			//av->AddController(new MagnetController()); //TODO: uncomment it if works well


			Goal* goal = new RandomMoveGoal();
			m_world->AddGoal(goal);

			goal = new ChangeDirGoal();
			m_world->AddGoal(goal);

			Rule* rule = new Rule(new LimitedAreaConstraint(), new LimitedAreaGoal());
			m_world->AddRule(rule);

            if (i==0)
            {
                ControlManager::getInstance()->setActiveAvatar(0);
            }
			av->StartSimulation();
        }
    }

    GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_START_SIMULATION), true);


}

/**
 * \brief Sets active 'ThirdPersonCamera' to active avatar if any is set
 */
void GeneratorApp::SetCameraToActiveAvatar()
{
    CalAvatar* av = ControlManager::getInstance()->getActiveAvatarImpl();
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
  //Avatar* avatar = (Avatar*)ft::AvatarFactory::getAvatarFactoryInstance()->CreateMeshObject(calCoreModel, name);

  Avatar* avatar = Factory::getInstance()->createAvatar(name,calCoreModel);
  if (avatar != NULL)
  {
	  CalAvatar* av = static_cast<CalAvatar*>(avatar->getImplementation());
	  SceneManager::getInstance()->AddObject(av); 
      //UpdateManager::getInstance()->AddUpdateObject(avatar);
      //GlobalMsgSender::getInstance()->AddMsgListener(avatar);
	  GlobalMsgSender::getInstance()->AddMsgListener(av);
      //AIManager::getInstance()->AddAvatar((AIAvatar*)av);

	  m_world->AddAvatar(avatar);
  }
  

  return avatar;
}

void GeneratorApp::StartAISimulation()
{
    m_world->StartThinking();
}

void GeneratorApp::OnUpdate(const double elapsedTime)
{
	m_world->Update(elapsedTime);
}
