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
	unsigned int listID = 1000;
	vector<string> tokenizer;
	std::string menuOptions;
	GLuint logoTexture;
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
		menuParameters[1] = StringHelper::RemoveChar(menuParameters[1],' ');
		if (menuParameters.size() == 1) 
			continue;
		else
		{
			MenuItem *mi = new MenuItem(tokenizer[i],0,0);  // button id
			mi->setInfoLabel(menuParameters[0]);            // button Information label
			//create texture list
			glNewList(listID,GL_COMPILE);
				glEnable(GL_TEXTURE_2D);
				if ((logoTexture = ft::TextureManager::getInstance()-> LoadTexture(FT_TEXTUREPATH + menuParameters[1]))==0)
					return 1;
					glBindTexture(GL_TEXTURE_2D,logoTexture);
					glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
					glBegin(GL_QUADS);
						glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
						glTexCoord2f(1.0f, 1.0f); glVertex2i(32, 0);
						glTexCoord2f(1.0f, 0.0f); glVertex2i(32, 32);
						glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 32);
					glEnd();
				glDisable(GL_TEXTURE_2D);
			glEndList();

			mi->setTexureID(listID);					// button texture filename
			m_mainMenu->AddObject(mi);
			listID++;

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
	int x,y;
	x = OGLContext::getInstance()->getWidth() / 2;
	y = OGLContext::getInstance()->getHeight() / 2;
	vector<MenuItem*> mi;
	unsigned int menuCounter = m_mainMenu->getSubMenu().size();
	int w = menuCounter*32+2;
	int h = 34;
	glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glTranslatef(-x,-y,0);
	
		glColor4f(0.9f,0.0f,0.0f,0.6f);
		glRectf(0,0,w,h);
		//glColor4f(0.5f, 0.5f, 0.5f, 0.5f);

		for (unsigned int i = 0; i < menuCounter; i++)
		{
			if (i==0) 
				glTranslatef(0,0,0);
			else
				glTranslatef(32,0,0);
			
			glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
			glRectf(2,2,32,32);
			glPushMatrix();
			 glCallList(m_mainMenu->getSubMenu().at(i)->getTextureID());
			glPopMatrix();
		}

		glLoadIdentity();
		glTranslatef(-x,-y-10,0);
		//get apropriate text
		glEnable(GL_LINE_SMOOTH);
			glColor4f(1,1,1,0.5f);
			OGLContext::getInstance()->OGLWriteBitmap(5,5, 50, "Menu text is going to be here.");
		glDisable(GL_LINE_SMOOTH);


		glDisable(GL_BLEND);
	glPopMatrix();

	return true;
}


const MenuItem* MenuManager::getMainMenu() const
{
	return m_mainMenu;
}