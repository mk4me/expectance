/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "menuitem.h"
using namespace ft;

MenuItem::MenuItem(const std::string& name, int x, int y)
{	
	setName(name);
	MenuItem(x,y);
}

MenuItem::MenuItem(int x, int y)
{
	setPosition(FTVect(x,y,0)); // or setPosition2D
	m_width = 32;
	m_height = 32;
}

void MenuItem::AddObject(MenuItem *menuElement)
{
	m_menuItems.push_back(menuElement);

}

void MenuItem::setWidth(int size)
{
	m_width = size;
}


void MenuItem::setHeight(int size)
{
	m_height = size;
}

MenuItem& MenuItem::setInfoLabel(const std::string& label)
{
	m_label = label;
	return *this;
}

MenuItem& MenuItem::setTexureID(unsigned int textureID)
{
	m_textureID = textureID;
	return *this;
}


const MenuItem* MenuItem::getObject(unsigned int i) const
{
	return m_menuItems[i];
}

const std::vector<MenuItem*> MenuItem::getSubMenu() const
{
	return m_menuItems;
}

const std::string& MenuItem::getMenuName() 
{
	return getName();
}

const std::string& MenuItem::getLabel() const
{
	return m_label;
}

const unsigned int MenuItem::getTextureID() const
{
	return m_textureID;
}

const int MenuItem::getWidth() const
{
	return m_width;
}

const int MenuItem::getHeight() const
{
	return m_height;
}

