/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_GENVISUALIZATION_MANAGER_H
#define _GEN_GENVISUALIZATION_MANAGER_H

#include "scene/visualizationmanager.h"


namespace ft
{
	//! A VisualizationManager class for 'generator' application
    class GenVisualizationManager : public VisualizationManager
    {
    public:
        GenVisualizationManager (void) { /*empty*/ }
	    virtual ~GenVisualizationManager (void) { /*empty*/ }

        //! it cerates only instance when called first time
        static VisualizationManager* createInstanceFirstTime();

        virtual bool IsObjectTraceableByCamera(SceneObject* pObj);
    };
};

#endif //_GEN_GENVISUALIZATION_MANAGER_H