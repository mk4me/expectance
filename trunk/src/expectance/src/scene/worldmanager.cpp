#include "worldmanager.h"
#include <iostream>

using namespace ft;

WorldManager* WorldManager::m_instance = NULL;
/**
 * \brief Returns the only instance of ft::WorldManager (creates it at first call to this method)
 *
 * \return ft::WorldManager * - the only instance of WorldManager
 **/
WorldManager* WorldManager::getInstance()
{
    if (m_instance == NULL)
    {
        std::cout << "WorldManager::getInstace(): instance of WorldManager created " << std::endl;
        m_instance = new WorldManager();
    }

    return m_instance;
}


/**
 * \brief Releases all resources related to this WorldManager
 *
 **/
void WorldManager::DestroyInstance()
{
    if (m_instance != NULL)
        delete m_instance;
}

bool WorldManager::AddToScene(osg::Group *node)
{
	if (node!=NULL)
	{
		m_matrixTransformPtr->addChild(node);
		return true;
	} else
		return false;
}

bool WorldManager::RemoveFromScene(osg::Group *node)
{
	if (node!=NULL)
	{
		m_matrixTransformPtr->removeChild(node);
		return true;
	} else
		return false;
}