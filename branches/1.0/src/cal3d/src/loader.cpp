//****************************************************************************//
// loader.cpp                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/src/loader.h"
#include "cal3d/src/error.h"
#include "cal3d/src/matrix.h"
#include "cal3d/src/vector.h"
#include "cal3d/src/quaternion.h"
#include "cal3d/src/coremodel.h"
#include "cal3d/src/coreskeleton.h"
#include "cal3d/src/corebone.h"
#include "cal3d/src/coreanimation.h"
#include "cal3d/src/coremesh.h"
#include "cal3d/src/coresubmesh.h"
#include "cal3d/src/corematerial.h"
#include "cal3d/src/corekeyframe.h"
#include "cal3d/src/coretrack.h"
#include "cal3d/src/tinyxml.h"
#include "cal3d/src/streamsource.h"
#include "cal3d/src/buffersource.h"

using namespace cal3d;

int CalLoader::loadingMode;

 /*****************************************************************************/
/** Sets optional flags which affect how the model is loaded into memory.
  *
  * This function sets the loading mode for all future loader calls.
  *
  * @param flags A boolean OR of any of the following flags
  *         \li LOADER_ROTATE_X_AXIS will rotate the mesh 90 degrees about the X axis,
  *             which has the effect of swapping Y/Z coordinates.
  *         \li LOADER_INVERT_V_COORD will substitute (1-v) for any v texture coordinate
  *             to eliminate the need for texture inversion after export.
  *
  *****************************************************************************/
void CalLoader::setLoadingMode(int flags)
{
    loadingMode = flags;
}

 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from a file.
  *
  * @param strFilename The file to load the core animation instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadCoreAnimation(const std::string& strFilename, CalCoreSkeleton *skel)
{
	int _cmpANIMATION;
  	#if defined(FT_STRICMP)
		_cmpANIMATION = FT_STRICMP(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATION_XMLFILE_MAGIC);
	#else
		_cmpANIMATION = stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATION_XMLFILE_MAGIC);
	#endif

	if(strFilename.size()>= 3 && _cmpANIMATION==0)
    return loadXmlCoreAnimation(strFilename, skel);

  // open the file
  std::ifstream file(strFilename.c_str(), std::ios::in | std::ios::binary);

  //make sure it was opened properly
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  //make a new stream data source and use it to load the animation
  CalStreamSource streamSrc( file );
  
  CalCoreAnimationPtr coreanim = loadCoreAnimation( streamSrc,skel );
  if(coreanim) coreanim->setFilename( strFilename );

  //close the file
  file.close();

  return coreanim;
}

 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from a file.
  *
  * @param strFilename The file to load the core material instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterialPtr CalLoader::loadCoreMaterial(const std::string& strFilename)
{
	int _cmpMATERIAL;
  	#if defined(FT_STRICMP)
		_cmpMATERIAL = FT_STRICMP(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MATERIAL_XMLFILE_MAGIC);
	#else
		_cmpMATERIAL = stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MATERIAL_XMLFILE_MAGIC);
	#endif

  if(strFilename.size()>= 3 && _cmpMATERIAL==0)
    return loadXmlCoreMaterial(strFilename);

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);

  // make sure it opened properly
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  //make a new stream data source and use it to load the material
  CalStreamSource streamSrc( file );
  
  CalCoreMaterialPtr coremat = loadCoreMaterial( streamSrc );

  if(coremat) coremat->setFilename( strFilename );

  //close the file
  file.close();

  return coremat;

}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from a file.
  *
  * @param strFilename The file to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadCoreMesh(const std::string& strFilename)
{
	int _cmpMESH;
  	#if defined(FT_STRICMP)
		_cmpMESH = FT_STRICMP(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MESH_XMLFILE_MAGIC);
	#else
		_cmpMESH = stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MESH_XMLFILE_MAGIC);
	#endif

  if(strFilename.size()>= 3 && _cmpMESH==0)
    return loadXmlCoreMesh(strFilename);

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);

  // make sure it opened properly
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  //make a new stream data source and use it to load the mesh
  CalStreamSource streamSrc( file );
  
  CalCoreMeshPtr coremesh = loadCoreMesh( streamSrc );

  if(coremesh) coremesh->setFilename( strFilename );


  //close the file
  file.close();

  return coremesh;

}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from a file.
  *
  * @param strFilename The file to load the core skeleton instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadCoreSkeleton(const std::string& strFilename)
{
	int _cmpSKELETON;
  	#if defined(FT_STRICMP)
		_cmpSKELETON = FT_STRICMP(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::SKELETON_XMLFILE_MAGIC);
	#else
		_cmpSKELETON = stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::SKELETON_XMLFILE_MAGIC);
	#endif

  if(strFilename.size()>= 3 && _cmpSKELETON==0)
    return loadXmlCoreSkeleton(strFilename);

  // open the file
  std::ifstream file;
  file.open(strFilename.c_str(), std::ios::in | std::ios::binary);

  //make sure it opened properly
  if(!file)
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  //make a new stream data source and use it to load the skeleton
  CalStreamSource streamSrc( file );
  
  CalCoreSkeletonPtr coreskeleton = loadCoreSkeleton( streamSrc );

  //close the file
  file.close();

  return coreskeleton;

}


 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from an input stream.
  *
  * @param inputStream The stream to load the core animation instance from. This
  *                    stream should be initialized and ready to be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadCoreAnimation(std::istream& inputStream, CalCoreSkeleton *skel )
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreAnimation(streamSrc, skel);
}

 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from an input stream.
  *
  * @param inputStream The stream to load the core material instance from. This
  *                    stream should be initialized and ready to be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterialPtr CalLoader::loadCoreMaterial(std::istream& inputStream)
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreMaterial(streamSrc);
}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from an input stream.
  *
  * @param inputStream The stream to load the core mesh instance from. This
  *                    stream should be initialized and ready to be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadCoreMesh(std::istream& inputStream)
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreMesh(streamSrc);
}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from an input stream.
  *
  * @param inputStream The stream to load the core skeleton instance from. This
  *                    stream should be initialized and ready to be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadCoreSkeleton(std::istream& inputStream)
{
   //Create a new istream data source and pass it on
   CalStreamSource streamSrc(inputStream);
   return loadCoreSkeleton(streamSrc);
}




 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from a memory buffer.
  *
  * @param inputBuffer The memory buffer to load the core animation instance 
  *                    from. This buffer should be initialized and ready to 
  *                    be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadCoreAnimation(void* inputBuffer, CalCoreSkeleton *skel)
{
   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   return loadCoreAnimation(bufferSrc,skel);
}

 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from a memory buffer.
  *
  * @param inputBuffer The memory buffer to load the core material instance 
  *                    from. This buffer should be initialized and ready to 
  *                    be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterialPtr CalLoader::loadCoreMaterial(void* inputBuffer)
{
	if ( (memcmp( inputBuffer, "<HEADER", 7 ) == 0) || (memcmp( inputBuffer, "<MATERIAL", 9 ) == 0) )
	{
		cal3d::TiXmlDocument	doc;
		doc.Parse( static_cast<const char*>(inputBuffer) );
		if (doc.Error())
		{
		    CalError::setLastError(CalError::FILE_PARSER_FAILED, __FILE__, __LINE__ );
 		   return 0;
		}
		return loadXmlCoreMaterial( doc );
	}
	
   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   return loadCoreMaterial(bufferSrc);
}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from a memory buffer.
  *
  * @param inputBuffer The memory buffer to load the core mesh instance from. 
  *                    This buffer should be initialized and ready to be 
  *                    read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadCoreMesh(void* inputBuffer)
{
	if ( (memcmp( inputBuffer, "<HEADER", 7 ) == 0) || (memcmp( inputBuffer, "<MESH", 5 ) == 0) )
	{
		TiXmlDocument	doc;
		doc.Parse( static_cast<const char*>(inputBuffer) );
		if (doc.Error())
		{
		    CalError::setLastError(CalError::FILE_PARSER_FAILED, __FILE__, __LINE__ );
 		   return 0;
		}
		return loadXmlCoreMesh( doc );
	}

   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   return loadCoreMesh(bufferSrc);
}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from a memory buffer.
  *
  * @param inputBuffer The memory buffer to load the core skeleton instance 
  *                    from. This buffer should be initialized and ready to 
  *                    be read from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadCoreSkeleton(void* inputBuffer)
{
	if ( (memcmp( inputBuffer, "<HEADER", 7 ) == 0) || (memcmp( inputBuffer, "<SKELETON", 9 ) == 0) )
	{
		TiXmlDocument	doc;
		doc.Parse( static_cast<const char*>(inputBuffer) );
		if (doc.Error())
		{
		    CalError::setLastError(CalError::FILE_PARSER_FAILED, __FILE__, __LINE__ );
 		   return 0;
		}
		return loadXmlCoreSkeleton( doc );
	}

   //Create a new buffer data source and pass it on
   CalBufferSource bufferSrc(inputBuffer);
   return loadCoreSkeleton(bufferSrc);
}

 /*****************************************************************************/
