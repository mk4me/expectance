/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "game.h"

#include "scene/scenemanager.h"
#include "core/updatemanager.h"
#include "core/globalmsgsender.h"
#include "scene/transformmanager.h"
#include "../framework/control/controlmanager.h"
#include "../framework/avatar/avatarfactory.h"
#include "field/ballfactory.h"
#include "../framework/app/gendebug.h"
#include "../framework/physics/speedcontroller.h"
#include "../framework/physics/magnetcontroller.h"
#include "../framework/timeline/lcsmodifier.h"
#include "../framework/ai/goals/randommovegoal.h"
#include "../framework/ai/goals/limitedareagoal.h"
#include "../framework/ai/constraints/limitedareaconstraint.h"
#include "core/inputmanager.h"
#include "player.h"
#include "restposconstraint.h"
#include "restposgoal.h"
#include "evolution/dataprovider.h"

using namespace ft;

Game::Game()
{
	m_field = NULL;
	m_teams[LEFT_TEAM] = NULL;
	m_teams[RIGHT_TEAM] = NULL;
}
void Game::InitScene()
{
	InputManager::getInstance()->AddListener(this);

	m_evolutionImpl.Init();
	m_animProvider.Init();

	m_world = ft::Factory::getInstance()->CreateWorld();
    
	m_field = new Field();
	m_field->setCorners(CalVector(-100,0,-100), CalVector(100,0,-100) , CalVector(100,0,100), CalVector(-100,0,100));
	m_field->InitFieldObjects();
	m_field->InitLines();
	
	m_teams[LEFT_TEAM] = new Team();
//	m_teams[RIGHT_TEAM] = new Team();

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
void Game::InitStaticObjects()
{
	// LEFT GATE --------
    Cal3DObject *table = Cal3DObjectFactory::getMeshObjectFactoryInstance()->CreateMeshObject("table","table01");
    if (table != NULL)
    {
      SceneManager::getInstance()->AddObject(table); 
	  UpdateManager::getInstance()->AddUpdateObject(table);
      CalVector vStartPos = m_field->getLeftGatePos();
	  vStartPos  -= CalVector(0,0,100);
      table->setPosition(vStartPos);
    }

    table = Cal3DObjectFactory::getMeshObjectFactoryInstance()->CreateMeshObject("table","table02");
    if (table != NULL)
    {
      SceneManager::getInstance()->AddObject(table); 
	  UpdateManager::getInstance()->AddUpdateObject(table);
      CalVector vStartPos = m_field->getLeftGatePos();
	  vStartPos  -= CalVector(0,0,-100);
      table->setPosition(vStartPos);
    }

	// RIGHT GATE --------
	table = Cal3DObjectFactory::getMeshObjectFactoryInstance()->CreateMeshObject("table","table03");
    if (table != NULL)
    {
      SceneManager::getInstance()->AddObject(table); 
	  UpdateManager::getInstance()->AddUpdateObject(table);
      CalVector vStartPos = m_field->getRightGatePos();
	  vStartPos  -= CalVector(0,0,100);
      table->setPosition(vStartPos);
    }

    table = Cal3DObjectFactory::getMeshObjectFactoryInstance()->CreateMeshObject("table","table04");
    if (table != NULL)
    {
      SceneManager::getInstance()->AddObject(table); 
	  UpdateManager::getInstance()->AddUpdateObject(table);
      CalVector vStartPos = m_field->getRightGatePos();
	  vStartPos  -= CalVector(0,0,-100);
      table->setPosition(vStartPos);
    }

	Ball* ball = (Ball*)BallFactory::getBallFactoryInstance()->CreateMeshObject("ball","ball01");
    if (ball != NULL)
    {
      SceneManager::getInstance()->AddObject(ball); 
	  UpdateManager::getInstance()->AddUpdateObject(ball);
      
	  CalVector vStartPos  = Field::WEST * 900;
	  vStartPos.y = ball->getRestY();
      ball->setPosition(vStartPos);
	  m_field->setBall(ball);
    }

}
/**
 * \brief Initializes avatars on scene
 *
 **/
void Game::InitAvatars()
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
//		if (i%2==0)
			modelName = "freebie";
//		else
//			modelName = "cally";

        av = dynamic_cast<Avatar*>(CreateAvatarOnScene(modelName, _nameHelper));
        if (av != NULL)  
        {
			m_teams[LEFT_TEAM]->AddPlayer(av);
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

			//Goal* goal = new RandomMoveGoal();
			//m_world->AddGoal(goal);

			//goal = new ChangeDirGoal();
			//m_world->AddGoal(goal);

			//Rule* rule = new Rule(new LimitedAreaConstraint(), new LimitedAreaGoal());
			//m_world->AddRule(rule);

            if (i==0)
            {
                ControlManager::getInstance()->setActiveAvatar(0);
            }
			av->StartSimulation();
			av->ExecuteAction("idle");
        }
    }

    GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_START_SIMULATION), true);
}

