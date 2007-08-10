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
	//! A MenuManager class
	/*!
	 *	This class is responsible for construction and management of menu and communication with the rest parts of the system. 
	 *  It creates menu on the base of configuration file. Receives messages from InputManager and sends by ControlManager 
	 *  messages to interested listeners.  
	 */ 
	class MenuManager
    {
    public:
        
		MenuManager(void) { /*empty*/}
	    virtual ~MenuManager(void) { /*empty*/}
        
		//! singleton - Returns the only instance of ft::MenuManager 
        static MenuManager* getInstance();
		//! destroy all resources owned by MenuManager
        static void DestroyInstance();
		//! create menu instance
		/*! creates menu instance in location (x,y) on the screen and configures it according to parameters defined in Config file (application.cfg) */
		bool Init(int x, int y);
		//! locate button index for given coordinates (x,y), return id or -1 otherwise  
		int MenuManager::checkScope(int x, int y);	
		//! render menu 
		/*! renders menu context (hightlights active/selected control and deactivates remaining) */
		bool Render();

		//! check if MenuItem is selected and set temporarily information about it (for rendering purposes)
		void OnMouseButtonDown(int button, int x, int y);
		//! check and activate selected MenuItem
		/*! checks if MenuItem is selected and set permanently information about it (for rendering purposes) 
		 * ControlManager informs all interested listeners about it
		 */
        void OnMouseButtonUp(int button, int x, int y);

		//! get the pointer value of MenuItem indicating on MainMenu (first element in collection) 
		const MenuItem * getMainMenu() const;


    private:
        static MenuManager* m_instance;
		MenuItem* m_mainMenu;
		int m_width, m_height;
		int m_x, m_y;
		int m_avtiveButton,m_pressedButton, m_releasedButon;
		int level; //how deep we are base level = 0
    };
};

#endif //_GEN_MENU_MANAGER_H

//for future use
//void Update(); //wysyla informacje i updaetuje kontrolki