/** Loads a core animation instance.
  *
  * This function loads a core animation instance from a data source.
  *
  * @param dataSrc The data source to load the core animation instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadCoreAnimation(CalDataSource& dataSrc, CalCoreSkeleton *skel)
{

  // check if this is a valid file
  char magic[4];
  if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::ANIMATION_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
    return 0;
  }

  // allocate a new core animation instance
  CalCoreAnimationPtr pCoreAnimation(new(std::nothrow) CalCoreAnimation);
  if(!pCoreAnimation)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // get the duration of the core animation
  float duration;
  if(!dataSrc.readFloat(duration))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check for a valid duration
  if(duration <= 0.0f)
  {
    CalError::setLastError(CalError::INVALID_ANIMATION_DURATION, __FILE__, __LINE__);
    return 0;
  }

  // set the duration in the core animation instance
  pCoreAnimation->setDuration(duration);

  // read the number of tracks
  int trackCount;
  if(!dataSrc.readInteger(trackCount) || (trackCount <= 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

	// read flags
	int flags = 0;
	if(version >= LIBRARY_VERSION) {
	  if(!dataSrc.readInteger(flags))
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
			return 0;
		}
	}

  // load all core bones
  int trackId;
  for(trackId = 0; trackId < trackCount; ++trackId)
  {
    // load the core track
    CalCoreTrack *pCoreTrack;
    pCoreTrack = loadCoreTrack(dataSrc, skel, duration, flags);
    if(pCoreTrack == 0)
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
    }

    // add the core track to the core animation instance
    pCoreAnimation->addCoreTrack(pCoreTrack);
  }

  return pCoreAnimation;
}

 /*****************************************************************************/
/** Loads a core material instance.
  *
  * This function loads a core material instance from a data source.
  *
  * @param dataSrc The data source to load the core material instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMaterialPtr CalLoader::loadCoreMaterial(CalDataSource& dataSrc)
{

  // check if this is a valid file
  char magic[4];
  if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::MATERIAL_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
    return 0;
  }

  // allocate a new core material instance
  CalCoreMaterialPtr pCoreMaterial = new(std::nothrow) CalCoreMaterial();
  if(!pCoreMaterial)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // get the ambient color of the core material
  CalCoreMaterial::Color ambientColor;
  dataSrc.readBytes(&ambientColor, sizeof(ambientColor));

  // get the diffuse color of the core material
  CalCoreMaterial::Color diffuseColor;
  dataSrc.readBytes(&diffuseColor, sizeof(diffuseColor));

  // get the specular color of the core material
  CalCoreMaterial::Color specularColor;
  dataSrc.readBytes(&specularColor, sizeof(specularColor));

  // get the shininess factor of the core material
  float shininess;
  dataSrc.readFloat(shininess);

  // check if an error happened
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // set the colors and the shininess
  pCoreMaterial->setAmbientColor(ambientColor);
  pCoreMaterial->setDiffuseColor(diffuseColor);
  pCoreMaterial->setSpecularColor(specularColor);
  pCoreMaterial->setShininess(shininess);

  // read the number of maps
  int mapCount;
  if(!dataSrc.readInteger(mapCount) || (mapCount < 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // reserve memory for all the material data
  if(!pCoreMaterial->reserve(mapCount))
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // load all maps
  int mapId;
  for(mapId = 0; mapId < mapCount; ++mapId)
  {
    CalCoreMaterial::Map map;

    // read the filename of the map
    std::string strName;
    dataSrc.readString(map.strFilename);

    // initialize the user data
    map.userData = 0;

    // check if an error happened
    if(!dataSrc.ok())
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
    }

    // set map in the core material instance
    pCoreMaterial->setMap(mapId, map);
  }

  return pCoreMaterial;
}

 /*****************************************************************************/
/** Loads a core mesh instance.
  *
  * This function loads a core mesh instance from a data source.
  *
  * @param dataSrc The data source to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadCoreMesh(CalDataSource& dataSrc)
{

  // check if this is a valid file
  char magic[4];
  if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::MESH_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
    return 0;
  }

  // get the number of submeshes
  int submeshCount;
  if(!dataSrc.readInteger(submeshCount))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // allocate a new core mesh instance
  CalCoreMeshPtr pCoreMesh = new(std::nothrow) CalCoreMesh();
  if(!pCoreMesh)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // load all core submeshes
  for(int submeshId = 0; submeshId < submeshCount; ++submeshId)
  {
    // load the core submesh
    CalCoreSubmesh *pCoreSubmesh = loadCoreSubmesh(dataSrc);
    if(pCoreSubmesh == 0)
    {
      return 0;
    }

    // add the core submesh to the core mesh instance
    pCoreMesh->addCoreSubmesh(pCoreSubmesh);
  }

  return pCoreMesh;
}

 /*****************************************************************************/
/** Loads a core skeleton instance.
  *
  * This function loads a core skeleton instance from a data source.
  *
  * @param dataSrc The data source to load the core skeleton instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadCoreSkeleton(CalDataSource& dataSrc)
{

  // check if this is a valid file
  char magic[4];
  if(!dataSrc.readBytes(&magic[0], 4) || (memcmp(&magic[0], Cal::SKELETON_FILE_MAGIC, 4) != 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // check if the version is compatible with the library
  int version;
  if(!dataSrc.readInteger(version) || (version < Cal::EARLIEST_COMPATIBLE_FILE_VERSION) || (version > Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__);
    return 0;
  }

  // read the number of bones
  int boneCount;
  if(!dataSrc.readInteger(boneCount) || (boneCount <= 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // allocate a new core skeleton instance
  CalCoreSkeletonPtr pCoreSkeleton = new(std::nothrow) CalCoreSkeleton();
  if(!pCoreSkeleton)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // load all core bones
  for(int boneId = 0; boneId < boneCount; ++boneId)
  {
    // load the core bone
    CalCoreBone *pCoreBone = loadCoreBones(dataSrc);
    if(pCoreBone == 0)
    {
      return 0;
    }

    // set the core skeleton of the core bone instance
    pCoreBone->setCoreSkeleton(pCoreSkeleton.get());

    // add the core bone to the core skeleton instance
    pCoreSkeleton->addCoreBone(pCoreBone);

    // add a core skeleton mapping of the bone's name for quick reference later
    pCoreSkeleton->mapCoreBoneName(boneId, pCoreBone->getName());

  }

  // calculate state of the core skeleton
  pCoreSkeleton->calculateState();

  return pCoreSkeleton;
}

 /*****************************************************************************/
/** Loads a core bone instance.
  *
  * This function loads a core bone instance from a data source.
  *
  * @param dataSrc The data source to load the core bone instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core bone
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreBone *CalLoader::loadCoreBones(CalDataSource& dataSrc)
{
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // read the name of the bone
  std::string strName;
  dataSrc.readString(strName);

  // get the translation of the bone
  float tx, ty, tz;
  dataSrc.readFloat(tx);
  dataSrc.readFloat(ty);
  dataSrc.readFloat(tz);

  // get the rotation of the bone
  float rx, ry, rz, rw;
  dataSrc.readFloat(rx);
  dataSrc.readFloat(ry);
  dataSrc.readFloat(rz);
  dataSrc.readFloat(rw);

  // get the bone space translation of the bone
  float txBoneSpace, tyBoneSpace, tzBoneSpace;
  dataSrc.readFloat(txBoneSpace);
  dataSrc.readFloat(tyBoneSpace);
  dataSrc.readFloat(tzBoneSpace);

  // get the bone space rotation of the bone
  float rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace;
  dataSrc.readFloat(rxBoneSpace);
  dataSrc.readFloat(ryBoneSpace);
  dataSrc.readFloat(rzBoneSpace);
  dataSrc.readFloat(rwBoneSpace);

  // get the parent bone id
  int parentId;
  dataSrc.readInteger(parentId);

  CalQuaternion rot(rx,ry,rz,rw);
  CalQuaternion rotbs(rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace);
  CalVector trans(tx,ty,tz);

  if (loadingMode & LOADER_ROTATE_X_AXIS)
  {
    if (parentId == -1) // only root bone necessary
    {
      // Root bone must have quaternion rotated
      CalQuaternion x_axis_90(0.7071067811f,0.0f,0.0f,0.7071067811f);
      rot *= x_axis_90;
      // Root bone must have translation rotated also
      trans *= x_axis_90;
    }
  }
  

  // check if an error happened
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // allocate a new core bone instance
  std::auto_ptr<CalCoreBone> pCoreBone( new(std::nothrow) CalCoreBone(strName) );
  if(pCoreBone.get() == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // set the parent of the bone
  pCoreBone->setParentId(parentId);

  // set all attributes of the bone
  pCoreBone->setTranslation(trans);
  pCoreBone->setRotation(rot);
  pCoreBone->setTranslationBoneSpace(CalVector(txBoneSpace, tyBoneSpace, tzBoneSpace));
  pCoreBone->setRotationBoneSpace(rotbs);

  // read the number of children
  int childCount;
  if(!dataSrc.readInteger(childCount) || (childCount < 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // load all children ids
  for(; childCount > 0; childCount--)
  {
    int childId;
    if(!dataSrc.readInteger(childId) || (childId < 0))
    {
      CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
      return 0;
    }

    pCoreBone->addChildId(childId);
  }

  return pCoreBone.release();
}

 /*****************************************************************************/
