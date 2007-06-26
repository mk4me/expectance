/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_MENU_ITEM_H
#define _GEN_MENU_ITEM_H

#include "../core/global.h"
#include "sceneobject.h"


namespace ft
{
	class MenuItem:  public SceneObject
    {
    public:
        MenuItem(const std::string& id, int x, int y); 
		MenuItem(int x, int y); 
	    virtual ~MenuItem(void) { /*empty*/}
        
		bool Init(); //ustaw status i etykiete
		virtual bool Render();
		void Set();
		void SendMessage(); // info jaki button "etykieta uid buttona" + stan to powinien interpretowac i pachowac menager, ktory wysyla takie informacje do systemu lub sam wykonuje akcje
		void AddObject(MenuItem *menuElement);
		const MenuItem* getObject(unsigned int i) const;
		const std::vector<MenuItem*> getSubMenu() const;

		MenuItem& setInfoLabel(const std::string& label);
		MenuItem& setTexureFile(const std::string& filename);
		const std::string& getDescriptionLabel() const;
		const GLint getTextureID() const;

		void setWidth(int size);
		void setHeight(int size);
		const int getWidth() const;
		const int getHeight() const;

    private:
		std::string m_label, m_filename;
		std::vector <MenuItem *> m_menuItems;
		int m_width, m_height;

		
    };
};

#endif //_GEN_MENU_ITEM_H