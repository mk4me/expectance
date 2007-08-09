/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "menumanager.h"

using namespace ft;

MenuManager* MenuManager::m_instance = NULL;

/**
 * getInstance: <describe the responsibilities and behavior of this method>
 *
 * \return ft::MenuManager * <describe what is returned if appropriate>
 **/
MenuManager* MenuManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("MenuManager::getInstace(): instance of MenuManager created ");
        m_instance = new MenuManager();
    }

    return m_instance;
}

/**
 * DestroyInstance: <describe the responsibilities and behavior of this method>
 *
 **/
void MenuManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

/**
 * Init: <describe the responsibilities and behavior of this method>
 *
 * \param int x <argument description>
 * \param int y <argument description>
 * \return bool <describe what is returned if appropriate>
 **/
bool MenuManager::Init(int x, int y)
{
	bool final = true;
	static unsigned int listID = 1000; // hope that system textures wont be so much
	vector<string> tokenizer;
	std::string menuOptions;
	GLuint logoTexture;
	m_avtiveButton = -1;
	m_x = x;
	m_y = y;
	//1. read main menu options
	m_mainMenu = new MenuItem(x,y); //main menu w (x,y)
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
				{
					final = false;
					return 0;
				}
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
	m_width = m_mainMenu->getWidth()*m_mainMenu->getSubMenu().size();
	m_height = m_mainMenu->getHeight();
	//3. add option to 
	return final;
}

/**
 * checkScope: <describe the responsibilities and behavior of this method>
 *
 * \param int x <argument description>
 * \param int y <argument description>
 * \return int <describe what is returned if appropriate>
 **/
int MenuManager::checkScope(int x, int y)
{
	float xDiv;
	unsigned int item = m_mainMenu->getWidth();

	if (((x>m_x)&&(x<m_x+m_width))&&((y>0)&&(y<m_height+m_y)))
	{
		xDiv = x/item; //take length
		item = static_cast<int>(xDiv);
		if ((item >= 0)&&(item<= m_mainMenu->getSubMenu().size()-1))
			return item;
		else
			return 0;
	}
	else
		return -1;
}


/**
 * Render: <describe the responsibilities and behavior of this method>
 *
 * \return bool <describe what is returned if appropriate>
 **/
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

		if (m_avtiveButton != -1)
		{
			std::string lab = m_mainMenu->getSubMenu().at(m_avtiveButton)->getLabel();
			glLoadIdentity();
			glTranslatef(-x,-y,0);
			glColor4f(0.7f, 0.7f, 0.7f, 0.3f);
			glRectf(m_avtiveButton*32+2,2,(m_avtiveButton+1)*32,32);
			glLoadIdentity();
			glTranslatef(-x,-y-10,0);
			//get apropriate text
			glEnable(GL_LINE_SMOOTH);
				glColor4f(1,1,1,0.5f);
				OGLContext::getInstance()->OGLWriteBitmap(5,5, 50, lab.c_str());
			glDisable(GL_LINE_SMOOTH);
		}


		glDisable(GL_BLEND);
	glPopMatrix();

	return true;
}


/**
 * getMainMenu: <describe the responsibilities and behavior of this method>
 *
 * \return const ft::MenuItem * <describe what is returned if appropriate>
 **/
const MenuItem* MenuManager::getMainMenu() const
{
	return m_mainMenu;
}

//label from pressed button
/**
 * OnMouseButtonDown: <describe the responsibilities and behavior of this method>
 *
 * \param int button <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void MenuManager::OnMouseButtonDown(int button, int x, int y)
{
	int btn;
	if ((btn=checkScope(x,y))>=0)
	{
		m_pressedButton = btn;
		std::cout <<"Button selected "<< btn << endl;
	}

}

//enter to pressed button (message from it)
/**
 * OnMouseButtonUp: <describe the responsibilities and behavior of this method>
 *
 * \param int button <argument description>
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void MenuManager::OnMouseButtonUp(int button, int x, int y)
{
	int btn;
	if ((btn=checkScope(x,y))>=0)
	{
		m_releasedButon = btn;
		if (m_pressedButton == m_releasedButon)
		{
			m_avtiveButton = m_releasedButon;
			std::string id = m_mainMenu->getSubMenu().at(m_avtiveButton)->getMenuName();
			ControlManager::getInstance()->SendMessage(new Message(MSG_MENU_ITEM_SELECTED, new MessageParam(id)), true);
		}
		std::cout <<"Button released "<< btn << endl;
	}

}
/**
 * OnMouseMove: <describe the responsibilities and behavior of this method>
 *
 * \param int x <argument description>
 * \param int y <argument description>
 **/
void MenuManager::OnMouseMove(int x, int y)
{

}
