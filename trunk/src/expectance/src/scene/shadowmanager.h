/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: mka
 */
#ifndef _FT_SHADOW_MANAGER
#define _FT_SHADOW_MANAGER

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osgShadow/ShadowedScene>

namespace ft 
{
	class ShadowManager
	{
	public:
		ShadowManager(void){m_matrixTransformPtr = NULL;};
		virtual ~ShadowManager(void){};
        static bool DROP_SHADOW;
        static ShadowManager* getInstance();
        static void DestroyInstance();
		osg::MatrixTransform* getShadowedWorldNode() {return m_matrixTransformPtr;}
		//connects with worldnode
		void Init(osg::MatrixTransform* worldMatrix) {m_matrixTransformPtr = worldMatrix; CreateGlobalShadowScene();};
		
		void CreateGlobalShadowScene();
		void AddShadowCastToGlobalScene(osg::Group* shadowCastingObj);
		void CreateGlobalShadowReceiver(osg::Group* node);
		osgShadow::ShadowedScene* GetGlobalShadowReceiver() { return m_globalShadowedScene.get(); }; 

		void CreateShadowScene(const std::string shadowSceneName);
		osgShadow::ShadowedScene* getShadowScene(const std::string shadowSceneName);
		void AddShadowReceiver(const std::string shadowSceneName, osg::Group* shadowReceiverObj);
		void AddShadowCast(const std::string shadowSceneName, osg::Group* shadowCastingObj);

		bool AddToScene(osg::Group* node);
		bool RemoveFromScene(osg::Group* node);

    private:
		osg::ref_ptr<osgShadow::ShadowedScene> m_globalShadowedScene;
		osg::MatrixTransform* m_matrixTransformPtr;
		std::map<std::string, osgShadow::ShadowedScene*> m_ShadowObjectsMap;
        static ShadowManager* m_instance;
		static const int ReceivesShadowTraversalMask = 0x1;
		static const int CastsShadowTraversalMask = 0x2;
	};

}
#endif //_FT_SHADOW_MANAGER