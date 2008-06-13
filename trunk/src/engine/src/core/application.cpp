/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "application.h"
#include "globalmsgsender.h"
#include "inputmanager.h"
#include "propmanager.h"
#include "../core/updatemanager.h"
#include "../scene/cal3dobjectfactory.h"
#include "../scene/transformmanager.h"
#include "../scene/visualizationmanager.h"
#include "../scene/scenemanager.h"
#include "../scene/cameramanager.h"
#include "../scene/oglcontext.h"

using namespace ft;

/**
 * \brief Loads configuration file for application
 *
 **/
void Application::InitConfig()
{
    if (Debug::APP>0)
        _dbg << "Application::InitConfig()." << endl;

    Config::getInstance()->LoadConfigFile();  //with creation of singleton in getInstance()
    Config::TEST_CONFIG();
}

/**
 * \brief Inits application
 * \return int - 0 if initialization succeed, -1 otherwise
 **/
int Application::Init()
{
    if (!InitModules())
	{
        cout << Debug::ERR_STR << "Application::InitModules error" << endl;
		return -1;
	}

	InitScene();
    InputManager::getInstance()->AddListener(this);

    return 0;
}

/**
 * \brief Initializes main modules of application
 *
 **/
bool Application::InitModules()
{
    if (Debug::APP>0)
        _dbg << "Application::InitModules()." << endl;

    Debug::InitDebug();
    PropManager::getInstance();  //enforced creation of singleton
    PropManager::getInstance()->setPropertyStr("TestProp","test_value");
    InputManager::getInstance();  //enforced creation of singleton
    UpdateManager::getInstance()->Init();  //enforced creation of singleton
    GlobalMsgSender::getInstance();  //enforced creation of singleton
	SceneManager::getInstance();	 // enforced creation of singleton
	Cal3DObjectFactory::getMeshObjectFactoryInstance();   //enforced creation of singleton
    TransformManager::getInstance(); //enforced creation of singleton
    CreateVisualizationManager();  //enforced creation of singleton
    if (!VisualizationManager::getInstance()->Init()) 
	{
        if (Debug::APP>0)
    		_dbg << "VisualizationManager::Init error" << endl;

		return false;
	}

	UpdateManager::getInstance()->AddUpdateObject(CameraManager::getInstance()); // synchronize cameramanager from global timer
//	UpdateManager::getInstance()->AddUpdateObject(SceneManager::getInstance());  // synchronize scenegraph elements updating from global timer

	return true;
}

/**
 * \brief Initializes objects on scene
 *
 **/
void Application::InitScene()
{
    //add objects to scene here
}


bool Application::CreateVisualizationManager()
{
    VisualizationManager* man = VisualizationManager::createInstanceFirstTime();  //enforced creation of singleton
    return (man!=NULL);
}

/*-----  -----*/
/**
 * \brief Handles a key event from keyboard
 *
 * \param unsigned char key - pressed key
 * \param int x - x coordinate on the application window
 * \param int y - y coordinate on the application window
 **/
void Application::OnKey(unsigned char key, int x, int y)
{
 
  switch(key)
  {
    case 13: //enter
//      GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_START_SIMULATION), true);
      break;
    case 'z':
    case 'Z': 
      GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_RESTART_SIMULATION), true);
      break;
    case 't':
      GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_TEST), true);
       break;     
    case 'i':
      GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_DUMP_STATE), true);
       break;     
    // test for quit event
    case 27:
    case 'Q':
      exit(0);
      break;
    case ' ':
      GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_CONTROL_PAUSE), true);
      break;
    case '*':
      UpdateManager::getInstance()->setTimeScale(  UpdateManager::getInstance()->getTimeScale() * 1.1f);
      break;
    case '/':
      UpdateManager::getInstance()->setTimeScale(  UpdateManager::getInstance()->getTimeScale() / 1.1f);
      break;
    case 'R':
      GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_PROPERTY_RENDER_METHOD), true);  
      break;
	case 'S':
      GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_PROPERTY_SHADOW), true);  
  	  break;
	case 'F':
		OGLContext::getInstance()->changeFloorType();
      break;
	case 'L':
		OGLContext::getInstance()->hideFTLogo();
      break;
	case 'D':
		OGLContext::getInstance()->DATA_VIEWPORT ^=1;
    default:
      if((key >= '0') && (key <= '9'))
      {
        float lod = (key == '0') ? 1.0f : (key - '0') * 0.1f;
        GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_PROPERTY_LOD, new MessageParam(lod)), true);
      }
      break;
  }
}


