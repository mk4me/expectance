/*
 * Copyright (c) 2007, Future Today. All rights reserved.
 * author: abak, mka
 */

#include "../avatar/calavatar.h"
#include "genvisualizationmanager.h"

using namespace ft;

VisualizationManager* GenVisualizationManager::createInstanceFirstTime()
{
    if (m_instance == NULL)
    {
        m_instance = new GenVisualizationManager();
    }
    else
    {
        if (Debug::ERR)
            _dbg << "GenVisualizationManager::createInstanceFirstTime(): instance alraedy created!!!! " << endl;
    }
    return m_instance;
}

bool GenVisualizationManager::IsObjectTraceableByCamera(SceneObject* pObj)
{
    return (dynamic_cast<CalAvatar*>(pObj)!=NULL);
}

