/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "menumanager.h"
#include "../core/inputmanager.h"
#include "../core/globalmsgsender.h"

using namespace ft;

MenuManager* MenuManager::m_instance = NULL;

MenuManager* MenuManager::getInstance()
{
    if (m_instance == NULL)
    {
        _dbg << "MenuManager::getInstace(): instance of MenuManager created " << endl;
        m_instance = new MenuManager();
    }

    return m_instance;
}

void MenuManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

bool MenuManager::Init(int x, int y)
{
    InputManager::getInstance()->AddListener(this);

	bool _final = true;
	static unsigned int _listID = 1000; // hope that system textures wont be so much
	vector<string> _tokens;
	std::string _menuOptions;
	GLuint _logoTexture;
	
	m_menuOGL = (Config::getInstance()->GetIntVal("main_menu_visible")==0)? false : true; //set if menu is visible from configuration

	m_avtiveButton = -1;
	m_x = x;
	m_y = y;
	//1. read main menu options
	m_mainMenu = new MenuItem(x,y); //main menu w (x,y)
	_menuOptions = Config::getInstance()->GetStrVal("main_menu");
	_menuOptions = StringHelper::ClearDelimiters(_menuOptions, '(',')');
	_menuOptions = StringHelper::RemoveChar(_menuOptions,' ');
	_tokens = StringHelper::Split(_menuOptions, ",");
	if (_tokens[0] == "<KEY_NOT_FOUND>")
		return false;	
	for (unsigned int i = 0; i <_tokens.size(); i++) //create main menu
	{
		vector<string> _menuParameters;
		std::string _menuOption;
		_menuOption = Config::getInstance()->GetStrVal(_tokens[i]);
		_menuOption = StringHelper::ClearDelimiters(_menuOption, '(', ')');
		_menuParameters = StringHelper::Split(_menuOption,",");
		_menuParameters[1] = StringHelper::RemoveChar(_menuParameters[1],' ');
		if (_menuParameters.size() == 1) 
			continue;
		else
		{
			// 2. dispatch each option
			MenuItem *mi = new MenuItem(_tokens[i],0,0);  // button id
			mi->setInfoLabel(_menuParameters[0]);            // button Information label
			//create texture list
			glNewList(_listID,GL_COMPILE);
				glEnable(GL_TEXTURE_2D);
				if ((_logoTexture = ft::TextureManager::getInstance()-> LoadTexture(FT_TEXTUREPATH + _menuParameters[1]))==0)
				{
					_final = false;
					return 0;
				}
				glBindTexture(GL_TEXTURE_2D,_logoTexture);
				glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
					glTexCoord2f(1.0f, 1.0f); glVertex2i(32, 0);
					glTexCoord2f(1.0f, 0.0f); glVertex2i(32, 32);
					glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 32);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			glEndList();

			mi->setTexureID(_listID);					// button texture filename
			//3. add option to menu
			m_mainMenu->AddObject(mi);
			_listID++;

		}
		_menuParameters.clear();
		_menuOption.clear();

	}
    // count global width and height taking into consideration all options in menu (on the top level)
	m_width = m_mainMenu->getWidth()*m_mainMenu->getSubMenu().size();
	m_height = m_mainMenu->getHeight();	
	return _final;
}

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

bool MenuManager::Render()
{
	if (m_menuOGL)
	{
		int x,y;
		x = OGLContext::getInstance()->getWidth() / 2;
		y = OGLContext::getInstance()->getHeight() / 2;
		unsigned int menuCounter = m_mainMenu->getSubMenu().size();
		int w = menuCounter*32+2;
		int h = 34;
		glPushMatrix();
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glTranslatef(-x,-y,0);
		
			glColor4f(0.9f,0.0f,0.0f,0.6f); // menu color
			glRectf(0,0,w,h);
			//glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
			
			//draw menu
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
			// set active button and text for it
			if (m_avtiveButton != -1) //-1 index is out of scope of menuitems collection 
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
	}
	return true;
}

const MenuItem* MenuManager::getMainMenu() const
{
	return m_mainMenu;
}

void MenuManager::OnMouseButtonDown(int button, int x, int y)
{
  	if (y < 60)
	{

	    int btn;
	    if ((btn=checkScope(x,y))>=0) //label from pressed button
	    {
		    m_pressedButton = btn;
		    _dbg <<"Button selected "<< btn << endl;
	    }
    }
}

void MenuManager::OnKey(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'M':
        hideMenu();
        break;
    default:
        break;
  }
}

void MenuManager::OnMouseButtonUp(int button, int x, int y)
{
    if (y < 60)
    {
	    int btn;
	    if ((btn=checkScope(x,y))>=0)
	    {
		    m_releasedButon = btn;
		    if (m_pressedButton == m_releasedButon) //enter to pressed button (message from it)
		    {
			    m_avtiveButton = m_releasedButon;
			    std::string id = m_mainMenu->getSubMenu().at(m_avtiveButton)->getMenuName();
			    GlobalMsgSender::getInstance()->SendMsg(new Message(MSG_MENU_ITEM_SELECTED, new MessageParam(id)), true);
		    }
		    _dbg <<"Button released "<< btn << endl;
	    }
    }

}

void MenuManager::hideMenu()
{
	m_menuOGL = !m_menuOGL;
}

bool MenuManager::IsMenuVisible()
{
	return m_menuOGL;
}