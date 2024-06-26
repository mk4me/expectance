#include "shadowmanager.h"

#include <iostream>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ParallelSplitShadowMap>
#include"../app/config.h"

using namespace ft;

ShadowManager* ShadowManager::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::ShadowManager (creates it at first call to this method)
 *
 * \return ft::ShadowManager * - the only instance of ShadowManager
 **/
ShadowManager* ShadowManager::getInstance()
{
    if (m_instance == NULL)
    {
        std::cout << "ShadowManager::getInstace(): instance of ShadowManager created " << std::endl;
        m_instance = new ShadowManager();
    }

    return m_instance;
}

ShadowManager::ShadowManager()
{
	DROP_SHADOW = ((Config::getInstance()->IsKey("osg_drop_shadow")) && (Config::getInstance()->GetIntVal("osg_drop_shadow")==1));
	CreateGlobalShadowedScene();
}

/**
 * \brief Releases all resources related to this ShadowManager
 *
 **/
void ShadowManager::DestroyInstance()
{
    if (m_instance != NULL)
	{
		//std::map<std::string, osgShadow::ShadowedScene* >::iterator it = m_instance->m_ShadowObjectsMap.begin();
		//for( ; it != m_instance->m_ShadowObjectsMap.end(); ++it ) {
		//	delete it->second;
	 //   }
	    m_instance->m_ShadowObjectsMap.clear();
        delete m_instance;
	}
}


void ShadowManager::CreateShadowScene(const std::string shadowSceneName)
{
	if ( (!shadowSceneName.empty()) && (m_matrixTransformPtr!=NULL) ) 
	{
	 	std::map<std::string,osgShadow::ShadowedScene*>::iterator it = m_ShadowObjectsMap.find(shadowSceneName);
		if ( it==m_ShadowObjectsMap.end()) // not found shadowscene
		{ 
			osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
			shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
			shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
			//////osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
			osg::ref_ptr<osgShadow::ShadowTexture> sm = new osgShadow::ShadowTexture;
			//osg::ref_ptr<osgShadow::ParallelSplitShadowMap> sm = new osgShadow::ParallelSplitShadowMap(NULL, 3);
			//sm->setTextureResolution(1024);
			//sm->setMinNearDistanceForSplits(1);
			//sm->setMaxFarDistance(1);
			//sm->setMoveVCamBehindRCamFactor(1);
			//double polyoffsetfactor = sm->getPolygonOffset().x();
			//double polyoffsetunit   = sm->getPolygonOffset().y();
			//sm->setPolygonOffset(osg::Vec2(polyoffsetfactor,polyoffsetunit));
			////osg::ref_ptr<osgShadow::SoftShadowMap> sm = new osgShadow::SoftShadowMap;
			shadowedScene->setShadowTechnique(sm.get());
			//////int mapres = 1024;
			//////sm->setTextureSize(osg::Vec2s(mapres,mapres));
			//////osg::Vec4 lightpos = osg::Vec4(0.5f,0.25f,0.8f,0.0f);
			//////osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
			//////ls->getLight()->setPosition(lightpos);
			//////ls->getLight()->setAmbient(osg::Vec4(0.2,0.2,0.2,1.0));
			//////ls->getLight()->setDiffuse(osg::Vec4(0.8,0.8,0.8,1.0));
			//////sm->setLight(ls.get());
			////////shadowedScene->addChild(ls.get());

			
			m_matrixTransformPtr->addChild(shadowedScene.get());
			
			m_ShadowObjectsMap.insert( std::make_pair( std::string(shadowSceneName), shadowedScene.get() ) );	
		}
	}
}

osgShadow::ShadowedScene* ShadowManager::getShadowScene(const std::string shadowSceneName)
{
	if ( (!shadowSceneName.empty()) && (m_matrixTransformPtr!=NULL) ) 
	{
	 	std::map<std::string,osgShadow::ShadowedScene*>::iterator it = m_ShadowObjectsMap.find(shadowSceneName);
		if ( it==m_ShadowObjectsMap.end()) // not found shadowscene
		{ 
			return NULL;
		}
		else
			return (dynamic_cast<osgShadow::ShadowedScene*>(it->second));
	}
	else
		return NULL;
}

void ShadowManager::AddShadowCast(const std::string shadowSceneName, osg::Group* shadowCastingObj)
{
	if ( (!shadowSceneName.empty()) && (shadowCastingObj!=NULL) )
	{
	 	std::map<std::string,osgShadow::ShadowedScene*>::iterator it = m_ShadowObjectsMap.find(shadowSceneName);
		if ( it!=m_ShadowObjectsMap.end()) // found shadowscene
		{
			//osg::Group* castShadowObject = new osg::Group();
			//castShadowObject->addChild(shadowCastingObj);
			shadowCastingObj->setNodeMask(shadowCastingObj->getNodeMask() | CastsShadowTraversalMask);
			//shadowedScene->addChild(castShadowObject);
			(dynamic_cast<osgShadow::ShadowedScene*>(it->second))->addChild(shadowCastingObj);
		}
	}
}

void ShadowManager::AddShadowReceiver(const std::string shadowSceneName, osg::Group *shadowReceiverObj)
{
	if ( (!shadowSceneName.empty()) && (shadowReceiverObj!=NULL) )
	{
	 	std::map<std::string,osgShadow::ShadowedScene*>::iterator it = m_ShadowObjectsMap.find(shadowSceneName);
		if ( it!=m_ShadowObjectsMap.end()) // found shadowscene
		{
			//osg::Group* castShadowObject = new osg::Group();
			//castShadowObject->addChild(shadowCastingObj);
			shadowReceiverObj->setNodeMask(ReceivesShadowTraversalMask);
			//shadowedScene->addChild(castShadowObject);
			(dynamic_cast<osgShadow::ShadowedScene*>(it->second))->addChild(shadowReceiverObj);
		}
	}
}

void ShadowManager::CreateGlobalShadowedScene()
{
	if (m_globalShadowedScene== NULL)
	{
		m_globalShadowedScene = new osgShadow::ShadowedScene;
		m_globalShadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
		m_globalShadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
		osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
		m_globalShadowedScene->setShadowTechnique(sm.get());
		int mapres = 1024;
		sm->setTextureSize(osg::Vec2s(mapres,mapres));
	}
}

void ShadowManager::AddToGlobalShadowedScene(osg::Node* node)
{
	if (node!=NULL)
		m_globalShadowedScene->addChild(node);
}

void ShadowManager::setShadowMask(osg::Node* node, ShadowTraversalMask mask)
{
	if (node!=NULL)
	{
		switch (mask)
		{
		case CastShadow: node->setNodeMask(node->getNodeMask() | CastsShadowTraversalMask);
			break;
		case ReceiveShadow: node->setNodeMask(node->getNodeMask() | ReceivesShadowTraversalMask);
			break;
		default:
			break;
		}
		
	}
}

//void ShadowManager::CreateGlobalShadowReceiver(osg::Group *node)
//{
//	if ( (m_globalShadowedScene != NULL) && (node!=NULL) )
//	{
//		node->setNodeMask(CastsShadowTraversalMask);
//	}
//}
//
//void ShadowManager::AddShadowCastToGlobalScene(osg::Group *shadowCastingObj)
//{
//}