/** Loads a core keyframe instance.
  *
  * This function loads a core keyframe instance from a data source.
  *
  * @param dataSrc The data source to load the core keyframe instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core keyframe
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreKeyframe* CalLoader::loadCoreKeyframe(CalDataSource& dataSrc)
{
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // get the time of the keyframe
  float time;
  dataSrc.readFloat(time);

  // get the translation of the bone
  float tx, ty, tz;
  dataSrc.readFloat(tx);
  dataSrc.readFloat(ty);
  dataSrc.readFloat(tz);

  // get the rotation of the bone
  float rx, ry, rz, rw;
  dataSrc.readFloat(rx);
  dataSrc.readFloat(ry);
  dataSrc.readFloat(rz);
  dataSrc.readFloat(rw);

  // check if an error happened
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return false;
  }

  // allocate a new core keyframe instance
  CalCoreKeyframe *pCoreKeyframe = new(std::nothrow) CalCoreKeyframe();

  if(pCoreKeyframe == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core keyframe instance
  if(!pCoreKeyframe->create())
  {
    delete pCoreKeyframe;
    return 0;
  }
  // set all attributes of the keyframe
  pCoreKeyframe->setTime(time);
  pCoreKeyframe->setTranslation(CalVector(tx, ty, tz));
  pCoreKeyframe->setRotation(CalQuaternion(rx, ry, rz, rw));

  return pCoreKeyframe;
}

 /*****************************************************************************/
/** Loads a core compressed keyframe instance.
  *
  * This function loads a core compressed keyframe instance from a data source.
  *
  * @param dataSrc The data source to load the core compressed keyframe instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core keyframe
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreKeyframe *CalLoader::loadCompressedCoreKeyframe(CalDataSource& dataSrc, const CalVector& trackMinPt, const CalVector& trackScale, float trackDuration)
{
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // get the time of the keyframe
	unsigned short itime;
  dataSrc.readShort((short&)itime);
	float time = (itime / 65535.0f) * trackDuration;

  // get the translation of the bone
  float tx, ty, tz;

	unsigned pt;
	dataSrc.readInteger((int&)pt);

	unsigned ptx = pt & 0x7ff;
	unsigned pty = (pt >> 11) & 0x7ff;
	unsigned ptz = pt >> 22;

	tx = ptx * trackScale.x + trackMinPt.x;
	ty = pty * trackScale.y + trackMinPt.y;
	tz = ptz * trackScale.z + trackMinPt.z;

  // get the rotation of the bone
	short s0, s1, s2;
	dataSrc.readShort(s0);
	dataSrc.readShort(s1);
	dataSrc.readShort(s2);
	CalQuaternion quat;
	quat.decompress(s0, s1, s2);

  // check if an error happened
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // allocate a new core keyframe instance
  CalCoreKeyframe *pCoreKeyframe = new(std::nothrow) CalCoreKeyframe();

  if(pCoreKeyframe == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core keyframe instance
  if(!pCoreKeyframe->create())
  {
    delete pCoreKeyframe;
    return 0;
  }
  // set all attributes of the keyframe
  pCoreKeyframe->setTime(time);
  pCoreKeyframe->setTranslation(CalVector(tx, ty, tz));
	pCoreKeyframe->setRotation(quat);

  return pCoreKeyframe;
}

 /*****************************************************************************/
/** Loads a core submesh instance.
  *
  * This function loads a core submesh instance from a data source.
  *
  * @param dataSrc The data source to load the core submesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core submesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSubmesh *CalLoader::loadCoreSubmesh(CalDataSource& dataSrc)
{
	if(!dataSrc.ok())
	{
		dataSrc.setError();
		return 0;
	}

	// get the material thread id of the submesh
	int coreMaterialThreadId;
	dataSrc.readInteger(coreMaterialThreadId);

	// get the number of vertices, faces, level-of-details and springs
	int vertexCount;
	dataSrc.readInteger(vertexCount);

	int faceCount;
	dataSrc.readInteger(faceCount);

	int lodCount;
	dataSrc.readInteger(lodCount);

	int springCount;
	dataSrc.readInteger(springCount);

	// get the number of texture coordinates per vertex
	int textureCoordinateCount;
	dataSrc.readInteger(textureCoordinateCount);

	// check if an error happened
	if(!dataSrc.ok())
	{
		dataSrc.setError();
		return 0;
	}

	// allocate a new core submesh instance
	std::auto_ptr<CalCoreSubmesh> pCoreSubmesh( new(std::nothrow) CalCoreSubmesh() );
	if(pCoreSubmesh.get() == 0)
	{
		CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		return 0;
	}
	
	// set the LOD step count
	pCoreSubmesh->setLodCount(lodCount);

	// set the core material id
	pCoreSubmesh->setCoreMaterialThreadId(coreMaterialThreadId);

	// reserve memory for all the submesh data
	if(!pCoreSubmesh->reserve(vertexCount, textureCoordinateCount, faceCount, springCount))
	{
		CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		return 0;
	}

	// load the tangent space enable flags.
	int textureCoordinateId;
	for (textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; textureCoordinateId++)
	{
		pCoreSubmesh->enableTangents(textureCoordinateId, false);
	}

	// load all vertices and their influences
	int vertexId;
	for(vertexId = 0; vertexId < vertexCount; ++vertexId)
	{
		CalCoreSubmesh::Vertex vertex;

		// load data of the vertex
		dataSrc.readFloat(vertex.position.x);
		dataSrc.readFloat(vertex.position.y);
		dataSrc.readFloat(vertex.position.z);
		dataSrc.readFloat(vertex.normal.x);
		dataSrc.readFloat(vertex.normal.y);
		dataSrc.readFloat(vertex.normal.z);
		dataSrc.readInteger(vertex.collapseId);
		dataSrc.readInteger(vertex.faceCollapseCount);

		// check if an error happened
		if(!dataSrc.ok())
		{
			dataSrc.setError();
			return 0;
		}

		// load all texture coordinates of the vertex
		int textureCoordinateId;
		for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; ++textureCoordinateId)
		{
			CalCoreSubmesh::TextureCoordinate textureCoordinate;

			// load data of the influence
			dataSrc.readFloat(textureCoordinate.u);
			dataSrc.readFloat(textureCoordinate.v);

			if (loadingMode & LOADER_INVERT_V_COORD)
			{
				textureCoordinate.v = 1.0f - textureCoordinate.v;
			}

			// check if an error happened
			if(!dataSrc.ok())
			{
				dataSrc.setError();
				return 0;
			}

			// set texture coordinate in the core submesh instance
			pCoreSubmesh->setTextureCoordinate(vertexId, textureCoordinateId, textureCoordinate);
		}

		// get the number of influences
		int influenceCount;
		if(!dataSrc.readInteger(influenceCount) || (influenceCount < 0))
		{
			dataSrc.setError();
			return 0;
		}

		// reserve memory for the influences in the vertex
		vertex.vectorInfluence.reserve(influenceCount);
		vertex.vectorInfluence.resize(influenceCount);

		// load all influences of the vertex
		int influenceId;
		for(influenceId = 0; influenceId < influenceCount; ++influenceId)
		{
			// load data of the influence
			dataSrc.readInteger(vertex.vectorInfluence[influenceId].boneId),
			dataSrc.readFloat(vertex.vectorInfluence[influenceId].weight);

			// check if an error happened
			if(!dataSrc.ok())
			{
				dataSrc.setError();
				return 0;
			}
		}

		// set vertex in the core submesh instance
		pCoreSubmesh->setVertex(vertexId, vertex);

		// load the physical property of the vertex if there are springs in the core submesh
		if(springCount > 0)
		{
			CalCoreSubmesh::PhysicalProperty physicalProperty;

			// load data of the physical property
			dataSrc.readFloat(physicalProperty.weight);

			// check if an error happened
			if(!dataSrc.ok())
			{
				dataSrc.setError();
				return 0;
			}

			// set the physical property in the core submesh instance
			pCoreSubmesh->setPhysicalProperty(vertexId, physicalProperty);
		}
	}

	// load all springs
	int springId;
	for(springId = 0; springId < springCount; ++springId)
	{
		CalCoreSubmesh::Spring spring;

		// load data of the spring
		dataSrc.readInteger(spring.vertexId[0]);
		dataSrc.readInteger(spring.vertexId[1]);
		dataSrc.readFloat(spring.springCoefficient);
		dataSrc.readFloat(spring.idleLength);

		// check if an error happened
		if(!dataSrc.ok())
		{
			dataSrc.setError();
			return 0;
		}

		// set spring in the core submesh instance
		pCoreSubmesh->setSpring(springId, spring);
	}


	// load all faces
	int faceId;
	int justOnce = 0;
	bool flipModel = false;
	for(faceId = 0; faceId < faceCount; ++faceId)
	{
		CalCoreSubmesh::Face face;

		// load data of the face

		int tmp[4];
		dataSrc.readInteger(tmp[0]);
		dataSrc.readInteger(tmp[1]);
		dataSrc.readInteger(tmp[2]);

		if(sizeof(CalIndex)==2)
		{
			if(tmp[0]>65535 || tmp[1]>65535 || tmp[2]>65535)
			{      
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
				return 0;
			}
		}
		face.vertexId[0]=tmp[0];
		face.vertexId[1]=tmp[1];
		face.vertexId[2]=tmp[2];

		// check if an error happened
		if(!dataSrc.ok())
		{
			dataSrc.setError();
			return 0;
		}

		// check if left-handed coord system is used by the object
		// can be done only once since the object has one system for all faces
		if (justOnce==0)
		{
			// get vertexes of first face
			std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pCoreSubmesh->getVectorVertex();
			CalCoreSubmesh::Vertex& v1 = vectorVertex[tmp[0]];
			CalCoreSubmesh::Vertex& v2 = vectorVertex[tmp[1]];
			CalCoreSubmesh::Vertex& v3 = vectorVertex[tmp[2]];

			CalVector point1 = CalVector(v1.position.x, v1.position.y, v1.position.z);
			CalVector point2 = CalVector(v2.position.x, v2.position.y, v2.position.z);
			CalVector point3 = CalVector(v3.position.x, v3.position.y, v3.position.z);

			// gets vectors (v1-v2) and (v3-v2)
			CalVector vect1 = point1 - point2;
			CalVector vect2 = point3 - point2;

			// calculates normal of face
			CalVector cross = vect1 % vect2;
			float	crossLength = cross.length();
			if (crossLength == 0.0f)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
				return 0;
			}
			CalVector faceNormal = cross / crossLength;

			// compare the calculated normal with the normal of a vertex
			CalVector maxNorm = v1.normal;

			// if the two vectors point to the same direction then the poly needs flipping
			// so if the dot product > 0 it needs flipping
			if (faceNormal*maxNorm>0)
				flipModel = true;

			// flip the winding order if the loading flags request it
			if (loadingMode & LOADER_FLIP_WINDING)
				flipModel = !flipModel;

			justOnce = 1;
		}


		// flip if needed
		if (flipModel) 
		{
			tmp[3] = face.vertexId[1];
			face.vertexId[1]=face.vertexId[2];
			face.vertexId[2]=tmp[3];
		}

		// set face in the core submesh instance
		pCoreSubmesh->setFace(faceId, face);
	}

	return pCoreSubmesh.release();
}

 /*****************************************************************************/
