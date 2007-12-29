/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "cal3dobjectfactory.h"


using namespace ft;

Cal3DObjectFactory* Cal3DObjectFactory::m_instance = NULL;


/**
 * \brief Returns the only instance of ft::Cal3DObjectFactory (creates it at first call to this method)
 *
 * \return ft::Cal3DObjectFactory * the only instance of Cal3DObjectFactory
 **/
Cal3DObjectFactory* Cal3DObjectFactory::getMeshObjectFactoryInstance()
{
    if (m_instance == NULL)
    {
        if (Debug::MODEL_LOADING)
            _dbg << "Cal3DObjectFactory::getMeshObjectFactoryInstance(): instance of Cal3DObjectFactory created " << endl;

        m_instance = new Cal3DObjectFactory();
    }

    return m_instance;
}

/// \brief Releases all resources related to this factory
void Cal3DObjectFactory::DestroyInstance()
{
    if (m_instance != NULL)
    {
        m_instance->ReleaseCalCoreModels();
        delete m_instance;
    }
}

/**
 * \brief Creates a a specific instance of mesh object. It can be overriden in descendant classes.
 *
 * \param const std::string modelName - unique string which is used for indexing appriopriate CalCoreModel in factory
 * \param const std::string objectName - name of object
 * \return ft::Cal3DObject* - new mesh object instance
 **/
Cal3DObject* Cal3DObjectFactory::CreateMeshObjectInstance(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName)
{
    return new Cal3DObject(calModel, calCoreModel, modelName);
}

/**
 * \brief Creates a new mesh object and constructs all related objects in Cal3d
 *
 * \param const std::string modelName - unique string which is used for indexing appriopriate CalCoreModel in factory
 * \param const std::string objectName - name of object
 * \return ft::Cal3DObject* - new mesh object or NULL if creation failed
 **/
Cal3DObject* Cal3DObjectFactory::CreateMeshObject(const std::string modelName, std::string objectName)
{
    if (Debug::MODEL_LOADING)
        _dbg << "Cal3DObjectFactory::CreateAvatar, for model: " + modelName + " with new name: " + objectName << std::endl; 

    Cal3DObject *newObject = NULL;
    CalModel* newModel= NULL;
    Cal3dType* coreModel = NULL;

	std::map<std::string,CalCoreModel*>::iterator it = m_coreModels.find(modelName);

    if ( it!=m_coreModels.end()) {

        coreModel = static_cast<Cal3dType *>(it->second);
	}
    else
    {
        m_source_3dsmax = false; //can be set to true inside LoadCalCoreModel method
        coreModel = LoadCalCoreModel(modelName);
        if (coreModel != NULL)
        {
            m_coreModels.insert( std::make_pair( std::string(modelName), coreModel ) );
            coreModel->InitTransform(m_source_3dsmax);
        }
    }

    if (coreModel != NULL)
    {
        newModel = new CalModel(coreModel);
        InitModelMeshes(coreModel, newModel);
        newObject = CreateMeshObjectInstance(newModel, coreModel, objectName);
    
        if (Debug::MODEL_LOADING)
            _dbg << "Cal3DObjectFactory::CreateMeshObject,new mesh object " + objectName + " created." << std::endl;
    }
    else
    {
        if (Debug::MODEL_LOADING)
            _dbg << "Cal3DObjectFactory::CreateMeshObject,cration of object " + objectName + " failed." << std::endl;
    }

    return newObject;
}

/**
 * \brief Create new Cal3dType object
 *
 * \param const std::string &typeName - name of core model
 * \return Cal3dType * - created object
 **/
Cal3dType* Cal3DObjectFactory::CreateCoreModel(const std::string &typeName)
{
    return new Cal3dType(typeName);
}

/**
 * \brief Loads new Cal3dType
 *
 * \param const std::string modelName - name of model which responds to appriopriate configuration file
 * \return Cal3dType* - loaded core model or NULL if loading failed
 **/
Cal3dType* Cal3DObjectFactory::LoadCalCoreModel(const std::string modelName)
{
    if (Debug::MODEL_LOADING)
        _dbg << "Cal3DObjectFactory::LoadCalCoreModel, for model: " + modelName  << std::endl;

    Cal3dType* coreModel = CreateCoreModel(modelName);
    
    if (ParseModelConfiguration(modelName, coreModel))
    {
        InitCoreModelMaterials(coreModel);
        // it must be done before access to bounding boxes by mka 2007.06.10
        coreModel->getCoreSkeleton()->calculateBoundingBoxes(coreModel);

        if (Debug::MODEL_LOADING)
            _dbg << "Cal3DObjectFactory::LoadCalCoreModel, model " << modelName << " loaded. " << std::endl;
    }
    else
    {
        delete coreModel;
        coreModel = NULL;
    }

    return coreModel;
}