/**
 * \brief Sets active 'ThirdPersonCamera' to active avatar if any is set
 */
void Game::SetCameraToActiveAvatar()
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
Avatar* Game::CreateAvatarOnScene(const std::string& calCoreModel,const  std::string& name)
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

void Game::StartAISimulation()
{
    m_world->StartThinking();
}

void Game::OnUpdate(const double elapsedTime)
{
	m_world->Update(elapsedTime);
	if (m_teams[LEFT_TEAM] != NULL)
	{
		m_teams[LEFT_TEAM]->Update(elapsedTime, m_field);
	}
}

void Game::InitWorld()
{
	Rule* rule = new Rule(new RestPosConstraint(), new RestPosGoal());
	m_world->AddRule(rule);

	m_world->LoadDataForType("data/motions.xml", "cally");
	m_world->LoadDataForType("data/motions.xml", "freebie");

	InitActionsForType("freebie");
	InitActionsForType("cally");
	InitGraphForType("freebie");
	InitGraphForType("cally");
	
	ControlManager::getInstance()->getActiveAvatar()->ExecuteAction("idle");
}

void Game::InitActionsForType(const std::string& avatarType)
{
	//---------------- IDLE
	Action* action = new Action(avatarType, "idle");

	Motion* idle = DataProvider::getInstance()->getMotion(avatarType, "idle");
	action->setMotion(idle);
	m_world->AddAction(avatarType, action);


	//---------------- WALK
	action = new Action(avatarType, "walk");
	Motion* walk = DataProvider::getInstance()->getMotion(avatarType, "walk");
	action->setMotion(walk);
	m_world->AddAction(avatarType, action);

	//---------------- RUN
	action = new Action(avatarType, "run");
    Motion* run = DataProvider::getInstance()->getMotion(avatarType, "run");
	action->setMotion(run);
	m_world->AddAction(avatarType, action);
}

void Game::InitGraphForType(const std::string& avatarType)
{
	Transition* transition = new Transition("idle","walk");
	Motion* walkstart = DataProvider::getInstance()->getMotion(avatarType, "walkstart");
	transition->setMotion(walkstart);
	m_world->AddTransition(avatarType, transition);


	transition = new Transition("walk","idle");
	Motion* walkstop = DataProvider::getInstance()->getMotion(avatarType, "walkstop");    
	transition->setMotion(walkstop);
	m_world->AddTransition(avatarType, transition);

	transition = new Transition("walk","run");
	Motion* runstart = DataProvider::getInstance()->getMotion(avatarType, "runstart");    
    transition->setMotion(runstart);
	m_world->AddTransition(avatarType, transition);

	transition = new Transition("run", "walk");
	Motion* runstop = DataProvider::getInstance()->getMotion(avatarType, "runstop");    
    transition->setMotion(runstop);
	m_world->AddTransition(avatarType, transition);
}



void Game::OnSpecial(int key, int x, int y)
{
 CalVector vStartPos;

  switch(key) 
  {
  case GLUT_KEY_F5:
	  m_world->DumpActions();
	  break;
  case GLUT_KEY_HOME:
	  vStartPos  = Field::WEST * 900;
	  vStartPos.y = m_field->getBall()->getRestY();
      m_field->getBall()->setPosition(vStartPos);
       break;
  case GLUT_KEY_PAGE_UP:
	  m_field->getBall()->Move(Field::EAST*900, 1000, 500);
        break;
  case GLUT_KEY_PAGE_DOWN:
  		m_field->getBall()->Move(Field::EAST*900, 1000, 0);
        break;
  case GLUT_KEY_END:
  		m_field->getBall()->MoveByLob(Field::EAST*900, m_field->getBall()->CalculateSpeed(Field::EAST*900, 1));
        break;

  case GLUT_KEY_F6:
	  //m_world->DumpAvatars();
	  cout << "XML will be loaded " << endl;
//	  m_world->LoadDataForType("dane.xml", "cally");
//	  m_world->LoadDataForType("dane.xml", "freebie");
	  break;

  default:
	  break;
  }
}