/** Loads a core track instance.
  *
  * This function loads a core track instance from a data source.
  *
  * @param dataSrc The data source to load the core track instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core track
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreTrack *CalLoader::loadCoreTrack(CalDataSource& dataSrc, CalCoreSkeleton *skel, float duration, int flags)
{
  if(!dataSrc.ok())
  {
    dataSrc.setError();
    return 0;
  }

  // read the bone id
  int coreBoneId;
  if(!dataSrc.readInteger(coreBoneId) || (coreBoneId < 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

  // allocate a new core track instance
  std::auto_ptr<CalCoreTrack> pCoreTrack( new(std::nothrow) CalCoreTrack() );
  if(pCoreTrack.get() == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  // create the core track instance
  if(!pCoreTrack->create())
  {
    return 0;
  }

  // link the core track to the appropriate core bone instance
  pCoreTrack->setCoreBoneId(coreBoneId);

  // read the number of keyframes
  int keyframeCount;
  if(!dataSrc.readInteger(keyframeCount) || (keyframeCount <= 0))
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__);
    return 0;
  }

	CalVector minpt, scale;
	if(flags & 1)
	{
		dataSrc.readFloat(minpt.x);
		dataSrc.readFloat(minpt.y);
		dataSrc.readFloat(minpt.z);
		dataSrc.readFloat(scale.x);
		dataSrc.readFloat(scale.y);
		dataSrc.readFloat(scale.z);
	}

  // load all core keyframes
  int keyframeId;
  for(keyframeId = 0; keyframeId < keyframeCount; ++keyframeId)
  {
    // load the core keyframe
    CalCoreKeyframe *pCoreKeyframe;

		if(flags & 1)
		{
			pCoreKeyframe = loadCompressedCoreKeyframe(dataSrc, minpt, scale, duration);
		}
		else
		{
			pCoreKeyframe = loadCoreKeyframe(dataSrc);
		}

    if(pCoreKeyframe == 0)
    {
      pCoreTrack->destroy();
      return 0;
    }
    if(loadingMode & LOADER_ROTATE_X_AXIS)
    {
      // Check for anim rotation
      if (skel && skel->getCoreBone(coreBoneId)->getParentId() == -1)  // root bone
      {
        // rotate root bone quaternion
        CalQuaternion rot = pCoreKeyframe->getRotation();
        CalQuaternion x_axis_90(0.7071067811f,0.0f,0.0f,0.7071067811f);
        rot *= x_axis_90;
        pCoreKeyframe->setRotation(rot);
        // rotate root bone displacement
        CalVector vec = pCoreKeyframe->getTranslation();
				vec *= x_axis_90;
        pCoreKeyframe->setTranslation(vec);
      }
    }    

    // add the core keyframe to the core track instance
    pCoreTrack->addCoreKeyframe(pCoreKeyframe);
  }

  return pCoreTrack.release();
}


 /*****************************************************************************/
