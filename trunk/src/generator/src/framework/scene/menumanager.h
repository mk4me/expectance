/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_MENU_MANAGER_H
#define _GEN_MENU_MANAGER_H

#include "../core/controlobject.h"
#include "../core/config.h"
#include "../utility/stringhelper.h"
#include "menuitem.h"
#include "oglcontext.h"



namespace ft
{
	class MenuManager:  public ControlObject
    {
    public:
        MenuManager(void) { /*empty*/}
	    virtual ~MenuManager(void) { /*empty*/}
        
        static MenuManager* getInstance();
        static void DestroyInstance();
		
		bool Init();
		void Configure();		
		void Update(); //wysyla informacje i updateuje kontrolki
		virtual void OnMessage(Message *msg);
		bool Render();
		
		const MenuItem * getMainMenu() const;


    private:
        static MenuManager* m_instance;
		MenuItem* m_mainMenu;
    };
};

#endif //_GEN_MENU_MANAGER_H