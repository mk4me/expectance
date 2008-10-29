/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: mka
 */
#ifndef _FT_WORLD_MANAGER
#define _FT_WORLD_MANAGER

#include <osg/Node>
#include <osg/MatrixTransform>

namespace ft 
{
	class WorldManager
	{
	public:
		WorldManager(void){m_matrixTransformPtr = NULL;};
		virtual ~WorldManager(void){};
        
        static WorldManager* getInstance();
        static void DestroyInstance();

		void Init(osg::MatrixTransform* worldMatrix) {m_matrixTransformPtr = worldMatrix;};

		bool AddToScene(osg::Group* node);
		bool RemoveFromScene(osg::Group* node);

    private:
		osg::MatrixTransform* m_matrixTransformPtr;
        static WorldManager* m_instance;

	};

}
#endif //_FT_WORLD_MANAGER