/** Loads a core animation instance from a XML file.
  *
  * This function loads a core animation instance from a XML file.
  *
  * @param strFilename The name of the file to load the core animation instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreAnimationPtr CalLoader::loadXmlCoreAnimation(const std::string& strFilename, CalCoreSkeleton *skel)
{
	std::stringstream str;
	TiXmlDocument doc(strFilename.c_str());
	if(!doc.LoadFile())
	{
		CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
		return 0;
	}

	TiXmlNode* node;

	TiXmlElement*animation = doc.FirstChildElement();
	if(!animation)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		return false;
	}

	int _cmpHEADER, _cmpMAGIC, _cmpANIMATION;
  	
	#if defined(FT_STRICMP)
		_cmpHEADER = FT_STRICMP(animation->Value(),"HEADER");
		_cmpMAGIC = FT_STRICMP(animation->Attribute("MAGIC"),Cal::ANIMATION_XMLFILE_MAGIC);
	#else
		_cmpHEADER = stricmp(animation->Value(),"HEADER");
		_cmpMAGIC = stricmp(animation->Attribute("MAGIC"),Cal::ANIMATION_XMLFILE_MAGIC);
	#endif

	if(_cmpHEADER==0)
	{
		if(_cmpMAGIC!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			return false;
		}

		if(atoi(animation->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
		{
			CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
			return false;
		}

		animation = animation->NextSiblingElement();
	}

	#if defined(FT_STRICMP)
		_cmpANIMATION = FT_STRICMP(animation->Value(),"ANIMATION");		
	#else
		_cmpANIMATION = stricmp(animation->Value(),"ANIMATION");
	#endif

	if(!animation || _cmpANIMATION!=0)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		return false;
	}

	#if defined(FT_STRICMP)
		_cmpMAGIC = FT_STRICMP(animation->Attribute("MAGIC"),Cal::ANIMATION_XMLFILE_MAGIC);
	#else
		_cmpMAGIC = stricmp(animation->Attribute("MAGIC"),Cal::ANIMATION_XMLFILE_MAGIC);
	#endif
	
	if(animation->Attribute("MAGIC") !=NULL && _cmpMAGIC!=0)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
		return false;
	}

	if(animation->Attribute("VERSION")!=NULL && atoi(animation->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
	{
		CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, strFilename);
		return false;
	}

	int trackCount= atoi(animation->Attribute("NUMTRACKS"));
	float duration= (float) atof(animation->Attribute("DURATION"));

	// allocate a new core animation instance
	CalCoreAnimation *pCoreAnimation;
	pCoreAnimation = new(std::nothrow) CalCoreAnimation();
	if(pCoreAnimation == 0)
	{
		CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		return 0;
	}

	// check for a valid duration
	if(duration <= 0.0f)
	{
		CalError::setLastError(CalError::INVALID_ANIMATION_DURATION, __FILE__, __LINE__, strFilename);
		return 0;
	}

	// set the duration in the core animation instance
	pCoreAnimation->setDuration(duration);
	TiXmlElement* track=animation->FirstChildElement();

	// load all core bones
	int trackId;
	for(trackId = 0; trackId < trackCount; ++trackId)
	{
		int _cmpTRACK;
  	
		#if defined(FT_STRICMP)
		_cmpTRACK = FT_STRICMP(track->Value(),"TRACK");
		#else
		_cmpTRACK = stricmp(track->Value(),"TRACK");
		#endif
		
		if(!track || _cmpTRACK!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			return 0;
		}

		CalCoreTrack *pCoreTrack;

		pCoreTrack = new(std::nothrow) CalCoreTrack();
		if(pCoreTrack == 0)
		{
			CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
			return 0;
		}

		// create the core track instance
		if(!pCoreTrack->create())
		{
			delete pCoreTrack;
			return 0;
		}

		int coreBoneId = atoi(track->Attribute("BONEID"));

		// link the core track to the appropriate core bone instance
		pCoreTrack->setCoreBoneId(coreBoneId);

		// read the number of keyframes
		int keyframeCount= atoi(track->Attribute("NUMKEYFRAMES"));

		if(keyframeCount <= 0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
			return 0;
		}

		TiXmlElement* keyframe= track->FirstChildElement();

		// load all core keyframes
		int keyframeId;
		for(keyframeId = 0; keyframeId < keyframeCount; ++keyframeId)
		{
			int _cmpKEYFRAME, _cmpTRANSLATION, _cmpROTATION;
  	
			#if defined(FT_STRICMP)
				_cmpKEYFRAME = FT_STRICMP(keyframe->Value(),"KEYFRAME");
			#else
				_cmpKEYFRAME = stricmp(keyframe->Value(),"KEYFRAME");
			#endif

			// load the core keyframe
			if(!keyframe|| _cmpKEYFRAME!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				return 0;
			}

			float time= (float) atof(keyframe->Attribute("TIME"));

			TiXmlElement* translation = keyframe->FirstChildElement();
			
			#if defined(FT_STRICMP)
				_cmpTRANSLATION = FT_STRICMP(translation->Value(),"TRANSLATION");
			#else
				_cmpTRANSLATION = stricmp(translation->Value(),"TRANSLATION");
			#endif
			
			if(!translation || _cmpTRANSLATION!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				return 0;
			}

			float tx, ty, tz;

			node = translation->FirstChild();
			if(!node)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				return 0;
			}

			TiXmlText* translationdata = node->ToText();
			if(!translationdata)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				return 0;
			}
			str.clear();
			str << translationdata->Value();
			str >> tx >> ty >> tz;  

			TiXmlElement* rotation = translation->NextSiblingElement();

			#if defined(FT_STRICMP)
				_cmpROTATION = FT_STRICMP(rotation->Value(),"ROTATION");
			#else
				_cmpROTATION = stricmp(rotation->Value(),"ROTATION");
			#endif
			if(!rotation || _cmpROTATION!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				return 0;
			}

			float rx, ry, rz, rw;

			node = rotation->FirstChild();
			if(!node)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				return 0;
			}
			TiXmlText* rotationdata = node->ToText();
			if(!rotationdata)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, strFilename);
				return 0;
			}
			str.clear();
			str << rotationdata->Value();
			str >> rx >> ry >> rz >> rw;  

			// allocate a new core keyframe instance

			CalCoreKeyframe *pCoreKeyframe;
			pCoreKeyframe = new(std::nothrow) CalCoreKeyframe();
			if(pCoreKeyframe == 0)
			{
				CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
				return 0;
			}

			// create the core keyframe instance
			if(!pCoreKeyframe->create())
			{
				return 0;			  
			}
			// set all attributes of the keyframe
			pCoreKeyframe->setTime(time);
			pCoreKeyframe->setTranslation(CalVector(tx, ty, tz));
			pCoreKeyframe->setRotation(CalQuaternion(rx, ry, rz, rw));

			if (loadingMode & LOADER_ROTATE_X_AXIS)
			{
				// Check for anim rotation
				if (skel && skel->getCoreBone(coreBoneId)->getParentId() == -1)  // root bone
				{
					// rotate root bone quaternion
					CalQuaternion rot = pCoreKeyframe->getRotation();
					CalQuaternion x_axis_90(0.7071067811f,0.0f,0.0f,0.7071067811f);
					rot *= x_axis_90;
					pCoreKeyframe->setRotation(rot);
					// rotate root bone displacement
					CalVector trans = pCoreKeyframe->getTranslation();
					trans *= x_axis_90;
					pCoreKeyframe->setTranslation(trans);
				}
			}    

			// add the core keyframe to the core track instance
			pCoreTrack->addCoreKeyframe(pCoreKeyframe);
			keyframe = keyframe->NextSiblingElement();
		}

		pCoreAnimation->addCoreTrack(pCoreTrack);	  
		track=track->NextSiblingElement();
	}

	// explicitly close the file
	doc.Clear();

	return pCoreAnimation;
}

 /*****************************************************************************/
/** Loads a core material instance from a XML file.
  *
  * This function loads a core material instance from a XML file.
  *
  * @param strFilename The name of the file to load the core material instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/


CalCoreMaterialPtr CalLoader::loadXmlCoreMaterial(const std::string& strFilename)
{
  std::stringstream str;
  TiXmlDocument doc(strFilename.c_str());
  if(!doc.LoadFile())
  {
    CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
    return 0;
  }

  return loadXmlCoreMaterial( doc );
}

 /*****************************************************************************/
/** Loads a core mesh instance from a Xml file.
  *
  * This function loads a core mesh instance from a Xml file.
  *
  * @param strFilename The name of the file to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreMeshPtr CalLoader::loadXmlCoreMesh(const std::string& strFilename)
{
	std::stringstream str;
	TiXmlDocument doc(strFilename.c_str());
	if(!doc.LoadFile())
	{
		CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
		return 0;
	}

	return loadXmlCoreMesh( doc );
}

 /*****************************************************************************/
/** Loads a core skeleton instance from a XML file.
  *
  * This function loads a core skeleton instance from a XML file.
  *
  * @param strFilename The name of the file to load the core skeleton instance
  *                    from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/

CalCoreSkeletonPtr CalLoader::loadXmlCoreSkeleton(const std::string& strFilename)
{
	TiXmlDocument doc(strFilename.c_str());
	if(!doc.LoadFile())
	{
		CalError::setLastError(CalError::FILE_NOT_FOUND, __FILE__, __LINE__, strFilename);
		return 0;
	}

	return loadXmlCoreSkeleton( doc );
}

 /*****************************************************************************/
/** Loads a core material instance from an XML document.
  *
  * This function loads a core material instance from an XML document.
  *
  * @param doc			The document to load the core material instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happened
  *****************************************************************************/
