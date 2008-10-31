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
	enum ShadowTraversalMask { CastShadow = 0x1, ReceiveShadow = 0x2 };
	class ShadowManager
	{
	public:
		ShadowManager(void);
		virtual ~ShadowManager(void){};
        bool DROP_SHADOW;
        static ShadowManager* getInstance();
        static void DestroyInstance();
		void Init(osg::MatrixTransform* worldMatrix) {m_matrixTransformPtr = worldMatrix;};
				
		void CreateGlobalShadowedScene();
		void AddToGlobalShadowedScene(osg::Node* node);
		osgShadow::ShadowedScene* GlobalShadowedScene() { return m_globalShadowedScene.get(); }; 
		void setShadowMask(osg::Node* node, ShadowTraversalMask mask);
		
		//void AddShadowCastToGlobalScene(osg::Group* shadowCastingObj);
		//void CreateGlobalShadowReceiver(osg::Group* node);


		void CreateShadowScene(const std::string shadowSceneName);
		osgShadow::ShadowedScene* getShadowScene(const std::string shadowSceneName);
		void AddShadowReceiver(const std::string shadowSceneName, osg::Group* shadowReceiverObj);
		void AddShadowCast(const std::string shadowSceneName, osg::Group* shadowCastingObj);

		bool AddToScene(osg::Group* node);
		bool RemoveFromScene(osg::Group* node);

		//osg::MatrixTransform* getShadowedWorldNode() {return m_matrixTransformPtr;}
		//connects with worldnode - whatfor
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


/*
ShadowManager use cases:
always at first define ShadowManager singleton in program
I. using one global shadowed scene
Such case gives a possibility of casting shadows on one global shadowed scene. 
Shadowed scene is attached to the main scene node and particular elements are set to be casting or receiving shadows.
a) define GlobalShadowedScene  (in constructor)
b) assign upper scene node to GlobalShadowedScene
	e.x. ShadowManager::getInstance->AddToGlobalShadowedScene(worldNode);
c) set mask for nodes we want to be affected by shadow mechanism we have three choices here:
c1) shadow cast  [ShadowManager::getInstance->setForGlobalShadow(element, CastShadow)]
c2) shadow receive [ShadowManager::getInstance->setForGlobalShadow(element, ReceiveShadow)]
c3) shadow cast and receive [ShadowManager::getInstance->setForGlobalShadow(element, CastAndReceiveShadow)]
d)attach GlobalShadowedScene to viewer or something similar


II. using specific shadow scenes (when we want to shadow within specific structure - i.e. human, passing cars and so on)
a) define shadow manager
b) define

*/