/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: abak
 */

#include "osgavatarfactory.h"

using namespace ft;

OsgAvatarFactory* OsgAvatarFactory::m_instance = NULL;

/**
 * \brief Returns the only instance of ft::OsgAvatarFactory (creates it at first call to this method)
 *
 * \return ft::OsgAvatarFactory * the only instance of OsgAvatarFactory
 **/
OsgAvatarFactory* OsgAvatarFactory::getOsgAvatarFactoryInstance()
{
    if (m_instance == NULL)
    {
        //if (Debug::MODEL_LOADING)
        //    _dbg << "Cal3DObjectFactory::getMeshObjectFactoryInstance(): instance of Cal3DObjectFactory created " << endl;

        m_instance = new OsgAvatarFactory();
    }

    return m_instance;
}

/// \brief Releases all resources related to this factory
void OsgAvatarFactory::DestroyInstance()
{
    if (m_instance != NULL)
    {
        m_instance->ReleaseAvatarTypes();
        delete m_instance;
    }
}

/**
 * \brief Creates a new mesh object and constructs all related objects in Cal3d
 *
 * \param const std::string modelName - unique string which is used for indexing appriopriate CalCoreModel in factory
 * \param const std::string objectName - name of object
 * \return ft::Cal3DObject* - new mesh object or NULL if creation failed
 **/
//Cal3DObject* Cal3DObjectFactory::CreateAvatar(const std::string modelName, std::string objectName)
OsgAvatar* OsgAvatarFactory::CreateAvatar(const std::string avatarTypeName, const std::string objectName)
{
//    if (Debug::MODEL_LOADING)
//        _dbg << "Cal3DObjectFactory::CreateAvatar, for model: " + modelName + " with new name: " + objectName << std::endl; 

    OsgAvatar* newObject = NULL;
											
	OsgAvatarType* avatarType = getAvatarType(avatarTypeName);

	if (avatarType == NULL)
	{
        //m_source_3dsmax = false; //can be set to true inside LoadCalCoreModel method
        avatarType = CreateAvatarType(avatarTypeName);
        if (avatarType != NULL)
        {
            m_avatarTypes.insert( std::make_pair( std::string(avatarTypeName), avatarType ) );
			avatarType->InitMotions();
            //(static_cast<Cal3dType*>(coreModel))->InitTransform(m_source_3dsmax);
        }
	}

    if (avatarType != NULL)
    {
		osg::ref_ptr< osgCal::BasicMeshAdder > meshAdder( new osgCal::DefaultMeshAdder );
	    osgCal::Model* model = new osgCal::Model();
		model->load( avatarType->getCoreModel(), meshAdder.get() );

		newObject = new OsgAvatar(model, avatarType, objectName);
    }
    else
    {

		//TODO:  print ERROR
        //if (Debug::MODEL_LOADING)
        //    _dbg << "Cal3DObjectFactory::CreateMeshObject,cration of object " + objectName + " failed." << std::endl;
    }

    return newObject;
}

/**
 * \brief Returns CalCoreModel for specifiv type name
 *
 * \param const std::string &typeName - name of core model
 * \return CalCoreModel* - CalCoreModel object
 **/
OsgAvatarType* OsgAvatarFactory::getAvatarType(const std::string &typeName)
{
	OsgAvatarType* avatarType = NULL;

	std::map<std::string,OsgAvatarType*>::iterator it = m_avatarTypes.find(typeName);

    if ( it!=m_avatarTypes.end()) {

        avatarType = it->second;
	}
	return avatarType;
}

/**
 * \brief Loads new Cal3dType
 *
 * \param const std::string modelName - name of model which responds to appriopriate configuration file
 * \return Cal3dType* - loaded core model or NULL if loading failed
 **/
OsgAvatarType* OsgAvatarFactory::CreateAvatarType(const std::string &typeName)
{
    //if (Debug::MODEL_LOADING)
    //    _dbg << "Cal3DObjectFactory::LoadCalCoreModel, for model: " + modelName  << std::endl;

    OsgAvatarType* avatarType =  NULL; //new OsgAvatarType(); //CreateCoreModel(modelName);

    osgCal::CoreModel* coreModel( new osgCal::CoreModel() );
    int         animNum = -1;
    osg::ref_ptr< osgCal::MeshParameters > p( new osgCal::MeshParameters );
	
	p->software = true; //TODO: make it configurable

	try 
	{
		coreModel->load( typeName, p.get() );
	}
	catch ( std::runtime_error& e )
    {
		//TODO:  release coreModel object

        std::cout << "runtime error during load:" << std::endl
                      << e.what() << std::endl;
		return NULL;
    }
    
	CalCoreModel* calCoreModel = coreModel->getCalCoreModel();
	/// TODO change it... calCoreModel->setName(typeName.c_str());  //replace "Dummy" with proper type name

	avatarType = new OsgAvatarType(typeName, coreModel);

    return avatarType;
}

/// \brief Releases all avatar type objects created by this factory
void OsgAvatarFactory::ReleaseAvatarTypes()
{
    //TODO: add releasing models
    // - delete each memeber of map
    // - make clear() on map
	std::map<std::string,OsgAvatarType*>::iterator it = m_instance->m_avatarTypes.begin();
	for( ; it != m_instance->m_avatarTypes.end(); ++it ) {
		delete it->second;
    }
    m_instance->m_avatarTypes.clear();
}