CalCoreMaterialPtr CalLoader::loadXmlCoreMaterial(TiXmlDocument& doc)
{
  std::stringstream str;
  int r,g,b,a;

  TiXmlNode* node;

  TiXmlElement*material = doc.FirstChildElement();
  if(!material)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }

	int _cmpHEADER, _cmpMAGIC, _cmpMATERIAL, _cmpAMBIENT, _cmpDIFFUSE, _cmpSPECULAR, _cmpSHININESS, _cmpMAP;
  	
	#if defined(FT_STRICMP)
		_cmpHEADER = FT_STRICMP(material->Value(),"HEADER");
		_cmpMAGIC = FT_STRICMP(material->Attribute("MAGIC"),Cal::MATERIAL_XMLFILE_MAGIC);
		_cmpMATERIAL = FT_STRICMP(material->Value(),"MATERIAL");		
	#else
		_cmpHEADER = stricmp(material->Value(),"HEADER");
		_cmpMAGIC = stricmp(material->Attribute("MAGIC"),Cal::MATERIAL_XMLFILE_MAGIC);
		_cmpMATERIAL = stricmp(material->Value(),"MATERIAL");
	#endif

  if(_cmpHEADER==0)
  {
    if(_cmpMAGIC!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
          return false;
    }

    if(atoi(material->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
    {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, doc.Value());
          return false;
    }

    material = material->NextSiblingElement();
  }

  if(!material|| _cmpMATERIAL!=0)
  {
	  CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
      return false;
  }
	#if defined(FT_STRICMP)
		_cmpMAGIC = FT_STRICMP(material->Attribute("MAGIC"),Cal::MATERIAL_XMLFILE_MAGIC);
	#else
		_cmpMAGIC = stricmp(material->Attribute("MAGIC"),Cal::MATERIAL_XMLFILE_MAGIC);
	#endif

  if(material->Attribute("MAGIC")!=NULL && _cmpMAGIC!=0)
  {
    CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }

  if(material->Attribute("VERSION") != NULL && atoi(material->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
  {
    CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, doc.Value());
        return false;
  }

  CalCoreMaterialPtr pCoreMaterial( new(std::nothrow) CalCoreMaterial() );
  if(pCoreMaterial.get() == 0)
  {
    CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
    return 0;
  }

  TiXmlElement* ambient = material->FirstChildElement();
 	
  #if defined(FT_STRICMP)
	_cmpAMBIENT = FT_STRICMP(ambient->Value(),"AMBIENT");
  #else
	_cmpAMBIENT = stricmp(ambient->Value(),"AMBIENT");
  #endif

  if(!ambient || _cmpAMBIENT!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }

  CalCoreMaterial::Color ambientColor; 
  node = ambient->FirstChild();
  if(!node)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }
  TiXmlText* ambientdata = node->ToText();
  if(!ambientdata)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }
  str << ambientdata->Value();
  str >> r >> g >> b >> a;
  ambientColor.red = (unsigned char)r;
  ambientColor.green = (unsigned char)g;
  ambientColor.blue = (unsigned char)b;
  ambientColor.alpha = (unsigned char)a; 

  TiXmlElement* diffuse = ambient->NextSiblingElement();
 	
  #if defined(FT_STRICMP)
	_cmpDIFFUSE = FT_STRICMP(diffuse->Value(),"DIFFUSE");
  #else
	_cmpDIFFUSE = stricmp(diffuse->Value(),"DIFFUSE");
  #endif

  if(!diffuse || _cmpDIFFUSE!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }

  CalCoreMaterial::Color diffuseColor; 
  node = diffuse->FirstChild();
  if(!node)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }
  TiXmlText* diffusedata = node->ToText();
  if(!diffusedata)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }
  str.clear();
  str << diffusedata->Value();
  str >> r >> g >> b >> a;
  diffuseColor.red = (unsigned char)r;
  diffuseColor.green = (unsigned char)g;
  diffuseColor.blue = (unsigned char)b;
  diffuseColor.alpha = (unsigned char)a;
  

  TiXmlElement* specular = diffuse->NextSiblingElement();

 	
  #if defined(FT_STRICMP)
	_cmpSPECULAR = FT_STRICMP(specular->Value(),"SPECULAR");
  #else
	_cmpSPECULAR = stricmp(specular->Value(),"SPECULAR");
  #endif

  if(!specular|| _cmpSPECULAR!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }

  CalCoreMaterial::Color specularColor; 
  node = specular->FirstChild();
  if(!node)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }
  TiXmlText* speculardata = node->ToText();
  if(!speculardata)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }
  str.clear();
  str << speculardata->Value();
  str >> r >> g >> b >> a;
  specularColor.red = (unsigned char)r;
  specularColor.green = (unsigned char)g;
  specularColor.blue = (unsigned char)b;
  specularColor.alpha = (unsigned char)a;


  TiXmlElement* shininess = specular->NextSiblingElement();
 	
  #if defined(FT_STRICMP)
	_cmpSHININESS = FT_STRICMP(shininess->Value(),"SHININESS");
  #else
	_cmpSHININESS = stricmp(shininess->Value(),"SHININESS");
  #endif

  if(!shininess|| _cmpSHININESS!=0)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }

  float fshininess;
  node = shininess->FirstChild();
  if(!node)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
  }
  TiXmlText* shininessdata = node->ToText();
  if(!shininessdata)
  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
         return false;
  }
  fshininess = (float)atof(shininessdata->Value());

  // set the colors and the shininess
  pCoreMaterial->setAmbientColor(ambientColor);
  pCoreMaterial->setDiffuseColor(diffuseColor);
  pCoreMaterial->setSpecularColor(specularColor);
  pCoreMaterial->setShininess(fshininess);
  
  std::vector<std::string> MatFileName;

  TiXmlElement* map;

  for( map = shininess->NextSiblingElement();map;map = map->NextSiblingElement() )
  {
 	
	  #if defined(FT_STRICMP)
		_cmpMAP = FT_STRICMP(map->Value(),"MAP");
	  #else
		_cmpMAP = stricmp(map->Value(),"MAP");
	  #endif

	  if(!map|| _cmpMAP!=0)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
	  }
	  

	  node= map->FirstChild();
	  if(!node)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
	  }

	  TiXmlText* mapfile = node->ToText();
	  if(!mapfile)
	  {
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
        return false;
	  }

	  MatFileName.push_back(mapfile->Value());
  }

  pCoreMaterial->reserve(MatFileName.size());



  for (unsigned int mapId=0; mapId < MatFileName.size(); ++mapId)
  {
	  CalCoreMaterial::Map Map;
	  // initialize the user data
	  Map.userData = 0;

	  Map.strFilename= MatFileName[mapId];	  

	  // set map in the core material instance
	  pCoreMaterial->setMap(mapId, Map);
  }

  doc.Clear();
  
  return pCoreMaterial;
}


 /*****************************************************************************/
/** Loads a core mesh instance from a Xml file.
  *
  * This function loads a core mesh instance from a Xml document.
  *
  * @param doc The document to load the core mesh instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happened
  *****************************************************************************/
