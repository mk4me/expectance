/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#include "avatarfactory.h"
#include "../utility/debug.h"

using namespace ft;

AvatarFactory* AvatarFactory::m_instance = NULL;


AvatarFactory* AvatarFactory::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("AvatarFactory::getInstace(): instance of AvatarFactory created ");
        m_instance = new AvatarFactory();
    }

    return m_instance;
}

void AvatarFactory::DestroyInstance()
{
    if (m_instance != NULL)
    {
        m_instance->ReleaseCalCoreModels();
        delete m_instance;
    }
}

Avatar* AvatarFactory::CreateAvatar(const std::string modelName, std::string avatarName)
{
    std::cout << "AvatarFactory::CreateAvatar, for model: " + modelName + " with new name: " + avatarName << std::endl; 

    Avatar *newAvatar = NULL;
    CalModel* newModel= NULL;
    CalCoreModel* coreModel = NULL;

	std::map<std::string,CalCoreModel*>::iterator it = m_coreModels.find(modelName);

    if ( it!=m_coreModels.end()) {

        coreModel = static_cast<CalCoreModel *>(it->second);
	}
    else
    {
        coreModel = LoadCalCoreModel(modelName);
        if (coreModel != NULL)
            m_coreModels.insert( std::make_pair( std::string(modelName), coreModel ) );
    }

    if (coreModel != NULL)
    {
        newModel = new CalModel(coreModel);
        InitModelMeshes(coreModel, newModel);
        newAvatar = new MovableAvatar(newModel, coreModel, avatarName);
    
        std::cout << "AvatarFactory::CreateAvatar,new avatar " + avatarName + " created." << std::endl;
    }
    else
        std::cout << "AvatarFactory::CreateAvatar,cration of avatar " + avatarName + " failed." << std::endl;

    return newAvatar;
}

CalCoreModel* AvatarFactory::LoadCalCoreModel(const std::string modelName)
{
    std::cout << "AvatarFactory::LoadCalCoreModel, for model: " + modelName  << std::endl;

    CalCoreModel* coreModel = new CalCoreModel("dummy");
    
    if (ParseModelConfiguration(modelName, coreModel))
    {
        InitCoreModelMaterials(coreModel);
        // it must be done before access to bounding boxes by mka 2007.06.10
        coreModel->getCoreSkeleton()->calculateBoundingBoxes(coreModel);

        std::cout << "AvatarFactory::LoadCalCoreModel, model " << modelName << " loaded. " << std::endl;
    }
    else
    {
        delete coreModel;
        coreModel = NULL;
    }

    return coreModel;
}


/*----- Parse the configuration file and load the whole model  -----
 * NOTE: Extracted from Form Viewer app.
 */
bool AvatarFactory::ParseModelConfiguration(const std::string& strFilename, CalCoreModel* coreModel)
{
  // open the model configuration file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
  if(!file)
  {
    std::cerr << "Failed to open model configuration file '" << strFilename << "'." << std::endl;
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
      std::cerr << "Error while reading from the model configuration file '" << strFilename << "'." << std::endl;
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
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
      return false;
    }

    // find the first non-whitespace character after the '=' character
    pos = strBuffer.find_first_not_of(" \t", pos + 1);

    // get the data
    std::string strData;
    strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

    // handle the model creation
    if(strKey == "scale")
    {
      // set rendering scale factor
      //m_scale = atof(strData.c_str());  //TODO  abak: it should be used somewhere
    }
    else if(strKey == "skeleton")
    {
      // load core skeleton
      std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
      if(!coreModel->loadCoreSkeleton(strData))
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "animation")
    {
      // load core animation
      std::cout << "Loading animation '" << strData << "'..." << std::endl;
      if(coreModel->loadCoreAnimation(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "mesh")
    {
      // load core mesh
      std::cout << "Loading mesh '" << strData << "'..." << std::endl;
      if(coreModel->loadCoreMesh(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else if(strKey == "material")
    {
      // load core material
      std::cout << "Loading material '" << strData << "'..." << std::endl;
      if(coreModel->loadCoreMaterial(strData) == -1)
      {
        CalError::printLastError();
        return false;
      }
    }
    else
    {
      // everything else triggers an error message, but is ignored
      std::cerr << strFilename << "(" << line << "): Invalid syntax." << std::endl;
    }
  }

  // explicitely close the file
  file.close();

  return true;
}

  // make one material thread for each material
  // NOTE: this is not the right way to do it, but this Viewer can't do the right
  // mapping without further information on the model etc., so this is the only
  // thing we can do here.
void AvatarFactory::InitCoreModelMaterials(CalCoreModel* coreModel)
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

void AvatarFactory::InitModelMeshes(CalCoreModel* coreModel, CalModel* model)
{
 
  
  // attach all meshes to the model
  for(int meshId = 0; meshId < coreModel->getCoreMeshCount(); meshId++)
  {
    model->attachMesh(meshId);
  }

  // set the material set of the whole model
  model->setMaterialSet(0);

}
void AvatarFactory::ReleaseCalCoreModels()
{
    //TODO: add releasing models
    // - delete each memeber of map
    // - make clear() on map
}

/*----- Load and create a texture from a given file -----*/
GLuint AvatarFactory::LoadTexture(const std::string& strFilename)
{
  GLuint textureId=0;
  if(_stricmp(strrchr(strFilename.c_str(),'.'),".raw")==0)
  {

     // open the texture file
     std::ifstream file;
     file.open(strFilename.c_str(), std::ios::in | std::ios::binary);
     if(!file)
     {
       std::cerr << "Texture file '" << strFilename << "' not found." << std::endl;
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
       std::cerr << "Memory allocation for texture '" << strFilename << "' failed." << std::endl;
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


