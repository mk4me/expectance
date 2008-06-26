/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "generatorapp.h"
#include "../avatar/avatarfactory.h"
#include "genvisualizationmanager.h"
#include "defaultgamehandler.h"
#include "gendebug.h"
#include "../physics/cdmanager.h"
#include "../control/controlmanager.h"
#include "core/updatemanager.h"
//#include "../../footbak/game.h"

using namespace ft;

GeneratorApp::GeneratorApp(void)
{
	setGameHandler(NULL);
}

GeneratorApp::~GeneratorApp(void)
{
}

void GeneratorApp::printSomeText()  //to test python iface
{
    std::cout << " Text from GeneratorApp in engine project " << std::endl;
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
	CollisionDetectionManager::getInstance();

    UpdateManager::getInstance()->AddUpdateObject(ControlManager::getInstance());
	UpdateManager::getInstance()->AddUpdateObject(CollisionDetectionManager::getInstance());
	
	UpdateManager::getInstance()->AddUpdateObject(this);
    return true;
}

/**
 * \brief OVERRIDEN from ft::Application
 *
 **/
void GeneratorApp::InitScene()
{
	Application::InitScene();

	// add here proper gamehandler
	setGameHandler(new DefaultGameHandler());
	//setGameHandler(new Game());
	getGameHandler()->InitScene();
}


bool  GeneratorApp::CreateVisualizationManager()
{
    VisualizationManager* man = GenVisualizationManager::createInstanceFirstTime();  //enforced creation of singleton
    man = dynamic_cast<GenVisualizationManager* >(man);  // additional check of type
    return (man!=NULL);
}

void GeneratorApp::OnUpdate(const double elapsedTime)
{
	if (getGameHandler() != NULL)
		getGameHandler()->OnUpdate(elapsedTime);
}