CalCoreMeshPtr CalLoader::loadXmlCoreMesh(TiXmlDocument& doc)
{
   std::stringstream str;

	TiXmlNode* node;

	TiXmlElement*mesh = doc.FirstChildElement();
	if(!mesh)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
		return false;
	}
	
	int _cmpHEADER, _cmpMAGIC, _cmpMESH, _cmpSUBMESH, _cmpVERTEX, _cmpPOS, _cmpNORM, _cmpCOLLAPSEID,
		_cmpCOLLAPSECOUNT, _cmpTEXCOORD, _cmpINFLUENCE, _cmpPHYSIQUE, _cmpSPRING, _cmpFACE;
  	
	#if defined(FT_STRICMP)
		_cmpHEADER = FT_STRICMP(mesh->Value(),"HEADER");
		_cmpMAGIC = FT_STRICMP(mesh->Attribute("MAGIC"),Cal::MESH_XMLFILE_MAGIC);
	#else
		_cmpHEADER = stricmp(mesh->Value(),"HEADER");
		_cmpMAGIC = stricmp(mesh->Attribute("MAGIC"),Cal::MESH_XMLFILE_MAGIC);
	#endif
	
	if(_cmpHEADER==0)
	{
		if(_cmpMAGIC!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}

		if(atoi(mesh->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
		{
			CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, doc.Value());
			return false;
		}

		mesh = mesh->NextSiblingElement();
	}

	#if defined(FT_STRICMP)
		_cmpMESH = FT_STRICMP(mesh->Value(),"MESH");
		_cmpMAGIC = FT_STRICMP(mesh->Attribute("MAGIC"),Cal::MESH_XMLFILE_MAGIC);
	#else
		_cmpMESH = stricmp(mesh->Value(),"MESH");
		_cmpMAGIC = stricmp(mesh->Attribute("MAGIC"),Cal::MESH_XMLFILE_MAGIC);
	#endif

	if(!mesh || _cmpMESH!=0)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
		return false;
	}

	if(mesh->Attribute("MAGIC")!=NULL && _cmpMAGIC!=0)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
		return false;
	}

	if(mesh->Attribute("VERSION")!=NULL && atoi(mesh->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
	{
		CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, doc.Value());
		return false;
	}

	// get the number of submeshes
	int submeshCount = atoi(mesh->Attribute("NUMSUBMESH"));

	// allocate a new core mesh instance
	CalCoreMeshPtr pCoreMesh = new(std::nothrow) CalCoreMesh;
	if(!pCoreMesh)
	{
		CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		return 0;
	}

	TiXmlElement*submesh = mesh->FirstChildElement();

	// load all core submeshes
	int submeshId;
	for(submeshId = 0; submeshId < submeshCount; ++submeshId)
	{
		#if defined(FT_STRICMP)
			_cmpSUBMESH = FT_STRICMP(submesh->Value(),"SUBMESH");
		#else
			_cmpSUBMESH = stricmp(submesh->Value(),"SUBMESH");
		#endif
		
		if(!submesh || _cmpSUBMESH!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}

		// get the material thread id of the submesh
		int coreMaterialThreadId = atoi(submesh->Attribute("MATERIAL"));

		// get the number of vertices, faces, level-of-details and springs
		int vertexCount = atoi(submesh->Attribute("NUMVERTICES"));

		int faceCount = atoi(submesh->Attribute("NUMFACES"));

		int lodCount = atoi(submesh->Attribute("NUMLODSTEPS"));

		int springCount = atoi(submesh->Attribute("NUMSPRINGS"));

		int textureCoordinateCount = atoi(submesh->Attribute("NUMTEXCOORDS"));

		// allocate a new core submesh instance
		std::auto_ptr<CalCoreSubmesh> pCoreSubmesh( new(std::nothrow) CalCoreSubmesh() );
		if (pCoreSubmesh.get() == 0)
		{
			CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
			return 0;
		}

		// set the LOD step count
		pCoreSubmesh->setLodCount(lodCount);

		// set the core material id
		pCoreSubmesh->setCoreMaterialThreadId(coreMaterialThreadId);

		// reserve memory for all the submesh data
		if(!pCoreSubmesh->reserve(vertexCount, textureCoordinateCount, faceCount, springCount))
		{
			CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__, doc.Value());
			return 0;
		}

		TiXmlElement *vertex = submesh->FirstChildElement();

		// load all vertices and their influences
		int vertexId;
		for(vertexId = 0; vertexId < vertexCount; ++vertexId)
		{
			#if defined(FT_STRICMP)
				_cmpVERTEX = FT_STRICMP(vertex->Value(),"VERTEX");
			#else
				_cmpVERTEX = stricmp(vertex->Value(),"VERTEX");
			#endif

			if(!vertex || _cmpVERTEX!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}      

			CalCoreSubmesh::Vertex Vertex;

			TiXmlElement *pos= vertex->FirstChildElement();

			#if defined(FT_STRICMP)
				_cmpPOS = FT_STRICMP(pos->Value(),"POS");
			#else
				_cmpPOS = stricmp(pos->Value(),"POS");
			#endif
			
			if(!pos || _cmpPOS!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}

			node = pos->FirstChild();
			if(!node)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}      
			TiXmlText* posdata = node->ToText();
			if(!posdata)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}
			str.clear();
			str << posdata->Value();
			str >> Vertex.position.x >> Vertex.position.y >> Vertex.position.z;  

			TiXmlElement *norm= pos->NextSiblingElement();

			#if defined(FT_STRICMP)
				_cmpNORM = FT_STRICMP(norm->Value(),"NORM");
			#else
				_cmpNORM = stricmp(norm->Value(),"NORM");
			#endif

			if(!norm || _cmpNORM!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}

			node = norm->FirstChild();
			if(!norm)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}
			TiXmlText* normdata = node->ToText();
			if(!normdata)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}
			str.clear();
			str << normdata->Value();
			str >> Vertex.normal.x >> Vertex.normal.y >> Vertex.normal.z;

			TiXmlElement *collapse= norm->NextSiblingElement();

			#if defined(FT_STRICMP)
				_cmpCOLLAPSEID = FT_STRICMP(collapse->Value(),"COLLAPSEID");
			#else
				_cmpCOLLAPSEID = stricmp(collapse->Value(),"COLLAPSEID");
			#endif

			if(collapse && _cmpCOLLAPSEID==0)
			{
				node = collapse->FirstChild();
				if(!node)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				TiXmlText* collapseid = node->ToText();
				if(!collapseid)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				Vertex.collapseId = atoi(collapseid->Value());

				TiXmlElement *collapseCount= collapse->NextSiblingElement();

				#if defined(FT_STRICMP)
					_cmpCOLLAPSECOUNT = FT_STRICMP(collapseCount->Value(),"COLLAPSECOUNT");
				#else
					_cmpCOLLAPSECOUNT = stricmp(collapseCount->Value(),"COLLAPSECOUNT");
				#endif


				if(!collapseCount|| _cmpCOLLAPSECOUNT!=0)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}

				node = collapseCount->FirstChild();
				if(!node)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				TiXmlText* collapseCountdata = node->ToText();
				if(!collapseCountdata)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				Vertex.faceCollapseCount= atoi(collapseCountdata->Value());
				collapse = collapseCount->NextSiblingElement();        
			}
			else
			{
				Vertex.collapseId=-1;
				Vertex.faceCollapseCount=0;
			}


			TiXmlElement *texcoord = collapse;

			// load all texture coordinates of the vertex
			int textureCoordinateId;
			for(textureCoordinateId = 0; textureCoordinateId < textureCoordinateCount; ++textureCoordinateId)
			{
				CalCoreSubmesh::TextureCoordinate textureCoordinate;
				// load data of the influence
				#if defined(FT_STRICMP)
					_cmpTEXCOORD = FT_STRICMP(texcoord->Value(),"TEXCOORD");
				#else
					_cmpTEXCOORD = stricmp(texcoord->Value(),"TEXCOORD");
				#endif

				if(!texcoord || _cmpTEXCOORD!=0)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}

				node = texcoord->FirstChild();
				if(!node)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				TiXmlText* texcoorddata = node->ToText();
				if(!texcoorddata)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				str.clear();
				str << texcoorddata->Value();
				str >> textureCoordinate.u >> textureCoordinate.v;

				if (loadingMode & LOADER_INVERT_V_COORD)
				{
					textureCoordinate.v = 1.0f - textureCoordinate.v;
				}

				// set texture coordinate in the core submesh instance
				pCoreSubmesh->setTextureCoordinate(vertexId, textureCoordinateId, textureCoordinate);
				texcoord = texcoord->NextSiblingElement();
			}

			// get the number of influences
			int influenceCount= atoi(vertex->Attribute("NUMINFLUENCES"));

			if(influenceCount < 0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return 0;
			}

			// reserve memory for the influences in the vertex
			Vertex.vectorInfluence.reserve(influenceCount);
			Vertex.vectorInfluence.resize(influenceCount);

			TiXmlElement *influence = texcoord;

			// load all influences of the vertex
			int influenceId;
			for(influenceId = 0; influenceId < influenceCount; ++influenceId)
			{

				#if defined(FT_STRICMP)
					_cmpINFLUENCE = FT_STRICMP(influence->Value(),"INFLUENCE");
				#else
					_cmpINFLUENCE = stricmp(influence->Value(),"INFLUENCE");
				#endif

				if(!influence || _cmpINFLUENCE!=0)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}

				node = influence->FirstChild();
				if(!node)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				TiXmlText* influencedata = node->ToText();
				if(!influencedata)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}

				Vertex.vectorInfluence[influenceId].boneId = atoi(influence->Attribute("ID"));

				Vertex.vectorInfluence[influenceId].weight = (float) atof(influencedata->Value());

				influence=influence->NextSiblingElement();    
			}

			// set vertex in the core submesh instance
			pCoreSubmesh->setVertex(vertexId, Vertex);

			TiXmlElement *physique = influence;



			// load the physical property of the vertex if there are springs in the core submesh
			if(springCount > 0)
			{
				CalCoreSubmesh::PhysicalProperty physicalProperty;

				#if defined(FT_STRICMP)
					_cmpPHYSIQUE = FT_STRICMP(physique->Value(),"PHYSIQUE");
				#else
					_cmpPHYSIQUE = stricmp(physique->Value(),"PHYSIQUE");
				#endif

				if(!physique || _cmpPHYSIQUE!=0)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				node = physique->FirstChild();
				if(!node)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}
				TiXmlText* physiquedata = node->ToText();
				if(!physiquedata)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return false;
				}

				physicalProperty.weight = (float) atof(physiquedata->Value());

				// set the physical property in the core submesh instance
				pCoreSubmesh->setPhysicalProperty(vertexId, physicalProperty);          

			}


			vertex = vertex->NextSiblingElement();
		}

		TiXmlElement *spring= vertex;

		// load all springs
		int springId;
		for(springId = 0; springId < springCount; ++springId)
		{
			CalCoreSubmesh::Spring Spring;

			#if defined(FT_STRICMP)
				_cmpSPRING = FT_STRICMP(spring->Value(),"SPRING");
			#else
				_cmpSPRING = stricmp(spring->Value(),"SPRING");
			#endif

			if(!spring || _cmpSPRING!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}
			str.clear();
			str << spring->Attribute("VERTEXID");
			str >> Spring.vertexId[0] >> Spring.vertexId[1];
			Spring.springCoefficient = (float) atof(spring->Attribute("COEF"));
			Spring.idleLength = (float) atof(spring->Attribute("LENGTH"));

			// set spring in the core submesh instance
			pCoreSubmesh->setSpring(springId, Spring);
			spring = spring->NextSiblingElement();
		}

		TiXmlElement *face = spring;

		// load all faces
		int faceId;
		for(faceId = 0; faceId < faceCount; ++faceId)
		{
			CalCoreSubmesh::Face Face;

			#if defined(FT_STRICMP)
				_cmpFACE = FT_STRICMP(face->Value(),"FACE");
			#else
				_cmpFACE = stricmp(face->Value(),"FACE");
			#endif

			if(!face || _cmpFACE!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}

			int tmp[3];

			// load data of the face

			str.clear();
			str << face->Attribute("VERTEXID");
			str >> tmp[0] >> tmp [1] >> tmp[2];

			if(sizeof(CalIndex)==2)
			{
				if(tmp[0]>65535 || tmp[1]>65535 || tmp[2]>65535)
				{
					CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
					return 0;
				}
			}
			Face.vertexId[0]=tmp[0];
			Face.vertexId[1]=tmp[1];
			Face.vertexId[2]=tmp[2];

			pCoreSubmesh->setFace(faceId, Face);

			face=face->NextSiblingElement();
		}
		submesh=submesh->NextSiblingElement();

		// add the core submesh to the core mesh instance
		pCoreMesh->addCoreSubmesh(pCoreSubmesh.release());
	}

	// explicitly close the file
	doc.Clear();
	return pCoreMesh;
}



 /*****************************************************************************/
/** Loads a core skeleton instance from a XML document.
  *
  * This function loads a core skeleton instance from a XML document.
  *
  * @param doc		The document to load the core skeleton instance from.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happened
  *****************************************************************************/
