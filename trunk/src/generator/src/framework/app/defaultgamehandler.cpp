/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "defaultgamehandler.h"

#include "scene/scenemanager.h"
#include "core/updatemanager.h"
#include "core/globalmsgsender.h"
#include "scene/transformmanager.h"
#include "../control/controlmanager.h"
#include "../avatar/avatarfactory.h"
#include "gendebug.h"
#include "../physics/speedcontroller.h"
#include "../physics/magnetcontroller.h"
#include "../timeline/lcsmodifier.h"
#include "../ai/goals/randommovegoal.h"
#include "../ai/goals/limitedareagoal.h"
#include "../ai/constraints/limitedareaconstraint.h"

using namespace ft;

void DefaultGameHandler::InitScene()
{
	m_evolutionImpl.Init();
	m_animProvider.Init();
	m_world = ft::Factory::getInstance()->CreateWorld();
    
    InitAvatars();
    SetCameraToActiveAvatar();
    InitStaticObjects();
	InitWorld();

    StartAISimulation();
}


/**
 * \brief Initializes static objects on scene
 *
 **/
void DefaultGameHandler::InitStaticObjects()
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
void DefaultGameHandler::InitAvatars()
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
void DefaultGameHandler::SetCameraToActiveAvatar()
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
Avatar* DefaultGameHandler::CreateAvatarOnScene(const std::string& calCoreModel,const  std::string& name)
{
  Avatar* avatar = Factory::getInstance()->createAvatar(name,calCoreModel);
  if (avatar != NULL)
  {
	  CalAvatar* av = static_cast<CalAvatar*>(avatar->getImplementation());
	  SceneManager::getInstance()->AddObject(av); 
	  GlobalMsgSender::getInstance()->AddMsgListener(av);

	  m_world->AddAvatar(avatar);
  }
  

  return avatar;
}

void DefaultGameHandler::StartAISimulation()
{
    m_world->StartThinking();
}

void DefaultGameHandler::OnUpdate(const double elapsedTime)
{
	m_world->Update(elapsedTime);
}

void DefaultGameHandler::InitWorld()
{
	Goal* goal = new RandomMoveGoal();
	m_world->AddGoal(goal);

	goal = new ChangeDirGoal();
	m_world->AddGoal(goal);

	Rule* rule = new Rule(new LimitedAreaConstraint(), new LimitedAreaGoal());
	m_world->AddRule(rule);


	InitActionsForType("freebie");
	InitActionsForType("cally");
	InitGraphForType("freebie");
	InitGraphForType("cally");

	ControlManager::getInstance()->getActiveAvatar()->ExecuteAction("idle");
}

void DefaultGameHandler::InitActionsForType(const std::string& avatarType)
{
	//---------------- IDLE
	Action* action = new Action(avatarType, "idle");

	Motion* idle = new Motion("idle");
    Motion* timeLineMotion = new Motion("",avatarType, "idle.caf");
	timeLineMotion->setAnimLoop(true);
	timeLineMotion->setInterupting(true);
    idle->AddSubObject(timeLineMotion);
    idle->setBlender(new Blender(0.1f));

	action->setMotion(idle);
	m_world->AddAction(avatarType, action);


	//---------------- WALK
	action = new Action(avatarType, "walk");
	
    Motion* walk = new Motion("walk");
    timeLineMotion = new Motion("", avatarType, "walkloop.caf");
	timeLineMotion->setInterupting(true);
    timeLineMotion->setAnimLoop(true);
    timeLineMotion->setBlender(new Blender(0.2f));
    walk->AddSubObject(timeLineMotion);

	action->setMotion(walk);
	m_world->AddAction(avatarType, action);

	//---------------- RUN
	action = new Action(avatarType, "run");
    Motion* run = new Motion();
	run->setName("run");

    timeLineMotion = new Motion("", avatarType, "runloop.caf");
    timeLineMotion->setAnimLoop(true);
    timeLineMotion->setBlender(new Blender(0.2f));
    run->AddSubObject(timeLineMotion);

	action->setMotion(run);
	m_world->AddAction(avatarType, action);
}

void DefaultGameHandler::InitGraphForType(const std::string& avatarType)
{
	Transition* transition = new Transition("idle","walk");
    Motion* timeLineMotion = new Motion("", avatarType, "walkstart.caf");
    timeLineMotion->setInterupting(true);
    timeLineMotion->setBlender(new Blender(0.25f));
	transition->setMotion(timeLineMotion);
	m_world->AddTransition(avatarType, transition);


	transition = new Transition("walk","idle");
    timeLineMotion = new Motion("", avatarType, "walkstop.caf");
	timeLineMotion->setInterupting(true);
    timeLineMotion->setBlender(new Blender(0.2f));
    transition->setMotion(timeLineMotion);
	m_world->AddTransition(avatarType, transition);

	transition = new Transition("walk","run");
    timeLineMotion = new Motion("", avatarType, "runstart.caf");
	timeLineMotion->setInterupting(true);
    timeLineMotion->setBlender(new Blender(0.2f));
    transition->setMotion(timeLineMotion);
	m_world->AddTransition(avatarType, transition);

	transition = new Transition("run", "walk");
    timeLineMotion = new Motion("", avatarType, "runstop.caf");
	timeLineMotion->setInterupting(true);
    timeLineMotion->setBlender(new Blender(0.2f));
    transition->setMotion(timeLineMotion);
	m_world->AddTransition(avatarType, transition);
}


