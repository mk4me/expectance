/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "menuitem.h"
using namespace ft;
/**
 * MenuItem: Constructor
 *
 * \param const std::string & name MenuItem name
 * \param int x coordinate
 * \param int y coordinate
 **/
MenuItem::MenuItem(const std::string& name, int x, int y)
{	
	setName(name);
	MenuItem(x,y);
}

/**
 * MenuItem: <describe the responsibilities and behavior of this method>
 *
 * \param int x <argument description>
 * \param int y <argument description>
 **/
MenuItem::MenuItem(int x, int y)
{
	setPosition(FTVect(x,y,0)); // albo setPosition2D
	m_width = 32;
	m_height = 32;
}

/**
 * AddObject: <describe the responsibilities and behavior of this method>
 *
 * \param ft::MenuItem * menuElement <argument description>
 **/
void MenuItem::AddObject(MenuItem *menuElement)
{
	m_menuItems.push_back(menuElement);

}

/**
 * getObject: <describe the responsibilities and behavior of this method>
 *
 * \param unsigned int i <argument description>
 * \return const ft::MenuItem * <describe what is returned if appropriate>
 **/
const MenuItem* MenuItem::getObject(unsigned int i) const
{
	return m_menuItems[i];
}

/**
 * getSubMenu: <describe the responsibilities and behavior of this method>
 *
 * \return const std::vector<MenuItem*> <describe what is returned if appropriate>
 **/
const std::vector<MenuItem*> MenuItem::getSubMenu() const
{
	return m_menuItems;
}

/**
 * getMenuName: <describe the responsibilities and behavior of this method>
 *
 * \return const std::string & <describe what is returned if appropriate>
 **/
const std::string& MenuItem::getMenuName() 
{
	return getName();
}


/**
 * setInfoLabel: <describe the responsibilities and behavior of this method>
 *
 * \param const std::string & label <argument description>
 * \return ft::MenuItem & <describe what is returned if appropriate>
 **/
MenuItem& MenuItem::setInfoLabel(const std::string& label)
{
	m_label = label;
	return *this;
}

/**
 * setTexureID: <describe the responsibilities and behavior of this method>
 *
 * \param unsigned int textureID <argument description>
 * \return ft::MenuItem & <describe what is returned if appropriate>
 **/
MenuItem& MenuItem::setTexureID(unsigned int textureID)
{
	m_textureID = textureID;
	return *this;
}

/**
 * getLabel: <describe the responsibilities and behavior of this method>
 *
 * \return const std::string & <describe what is returned if appropriate>
 **/
const std::string& MenuItem::getLabel() const
{
	return m_label;
}
/**
 * getTextureID: <describe the responsibilities and behavior of this method>
 *
 * \return const unsigned int <describe what is returned if appropriate>
 **/
const unsigned int MenuItem::getTextureID() const
{
	return m_textureID;
}


/**
 * setWidth: <describe the responsibilities and behavior of this method>
 *
 * \param int size <argument description>
 **/
void MenuItem::setWidth(int size)
{
	m_width = size;
}

/**
 * setHeight: <describe the responsibilities and behavior of this method>
 *
 * \param int size <argument description>
 **/
void MenuItem::setHeight(int size)
{
	m_height = size;
}

/**
 * getWidth: <describe the responsibilities and behavior of this method>
 *
 * \return const int <describe what is returned if appropriate>
 **/
const int MenuItem::getWidth() const
{
	return m_width;
}

/**
 * getHeight: <describe the responsibilities and behavior of this method>
 *
 * \return const int <describe what is returned if appropriate>
 **/
const int MenuItem::getHeight() const
{
	return m_height;
}

