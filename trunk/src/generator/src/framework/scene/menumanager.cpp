/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "menumanager.h"
using namespace ft;

MenuManager* MenuManager::m_instance = NULL;

MenuManager* MenuManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("MenuManager::getInstace(): instance of MenuManager created ");
        m_instance = new MenuManager();
    }

    return m_instance;
}

void MenuManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

bool MenuManager::Init()
{
	vector<string> tokenizer;
	std::string menuOptions;
	//1. read main menu options
	m_mainMenu = new MenuItem(0,0); //main menu w (0,0)
	menuOptions = Config::getInstance()->GetStrVal("main_menu");
	menuOptions = StringHelper::ClearBrakets(menuOptions, '(',')');
	menuOptions = StringHelper::RemoveChar(menuOptions,' ');
	tokenizer = StringHelper::Tokens(menuOptions, ",");
	
	for (unsigned int i = 0; i <tokenizer.size(); i++) //create main menu
	{
		vector<string> menuParameters;
		std::string menuOption;
		menuOption = Config::getInstance()->GetStrVal(tokenizer[i]);
		menuOption = StringHelper::ClearBrakets(menuOption, '(', ')');
		menuParameters = StringHelper::Tokens(menuOption,",");
		if (menuParameters.size() == 1) 
			continue;
		else
		{
			MenuItem *mi = new MenuItem(tokenizer[i],0,0);  // button id
			mi->setInfoLabel(menuParameters[0]);            // button Information label
			mi->setTexureFile(menuParameters[1]);			// button texture filename
			m_mainMenu->AddObject(mi);
		}
		menuParameters.clear();
		menuOption.clear();

	}
	//2. dispatch each option

	//3. add option to 
	return true;
}

void MenuManager::Configure()
{
	// trzeba odczytaæ dane z pliku tekstowego ( wszystkie z przedrostkiem menu) i utworzyæ kolekcjê obiektów typu button (musi miec ja dostêpn¹ publicznie)

}



void MenuManager::OnMessage(Message* msg)
{
    if (DEBUG_MESSAGES)
        std::cout << "MenuManager<" << getID() << "> received message: " << Message::_GET_MSG_NAME(msg->getType()) << std::endl;

 //   if (msg->getType() == MSG_PROPERTY_LOD) 
 //   {
 //       SetLodLevel(msg->getParam()->getFloatValue());
 //   } 
 //   else if (msg->getType() == MSG_PROPERTY_RENDER_METHOD)
 //   {
 //       ChangeRenderMethod();
 //   }
	//else if (msg->getType() == MSG_PROPERTY_SHADOW)
	//{
	//	ChangeShadow();
	//}
}


bool MenuManager::Render()
{
	vector<MenuItem*> mi;
	unsigned int elNo = m_mainMenu->getSubMenu().size();
	int w = elNo*40;
	int h = 30;



	glPushMatrix();
		glEnable(GL_BLEND);
		glColor4f(1,1,1,0.2f);
		glRectf(0,0,w,h);
		for (unsigned int i = 0; i < elNo; i++)
		{
			glColor4f(0.9f, 0.9f, 0.9f, 0.5f);
			glRectf(i*42+1,1,(i+1)*42,42);
		}

		glDisable(GL_BLEND);
	glPopMatrix();

	return true;
}


const MenuItem* MenuManager::getMainMenu() const
{
	return m_mainMenu;
}