/**
 * \brief Parses configuration file for core model and initiates properly given CalCoreModel object
 *		  NOTE: Extracted from Form Viewer app.
 *
 * \param const std::string & strFilename - name of configuration file
 * \param CalCoreModel * coreModel - core model object to fill with data from configuration file
 * \return bool - true if parsing succeed, false if parsing failed
 **/
bool Cal3DObjectFactory::ParseModelConfiguration(const std::string& modelName, CalCoreModel* coreModel)
{
  // open the model configuration file
  std::string modelWorkDir = FT_MODELPATH + modelName + "\\";
  std::ifstream file;
  std::string fn = modelWorkDir + modelName + ".cfg";
  file.open(fn.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    if (Debug::ERR)
        _dbg << Debug::ERR_STR << "Failed to open model configuration file '" << fn << "'." << std::endl;

    return false;
  }

  // parse all lines from the model configuration file
  int line;
  for(line = 1; ; line++)
  {
    // read the next model configuration line
    std::string strBuffer;
    std::getline(file, strBuffer);

    // stop if we reached the end of file
    if(file.eof()) break;

    // check if an error happend while reading from the file
    if(!file)
    {
      if (Debug::ERR)
          _dbg << Debug::ERR_STR << "Error while reading from the model configuration file '" << fn << "'." << std::endl;
      return false;
    }

    // find the first non-whitespace character
    std::string::size_type pos;
    pos = strBuffer.find_first_not_of(" \t");

    // check for empty lines
    if((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

    // check for comment lines
    if(strBuffer[pos] == '#') continue;

    // get the key
    std::string strKey;
    strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
    pos += strKey.size();

    // get the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos);
    if((pos == std::string::npos) || (strBuffer[pos] != '='))
    {
      if (Debug::ERR)
          _dbg <<  Debug::ERR_STR << fn << "(" << line << "): Invalid syntax." << std::endl;
      return false;
    }

    // find the first non-whitespace character after the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos + 1);

    // get the data
    std::string strData;
    strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

    // handle the model creation
    if(strKey == "source") //mka - for correct axis order of loaded data
	{
		if(strData.compare("3dsmax")==0)
        {
			CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
            m_source_3dsmax = true;
        }
	}
	else if(strKey == "scale")
    {
      // set rendering scale factor
      //m_scale = atof(strData.c_str());  //TODO  abak: it should be used somewhere
    }
    else if(strKey == "skeleton")
    {
      // load core skeleton
      if (Debug::MODEL_LOADING)
        _dbg << "Loading skeleton '" << strData << "'..." << std::endl;

      if(!coreModel->loadCoreSkeleton(modelWorkDir + strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      if (Debug::MODEL_LOADING)
        _dbg << "Loading animation '" << strData << "'..." << std::endl;

      if(coreModel->loadCoreAnimation(modelWorkDir + strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "mesh")
    {
      // load core mesh
      if (Debug::MODEL_LOADING)
        _dbg << "Loading mesh '" << strData << "'..." << std::endl;

      if(coreModel->loadCoreMesh(modelWorkDir + strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "material")
    {
      // load core material
      if (Debug::MODEL_LOADING)
        _dbg << "Loading material '" << strData << "'..." << std::endl;

      if(coreModel->loadCoreMaterial(modelWorkDir + strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else
    {
      // everything else triggers an error message, but is ignored
      if (Debug::ERR)
          _dbg << Debug::ERR_STR << fn << "(" << line << "): Invalid syntax." << std::endl;
    }
  }

  // explicitely close the file
  file.close();

  return true;
}

/**
 * \brief Initiates materials for core model 
 *		  - make one material thread for each material
 *		  NOTE: this is not the right way to do it, but this Viewer can't do the right
 *		  mapping without further information on the model etc., so this is the only
 *		  thing we can do here.
 * \param CalCoreModel * coreModel - core model for which materials should be initialized
 **/
void Cal3DObjectFactory::InitCoreModelMaterials(CalCoreModel* coreModel)
{
  int materialId;
  for(materialId = 0; materialId < coreModel->getCoreMaterialCount(); materialId++)
  {
    // create the a material thread
    coreModel->createCoreMaterialThread(materialId);

    // initialize the material thread
    coreModel->setCoreMaterialId(materialId, 0, materialId);
  }

  // load all textures and store the opengl texture id in the corresponding map in the material
  for(int materialId = 0; materialId < coreModel->getCoreMaterialCount(); materialId++)
  {
    // get the core material
    CalCoreMaterial *pCoreMaterial = coreModel->getCoreMaterial(materialId);

    // loop through all maps of the core material
    for(int mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
    {
      // get the filename of the texture
      std::string strFilename = pCoreMaterial->getMapFilename(mapId);

      // load the texture from the file
      GLuint textureId = LoadTexture(strFilename);  //TODO: abak:  use TextureManager here

      // store the opengl texture id in the user data of the map
      pCoreMaterial->setMapUserData(mapId, (Cal::UserData)textureId);
    }
  }

}

/**
 * \brief Initializes meshes for specific CalModel that is instance of given CalCoreModel
 *
 * \param CalCoreModel * coreModel - core model that was used to create given CalModel
 * \param CalModel * model - model in Cal3d for which materials should be initialized
 **/
void Cal3DObjectFactory::InitModelMeshes(CalCoreModel* coreModel, CalModel* model)
{	
 
  
	// attach all meshes to the model
	for(int meshId = 0; meshId < coreModel->getCoreMeshCount(); meshId++)
	{
		model->attachMesh(meshId);
	}

	// set the material set of the whole model
	model->setMaterialSet(0);


	// Disable internal data if hardware acceleration has been activated
	// this disable spring system necessary for video hardware acceleration

	if(Config::getInstance()->IsKey("hardware_acceleration"))	
	{
		if (Config::getInstance()->GetIntVal("hardware_acceleration")==1)
		{
          if (Debug::MODEL_LOADING)
		    _dbg << "Disable internal (spring system for hardware acceleration)." << std::endl;

		  model->disableInternalData();
		}
	}

}
/// \brief Releases all CalCoreModel-s created by this factory
void Cal3DObjectFactory::ReleaseCalCoreModels()
{
    //TODO: add releasing models
    // - delete each memeber of map
    // - make clear() on map
	std::map<std::string,CalCoreModel*>::iterator it = m_instance->m_coreModels.begin();
	for( ; it != m_instance->m_coreModels.end(); ++it ) {
		delete it->second;
    }
    m_instance->m_coreModels.clear();
}


/// \brief Loads and creates a texture from a given file 
/// \param const std::string & strFilename - name of file with texture
/// \return GLuint - texture id of loaded texture
GLuint Cal3DObjectFactory::LoadTexture(const std::string& strFilename)
{
  GLuint textureId=0;
  if(_stricmp(strrchr(strFilename.c_str(),'.'),".raw")==0)
  {

     // open the texture file
     std::ifstream file;
     file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
     if(!file)
     {
       if (Debug::ERR)
           _dbg << Debug::ERR_STR << "Texture file '" << strFilename << "' not found." << std::endl;

       return 0;
     }

     // read the width, height and depth of the texture
     int width;
     file.read((char *)&width, 4);
     int height;
     file.read((char *)&height, 4);
     int depth;
     file.read((char *)&depth, 4);

     // allocate a temporary buffer to load the texture to
     unsigned char *pBuffer;
     pBuffer = new unsigned char[2 * width * height * depth];
     if(pBuffer == 0)
     {
       if (Debug::ERR)
           _dbg << Debug::ERR_STR << "Memory allocation for texture '" << strFilename << "' failed." << std::endl;

       return 0;
     }

     // load the texture
     file.read((char *)pBuffer, width * height * depth);

     // explicitely close the file
     file.close();

     // flip texture around y-axis (-> opengl-style)
     int y;
     for(y = 0; y < height; y++)
     {
       memcpy(&pBuffer[(height + y) * width * depth], &pBuffer[(height - y - 1) * width * depth], width * depth);
     }
     
     // generate texture
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

     glGenTextures(1, &textureId);
     glBindTexture(GL_TEXTURE_2D, textureId);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     
	 glTexImage2D(GL_TEXTURE_2D, 0, (depth == 3) ? GL_RGB : GL_RGBA, width, height, 0, (depth == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, &pBuffer[width * height * depth]);

     // free the allocated memory

     delete [] pBuffer;  
  }

  return textureId;
}


