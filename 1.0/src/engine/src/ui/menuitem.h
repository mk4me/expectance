/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_MENU_ITEM_H
#define _GEN_MENU_ITEM_H

#include "../scene/sceneobject.h"


namespace ft
{
	//! A MenuItem class
	/*!
	 *	This should be a fairly standard implementation of single item used for menu construction.
	 */ 
	class MenuItem: SceneObject 
	{
    public:
		//! default constructor creates menu item in a place given by x,y coordinates
		MenuItem(int x, int y); 
		/*! constructor creates menu item in a place given by x,y coordinates and item name */
		MenuItem(const std::string& name, int x, int y); 
	    virtual ~MenuItem(void) { /*empty*/}
		//! add element to submenu
		/*! Adds MenuItem as submenu collection element */
		void AddObject(MenuItem *menuElement);
		//! set the value of Width 
		void setWidth(int size);
		//! set the value of Height 
		void setHeight(int size);
		//! set the value of label
		MenuItem& setInfoLabel(const std::string& label);
		//! set the value of textureID
		MenuItem& setTexureID(unsigned int textureID);

		//! get the pointer value of object enumerated by index 
		const MenuItem* getObject(unsigned int i) const;		
		//! get the pointer value of submenu
		const std::vector<MenuItem*> getSubMenu() const;
		//! get the string literal of name assigned to MenuItem
		const std::string& getMenuName();
		//! get the string literal of label assigned to MenuItem
		const std::string& getLabel() const;
		//! get the value of texture ID assigned to MenuItem
		const unsigned int getTextureID() const;
		//! get the value of Width
		const int getWidth() const;
		//! get the value of Height
		const int getHeight() const;

    private:
		std::string m_label, m_filename;
		std::vector <MenuItem *> m_menuItems;
		int m_width, m_height;
		unsigned int m_textureID;
    };
}

#endif //_GEN_MENU_ITEM_H

/* for future use:
void SendMsg(); // info jaki button "etykieta uid buttona" + stan to powinien interpretowac i pachowac menager, ktory wysyla takie informacje do systemu lub sam wykonuje akcje
*/
