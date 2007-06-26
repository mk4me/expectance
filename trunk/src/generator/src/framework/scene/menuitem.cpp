/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "menuitem.h"
using namespace ft;
MenuItem::MenuItem(const std::string& id, int x, int y)
{	
	setID(id);
	MenuItem(x,y);
}

MenuItem::MenuItem(int x, int y)
{
	setPosition(FTVect(x,y,0)); // albo setPosition2D
	m_width = 32;
	m_height = 32;

}

bool MenuItem::Render()
{
	return true;
}

void MenuItem::AddObject(MenuItem *menuElement)
{
	m_menuItems.push_back(menuElement);

}

const MenuItem* MenuItem::getObject(unsigned int i) const
{
	return m_menuItems[i];
}
const std::vector<MenuItem*> MenuItem::getSubMenu() const
{
	return m_menuItems;
}


MenuItem& MenuItem::setInfoLabel(const std::string& label)
{
	m_label = label;
	return *this;
}

MenuItem& MenuItem::setTexureFile(const std::string& filename)
{
	m_filename = filename;
	return *this;
}

void MenuItem::setWidth(int size)
{
	m_width = size;
}
void MenuItem::setHeight(int size)
{
	m_height = size;
}
const int MenuItem::getWidth() const
{
	return m_width;
}
const int MenuItem::getHeight() const
{
	return m_height;
}