CalCoreSkeletonPtr CalLoader::loadXmlCoreSkeleton(cal3d::TiXmlDocument& doc)
{
	std::stringstream str;

	TiXmlNode* node;
	TiXmlElement*skeleton = doc.FirstChildElement();
	if(!skeleton)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
		return 0;
	}

	int _cmpHEADER, _cmpMAGIC, _cmpSKELETON, _cmpBONE, _cmpTRANSLATION, _cmpROTATION, _cmpLOCALTRANSLATION, _cmpLOCALROTATION,
		_cmpPARENTID, _cmpCHILDID;
  	
	#if defined(FT_STRICMP)
		_cmpHEADER = FT_STRICMP(skeleton->Value(),"HEADER");
		_cmpMAGIC = FT_STRICMP(skeleton->Attribute("MAGIC"),Cal::SKELETON_XMLFILE_MAGIC);
	#else
		_cmpHEADER = stricmp(skeleton->Value(),"HEADER");
		_cmpMAGIC = stricmp(skeleton->Attribute("MAGIC"),Cal::SKELETON_XMLFILE_MAGIC);
	#endif

	if(_cmpHEADER==0)
	{
		if(_cmpMAGIC!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}

		if(atoi(skeleton->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
		{
			CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, doc.Value());
			return false;
		}

		skeleton = skeleton->NextSiblingElement();
	}

	#if defined(FT_STRICMP)
		_cmpSKELETON = FT_STRICMP(skeleton->Value(),"SKELETON");
		_cmpMAGIC = FT_STRICMP(skeleton->Attribute("MAGIC"),Cal::SKELETON_XMLFILE_MAGIC);
	#else
		_cmpSKELETON = stricmp(skeleton->Value(),"SKELETON");
		_cmpMAGIC = stricmp(skeleton->Attribute("MAGIC"),Cal::SKELETON_XMLFILE_MAGIC);
	#endif

	if(!skeleton || _cmpSKELETON!=0)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
		return false;
	}

	if(skeleton->Attribute("MAGIC")!=NULL && _cmpMAGIC!=0)
	{
		CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
		return false;
	}

	if(skeleton->Attribute("VERSION")!=NULL && atoi(skeleton->Attribute("VERSION")) < Cal::EARLIEST_COMPATIBLE_FILE_VERSION )
	{
		CalError::setLastError(CalError::INCOMPATIBLE_FILE_VERSION, __FILE__, __LINE__, doc.Value());
		return false;
	}


	// allocate a new core skeleton instance
	CalCoreSkeletonPtr pCoreSkeleton = new(std::nothrow) CalCoreSkeleton();
	if(!pCoreSkeleton)
	{
		CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
		return 0;
	}

	TiXmlElement* bone;
	for( bone = skeleton->FirstChildElement();bone;bone = bone->NextSiblingElement() )
	{
		#if defined(FT_STRICMP)
			_cmpBONE = FT_STRICMP(bone->Value(),"BONE");
		#else
			_cmpBONE = stricmp(bone->Value(),"BONE");
		#endif

		if(_cmpBONE!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}    

		std::string strName=bone->Attribute("NAME");


		// get the translation of the bone

		TiXmlElement* translation = bone->FirstChildElement();

		#if defined(FT_STRICMP)
			_cmpTRANSLATION = FT_STRICMP(translation->Value(),"TRANSLATION");
		#else
			_cmpTRANSLATION = stricmp(translation->Value(),"TRANSLATION");
		#endif

		if(!translation || _cmpTRANSLATION!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}

		float tx, ty, tz;

		node = translation->FirstChild();
		if(!node)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}    
		TiXmlText* translationdata = node->ToText();
		if(!translationdata)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}    
		str.clear();
		str << translationdata->Value();
		str >> tx >> ty >> tz;

		// get the rotation of the bone

		TiXmlElement* rotation = translation->NextSiblingElement();

		#if defined(FT_STRICMP)
			_cmpROTATION = FT_STRICMP(rotation->Value(),"ROTATION");
		#else
			_cmpROTATION = stricmp(rotation->Value(),"ROTATION");
		#endif

		if(!rotation || _cmpROTATION!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}

		float rx, ry, rz, rw;

		node = rotation->FirstChild();
		if(!node)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}
		TiXmlText* rotationdata = node->ToText();
		if(!rotationdata)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}
		str.clear();
		str << rotationdata->Value();
		str >> rx >> ry >> rz >> rw;    

		// get the bone space translation of the bone


		TiXmlElement* translationBoneSpace = rotation->NextSiblingElement();

		#if defined(FT_STRICMP)
			_cmpLOCALTRANSLATION = FT_STRICMP(translationBoneSpace->Value(),"LOCALTRANSLATION");
		#else
			_cmpLOCALTRANSLATION = stricmp(translationBoneSpace->Value(),"LOCALTRANSLATION");
		#endif

		if(!rotation || _cmpLOCALTRANSLATION!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}

		float txBoneSpace, tyBoneSpace, tzBoneSpace;

		node = translationBoneSpace->FirstChild();
		if(!node)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}
		TiXmlText* translationBoneSpacedata = node->ToText();
		if(!translationBoneSpacedata)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}
		str.clear();
		str << translationBoneSpacedata->Value();
		str >> txBoneSpace >> tyBoneSpace >> tzBoneSpace;

		// get the bone space rotation of the bone

		TiXmlElement* rotationBoneSpace = translationBoneSpace->NextSiblingElement();

		#if defined(FT_STRICMP)
			_cmpLOCALROTATION = FT_STRICMP(rotationBoneSpace->Value(),"LOCALROTATION");
		#else
			_cmpLOCALROTATION = stricmp(rotationBoneSpace->Value(),"LOCALROTATION");
		#endif

		if(!rotationBoneSpace || _cmpLOCALROTATION!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}

		float rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace;

		node = rotationBoneSpace->FirstChild();
		if(!node)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}
		TiXmlText* rotationBoneSpacedata = node->ToText();
		if(!rotationBoneSpacedata)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}
		str.clear();
		str << rotationBoneSpacedata->Value();
		str >> rxBoneSpace >> ryBoneSpace >> rzBoneSpace >> rwBoneSpace;

		// get the parent bone id

		TiXmlElement* parent = rotationBoneSpace->NextSiblingElement();

		#if defined(FT_STRICMP)
			_cmpPARENTID = FT_STRICMP(parent->Value(),"PARENTID");
		#else
			_cmpPARENTID = stricmp(parent->Value(),"PARENTID");
		#endif

		if(!parent || _cmpPARENTID!=0)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}


		int parentId;

		node = parent->FirstChild();
		if(!node)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}
		TiXmlText* parentid = node->ToText();
		if(!parentid)
		{
			CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
			return false;
		}
		parentId = atoi(parentid->Value());

		// allocate a new core bone instance
		std::auto_ptr<CalCoreBone> pCoreBone( new(std::nothrow) CalCoreBone(strName) );
		if(pCoreBone.get() == 0)
		{
			CalError::setLastError(CalError::MEMORY_ALLOCATION_FAILED, __FILE__, __LINE__);
			return 0;
		}

		// set the parent of the bone
		pCoreBone->setParentId(parentId);

		// set all attributes of the bone

		CalVector trans = CalVector(tx, ty, tz);
		CalQuaternion rot = CalQuaternion(rx, ry, rz, rw);

		if (loadingMode & LOADER_ROTATE_X_AXIS)
		{
			if (parentId == -1) // only root bone necessary
			{
				// Root bone must have quaternion rotated
				CalQuaternion x_axis_90(0.7071067811f,0.0f,0.0f,0.7071067811f);
				rot *= x_axis_90;
				// Root bone must have translation rotated also
				trans *= x_axis_90;
			}
		}    


		pCoreBone->setTranslation(trans);
		pCoreBone->setRotation(rot);
		pCoreBone->setTranslationBoneSpace(CalVector(txBoneSpace, tyBoneSpace, tzBoneSpace));
		pCoreBone->setRotationBoneSpace(CalQuaternion(rxBoneSpace, ryBoneSpace, rzBoneSpace, rwBoneSpace));


		TiXmlElement* child;
		for( child = parent->NextSiblingElement();child;child = child->NextSiblingElement() )
		{
			#if defined(FT_STRICMP)
				_cmpCHILDID = FT_STRICMP(child->Value(),"CHILDID");
			#else
				_cmpCHILDID = stricmp(child->Value(),"CHILDID");
			#endif

			if(_cmpCHILDID!=0)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}

			TiXmlNode *node= child->FirstChild();
			if(!node)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}
			TiXmlText* childid = node->ToText();
			if(!childid)
			{
				CalError::setLastError(CalError::INVALID_FILE_FORMAT, __FILE__, __LINE__, doc.Value());
				return false;
			}

			int childId = atoi(childid->Value());

			pCoreBone->addChildId(childId);
		}

		// set the core skeleton of the core bone instance
		pCoreBone->setCoreSkeleton(pCoreSkeleton.get());

		// add the core bone to the core skeleton instance
		pCoreSkeleton->addCoreBone(pCoreBone.release());

	}

	doc.Clear();

	pCoreSkeleton->calculateState();

	return pCoreSkeleton;
}


//****************************************************************************//
