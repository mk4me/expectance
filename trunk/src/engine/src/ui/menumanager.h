/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_MENU_MANAGER_H
#define _GEN_MENU_MANAGER_H

#include "../core/UpdateManager.h"
#include "../core/config.h"
#include "../utility/stringhelper.h"
#include "../scene/oglcontext.h"
#include "menuitem.h"
#include "../core/inputlistener.h"



namespace ft
{
	//! A MenuManager class
	/*!
	 *	This class is responsible for construction and management of menu and   
	 *  communication with the other parts of the system in the scope of rendering. 
	 *  It creates menu on the base of configuration file. Receives messages from 
	 *  InputManager and sends by UpdateManager messages to interested listeners.  
	 */ 
    class MenuManager : public InputListener
    {
    public:
        
		MenuManager(void) { /*empty*/}
	    virtual ~MenuManager(void) { /*empty*/}
        
		//! singleton - Returns the only instance of ft::MenuManager 
        static MenuManager* getInstance();
		//! destroy all resources owned by MenuManager
        static void DestroyInstance();
		//! create menu instance
		/*! creates menu instance in location (x,y) on the screen and 
		  * configures it according to parameters defined in Config 
		  * file (application.cfg) */
		bool Init(int x, int y);
		//! locate button index for given coordinates (x,y), return id or -1 otherwise  
		int MenuManager::checkScope(int x, int y);	
		//! render menu 
		/*! renders menu context (hightlights active/selected control and deactivates remaining) */
		bool Render();

        void OnKey(unsigned char key, int x, int y);

		//! check if MenuItem is selected and set temporarily information about it (for rendering purposes)
		void OnMouseButtonDown(int button, int x, int y);
		//! check and activate selected MenuItem
		/*! checks if MenuItem is selected and set permanently information about it (for rendering purposes) 
		 * UpdateManager informs all interested listeners about it
		 */
        void OnMouseButtonUp(int button, int x, int y);

		//! get the pointer value of MenuItem indicating on MainMenu (first element in collection) 
		const MenuItem * getMainMenu() const;

		//! throw out graphics menu from rendering pipeline
		void hideMenu();
		//! inform about Menu visibility
		bool IsMenuVisible();

    private:
        static MenuManager* m_instance;
		MenuItem* m_mainMenu;
		int m_width, m_height;
		int m_x, m_y;
		int m_avtiveButton,m_pressedButton, m_releasedButon;
		bool m_menuOGL;
		int level; //how deep we are base level = 0 - for future use
    };
};

#endif //_GEN_MENU_MANAGER_H

//for future use
//void Update(); //wysyla informacje i updaetuje kontrolki
