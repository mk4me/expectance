/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_MENU_MANAGER_H
#define _GEN_MENU_MANAGER_H

#include "../core/controlmanager.h"
#include "../core/config.h"
#include "../utility/stringhelper.h"
#include "../scene/oglcontext.h"
#include "menuitem.h"



namespace ft
{
	class MenuManager
    {
    public:
        MenuManager(void) { /*empty*/}
	    virtual ~MenuManager(void) { /*empty*/}
        
        static MenuManager* getInstance();
        static void DestroyInstance();
		
		bool Init(int x, int y);
		int MenuManager::checkScope(int x, int y);	
		void Update(); //wysyla informacje i updateuje kontrolki
		bool Render();

		void OnMouseButtonDown(int button, int x, int y);
        void OnMouseButtonUp(int button, int x, int y);
        void OnMouseMove(int x, int y);

		const MenuItem * getMainMenu() const;


    private:
        static MenuManager* m_instance;
		MenuItem* m_mainMenu;
		int m_width, m_height;
		int m_x,m_y;
		int m_avtiveButton,m_pressedButton, m_releasedButon;
		int level; //how deep we are base level = 0
    };
};

#endif //_GEN_MENU_MANAGER_H