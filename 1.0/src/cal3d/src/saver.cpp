//****************************************************************************//
// saver.cpp                                                                  //
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

#include "cal3d/src/saver.h"
#include "cal3d/src/error.h"
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

#include <float.h>

using namespace cal3d;

 /*****************************************************************************/
/** Saves a core animation instance.
  *
  * This function saves a core animation instance to a file.
  *
  * @param strFilename The name of the file to save the core animation instance
  *                    to.
  * @param pCoreAnimation A pointer to the core animation instance that should
  *                       be saved.
	* param pOptions Optional pointer to save options.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreAnimation(const std::string& strFilename, CalCoreAnimation *pCoreAnimation, CalSaverAnimationOptions *pOptions)
{
	int _cmpANIMATION;
  	#if defined(FT_STRICMP)
		_cmpANIMATION = FT_STRICMP(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATION_XMLFILE_MAGIC);
	#else
		_cmpANIMATION = stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::ANIMATION_XMLFILE_MAGIC);
	#endif

  if(strFilename.size()>= 3 && _cmpANIMATION==0)
	 return saveXmlCoreAnimation(strFilename, pCoreAnimation);

  // open the file
  std::ofstream file;
  file.open(strFilename.c_str(), std::ios::out | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_CREATION_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write magic tag
  if(!CalPlatform::writeBytes(file, &Cal::ANIMATION_FILE_MAGIC, sizeof(Cal::ANIMATION_FILE_MAGIC)))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write version info
  if(!CalPlatform::writeInteger(file, Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the duration of the core animation
  if(!CalPlatform::writeFloat(file, pCoreAnimation->getDuration()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get core track list
  std::list<CalCoreTrack *>& listCoreTrack = pCoreAnimation->getListCoreTrack();

  // write the number of tracks
  if(!CalPlatform::writeInteger(file, listCoreTrack.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return 0;
  }

	if (pOptions)
		pOptions->duration = pCoreAnimation->getDuration();

	// write flags
	int flags = 0;
	if (pOptions && pOptions->bCompressKeyframes == true)
		flags |= 1;

  if(!CalPlatform::writeInteger(file, flags))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return 0;
  }

  // write all core bones
  std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = listCoreTrack.begin(); iteratorCoreTrack != listCoreTrack.end(); ++iteratorCoreTrack)
  {
    // save core track
    if(!saveCoreTrack(file, strFilename, *iteratorCoreTrack, pOptions))
    {
      return false;
    }
  }

  // explicitly close the file
  file.close();

  pCoreAnimation->setFilename(strFilename);

  return true;
}

 /*****************************************************************************/
/** Saves a core bone instance.
  *
  * This function saves a core bone instance to a file stream.
  *
  * @param file The file stream to save the core bone instance to.
  * @param strFilename The name of the file stream.
  * @param pCoreBone A pointer to the core bone instance that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreBones(std::ofstream& file, const std::string& strFilename, CalCoreBone *pCoreBone)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the name of the bone
  if(!CalPlatform::writeString(file, pCoreBone->getName()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the translation of the bone
  const CalVector& translation = pCoreBone->getTranslation();
  CalPlatform::writeFloat(file, translation[0]);
  CalPlatform::writeFloat(file, translation[1]);
  CalPlatform::writeFloat(file, translation[2]);

  // write the rotation of the bone
  const CalQuaternion& rotation = pCoreBone->getRotation();
  CalPlatform::writeFloat(file, rotation[0]);
  CalPlatform::writeFloat(file, rotation[1]);
  CalPlatform::writeFloat(file, rotation[2]);
  CalPlatform::writeFloat(file, rotation[3]);

  // write the translation of the bone
  const CalVector& translationBoneSpace = pCoreBone->getTranslationBoneSpace();
  CalPlatform::writeFloat(file, translationBoneSpace[0]);
  CalPlatform::writeFloat(file, translationBoneSpace[1]);
  CalPlatform::writeFloat(file, translationBoneSpace[2]);

  // write the rotation of the bone
  const CalQuaternion& rotationBoneSpace = pCoreBone->getRotationBoneSpace();
  CalPlatform::writeFloat(file, rotationBoneSpace[0]);
  CalPlatform::writeFloat(file, rotationBoneSpace[1]);
  CalPlatform::writeFloat(file, rotationBoneSpace[2]);
  CalPlatform::writeFloat(file, rotationBoneSpace[3]);

  // write the parent bone id
  if(!CalPlatform::writeInteger(file, pCoreBone->getParentId()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get children list
  std::list<int>& listChildId = pCoreBone->getListChildId();

  // write the number of children
  if(!CalPlatform::writeInteger(file, listChildId.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all children ids
  std::list<int>::iterator iteratorChildId;
  for(iteratorChildId = listChildId.begin(); iteratorChildId != listChildId.end(); ++iteratorChildId)
  {
    // write the child id
    if(!CalPlatform::writeInteger(file, *iteratorChildId))
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core keyframe instance.
  *
  * This function saves a core keyframe instance to a file stream.
  *
  * @param file The file stream to save the core keyframe instance to.
  * @param strFilename The name of the file stream.
  * @param pCoreKeyframe A pointer to the core keyframe instance that should be
  *                      saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreKeyframe(std::ofstream& file, const std::string& strFilename, CalCoreKeyframe *pCoreKeyframe)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the time of the keyframe
  CalPlatform::writeFloat(file, pCoreKeyframe->getTime());

  // write the translation of the keyframe
  const CalVector& translation = pCoreKeyframe->getTranslation();
  CalPlatform::writeFloat(file, translation[0]);
  CalPlatform::writeFloat(file, translation[1]);
  CalPlatform::writeFloat(file, translation[2]);

  // write the rotation of the keyframe
  const CalQuaternion& rotation = pCoreKeyframe->getRotation();
  CalPlatform::writeFloat(file, rotation[0]);
  CalPlatform::writeFloat(file, rotation[1]);
  CalPlatform::writeFloat(file, rotation[2]);
  CalPlatform::writeFloat(file, rotation[3]);

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  return true;
}

bool CalSaver::saveCompressedCoreKeyframe(std::ofstream& file, const std::string& strFilename, CalCoreKeyframe* pCoreKeyframe, CalSaverAnimationOptions *pOptions)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the time of the keyframe
	int time = int(pCoreKeyframe->getTime() / pOptions->duration * 65535.0f);
	if(time > 65535)
		time = 65535;
  CalPlatform::writeShort(file, time);

  // write the translation of the keyframe
  const CalVector &translation = pCoreKeyframe->getTranslation();

	const CalVector &minp = pOptions->keyframe_min;
	const CalVector &scale = pOptions->keyframe_scale;

	int px = int((translation.x - minp.x) * scale.x);
	int py = int((translation.y - minp.y) * scale.y);
	int pz = int((translation.z - minp.z) * scale.z);

	if(px > 2047)
		px = 2047;
	if(py > 2047)
		py = 2047;
	if(pz > 1023)
		pz = 1023;

	int towrite = px + (py << 11) + (pz << 22);
  CalPlatform::writeInteger(file, towrite);

  // write the compressed rotation of the keyframe
  CalQuaternion rotation = pCoreKeyframe->getRotation();
	short s0, s1, s2;
	rotation.compress(s0, s1, s2);

	CalPlatform::writeShort(file, s0);
	CalPlatform::writeShort(file, s1);
	CalPlatform::writeShort(file, s2);

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core material instance.
  *
  * This function saves a core material instance to a file.
  *
  * @param strFilename The name of the file to save the core material instance
  *                    to.
  * @param pCoreMaterial A pointer to the core material instance that should
  *                      be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreMaterial(const std::string& strFilename, CalCoreMaterial *pCoreMaterial)
{
	int _cmpMATERIAL;
  	#if defined(FT_STRICMP)
		_cmpMATERIAL = FT_STRICMP(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MATERIAL_XMLFILE_MAGIC);
	#else
		_cmpMATERIAL = stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MATERIAL_XMLFILE_MAGIC);
	#endif

  if(strFilename.size()>= 3 && _cmpMATERIAL==0)
    return saveXmlCoreMaterial(strFilename, pCoreMaterial);

  // open the file
  std::ofstream file;
  file.open(strFilename.c_str(), std::ios::out | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_CREATION_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write magic tag
  if(!CalPlatform::writeBytes(file, &Cal::MATERIAL_FILE_MAGIC, sizeof(Cal::MATERIAL_FILE_MAGIC)))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write version info
  if(!CalPlatform::writeInteger(file, Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the ambient color
  CalCoreMaterial::Color ambientColor;
  ambientColor = pCoreMaterial->getAmbientColor();
  CalPlatform::writeBytes(file, &ambientColor, sizeof(ambientColor));

  // write the diffuse color
  CalCoreMaterial::Color diffusetColor;
  diffusetColor = pCoreMaterial->getDiffuseColor();
  CalPlatform::writeBytes(file, &diffusetColor, sizeof(diffusetColor));

  // write the specular color
  CalCoreMaterial::Color specularColor;
  specularColor = pCoreMaterial->getSpecularColor();
  CalPlatform::writeBytes(file, &specularColor, sizeof(specularColor));

  // write the shininess factor
  CalPlatform::writeFloat(file, pCoreMaterial->getShininess());

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get the map vector
  std::vector<CalCoreMaterial::Map>& vectorMap = pCoreMaterial->getVectorMap();

  // write the number of maps
  if(!CalPlatform::writeInteger(file, vectorMap.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all maps
  int mapId;
  for(mapId = 0; mapId < (int)vectorMap.size(); ++mapId)
  {
    CalCoreMaterial::Map& map = vectorMap[mapId];

    // write the filename of the map
    if(!CalPlatform::writeString(file, map.strFilename))
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }
  }

  // explicitly close the file
  file.close();

  pCoreMaterial->setFilename(strFilename);

  return true;
}

 /*****************************************************************************/
/** Saves a core mesh instance.
  *
  * This function saves a core mesh instance to a file.
  *
  * @param strFilename The name of the file to save the core mesh instance to.
  * @param pCoreMesh A pointer to the core mesh instance that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreMesh(const std::string& strFilename, CalCoreMesh *pCoreMesh)
{
	int _cmpMESH;
  	#if defined(FT_STRICMP)
		_cmpMESH = FT_STRICMP(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MESH_XMLFILE_MAGIC);
	#else
		_cmpMESH = stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::MESH_XMLFILE_MAGIC);
	#endif

  if(strFilename.size()>= 3 && _cmpMESH==0)
    return saveXmlCoreMesh(strFilename, pCoreMesh);

  // open the file
  std::ofstream file;
  file.open(strFilename.c_str(), std::ios::out | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_CREATION_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write magic tag
  if(!CalPlatform::writeBytes(file, &Cal::MESH_FILE_MAGIC, sizeof(Cal::MESH_FILE_MAGIC)))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write version info
  if(!CalPlatform::writeInteger(file, Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get the submesh vector
  std::vector<CalCoreSubmesh *>& vectorCoreSubmesh = pCoreMesh->getVectorCoreSubmesh();

  // write the number of submeshes
  if(!CalPlatform::writeInteger(file, vectorCoreSubmesh.size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all core submeshes
  int submeshId;
  for(submeshId = 0; submeshId < (int)vectorCoreSubmesh.size(); ++submeshId)
  {
    // write the core submesh
    if(!saveCoreSubmesh(file, strFilename, vectorCoreSubmesh[submeshId]))
    {
      return false;
    }
  }

  // explicitly close the file
  file.close();

  pCoreMesh->setFilename(strFilename);

  return true;
}

 /*****************************************************************************/
/** Saves a core skeleton instance.
  *
  * This function saves a core skeleton instance to a file.
  *
  * @param strFilename The name of the file to save the core skeleton instance
  *                    to.
  * @param pCoreSkeleton A pointer to the core skeleton instance that should be
  *                      saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreSkeleton(const std::string& strFilename, CalCoreSkeleton *pCoreSkeleton)
{
	int _cmpSKELETON;
  	#if defined(FT_STRICMP)
		_cmpSKELETON = FT_STRICMP(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::SKELETON_XMLFILE_MAGIC);
	#else
		_cmpSKELETON = stricmp(strFilename.substr(strFilename.size()-3,3).c_str(),Cal::SKELETON_XMLFILE_MAGIC);
	#endif

  if(strFilename.size()>= 3 && _cmpSKELETON==0)
    return saveXmlCoreSkeleton(strFilename, pCoreSkeleton);

  // open the file
  std::ofstream file;
  file.open(strFilename.c_str(), std::ios::out | std::ios::binary);
  if(!file)
  {
    CalError::setLastError(CalError::FILE_CREATION_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write magic tag
  if(!CalPlatform::writeBytes(file, &Cal::SKELETON_FILE_MAGIC, sizeof(Cal::SKELETON_FILE_MAGIC)))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write version info
  if(!CalPlatform::writeInteger(file, Cal::CURRENT_FILE_VERSION))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the number of bones
  if(!CalPlatform::writeInteger(file, pCoreSkeleton->getVectorCoreBone().size()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all core bones
  int boneId;
  for(boneId = 0; boneId < (int)pCoreSkeleton->getVectorCoreBone().size(); ++boneId)
  {
    // write the core bone
    if(!saveCoreBones(file, strFilename, pCoreSkeleton->getCoreBone(boneId)))
    {
      return false;
    }
  }

  // explicitly close the file
  file.close();

  return true;
}

 /*****************************************************************************/
/** Saves a core submesh instance.
  *
  * This function saves a core submesh instance to a file stream.
  *
  * @param file The file stream to save the core submesh instance to.
  * @param strFilename The name of the file stream.
  * @param pCoreSubmesh A pointer to the core submesh instance that should be
  *                     saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreSubmesh(std::ofstream& file, const std::string& strFilename, CalCoreSubmesh *pCoreSubmesh)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the core material thread id
  if(!CalPlatform::writeInteger(file, pCoreSubmesh->getCoreMaterialThreadId()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // get the vertex, face, physical property and spring vector
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pCoreSubmesh->getVectorVertex();
  std::vector<CalCoreSubmesh::Face>& vectorFace = pCoreSubmesh->getVectorFace();
  std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pCoreSubmesh->getVectorPhysicalProperty();
  std::vector<CalCoreSubmesh::Spring>& vectorSpring = pCoreSubmesh->getVectorSpring();

  // write the number of vertices, faces, level-of-details and springs
  CalPlatform::writeInteger(file, vectorVertex.size());
  CalPlatform::writeInteger(file, vectorFace.size());
  CalPlatform::writeInteger(file, pCoreSubmesh->getLodCount());
  CalPlatform::writeInteger(file, pCoreSubmesh->getSpringCount());

  // get the texture coordinate vector vector
  std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& vectorvectorTextureCoordinate = pCoreSubmesh->getVectorVectorTextureCoordinate();

  // write the number of texture coordinates per vertex
  CalPlatform::writeInteger(file, vectorvectorTextureCoordinate.size());

  // check if an error happend
  if(!file)
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write all vertices
  int vertexId;
  for(vertexId = 0; vertexId < (int)vectorVertex.size(); ++vertexId)
  {
    CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];

    // write the vertex data
    CalPlatform::writeFloat(file, vertex.position.x);
    CalPlatform::writeFloat(file, vertex.position.y);
    CalPlatform::writeFloat(file, vertex.position.z);
    CalPlatform::writeFloat(file, vertex.normal.x);
    CalPlatform::writeFloat(file, vertex.normal.y);
    CalPlatform::writeFloat(file, vertex.normal.z);
    CalPlatform::writeInteger(file, vertex.collapseId);
    CalPlatform::writeInteger(file, vertex.faceCollapseCount);

    // write all texture coordinates of this vertex
    int textureCoordinateId;
    for(textureCoordinateId = 0; textureCoordinateId < (int)vectorvectorTextureCoordinate.size(); ++textureCoordinateId)
    {
      CalCoreSubmesh::TextureCoordinate& textureCoordinate = vectorvectorTextureCoordinate[textureCoordinateId][vertexId];

      // write the influence data
      CalPlatform::writeFloat(file, textureCoordinate.u);
      CalPlatform::writeFloat(file, textureCoordinate.v);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
        return false;
      }
    }

    // write the number of influences
    if(!CalPlatform::writeInteger(file, vertex.vectorInfluence.size()))
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }

     // write all influences of this vertex
    int influenceId;
    for(influenceId = 0; influenceId < (int)vertex.vectorInfluence.size(); ++influenceId)
    {
      CalCoreSubmesh::Influence& influence = vertex.vectorInfluence[influenceId];

      // write the influence data
      CalPlatform::writeInteger(file, influence.boneId);
      CalPlatform::writeFloat(file, influence.weight);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
        return false;
      }
    }

    // save the physical property of the vertex if there are springs in the core submesh
    if(pCoreSubmesh->getSpringCount() > 0)
    {
      // write the physical property of this vertex if there are springs in the core submesh
      CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

      // write the physical property data
      CalPlatform::writeFloat(file, physicalProperty.weight);

      // check if an error happend
      if(!file)
      {
        CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
        return false;
      }
    }
  }

  // write all springs
  int springId;
  for(springId = 0; springId < (int)pCoreSubmesh->getSpringCount(); ++springId)
  {
    CalCoreSubmesh::Spring& spring = vectorSpring[springId];

    // write the spring data
    CalPlatform::writeInteger(file, spring.vertexId[0]);
    CalPlatform::writeInteger(file, spring.vertexId[1]);
    CalPlatform::writeFloat(file, spring.springCoefficient);
    CalPlatform::writeFloat(file, spring.idleLength);

    // check if an error happend
    if(!file)
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }
  }

  // write all faces
  int faceId;
  for(faceId = 0; faceId < (int)vectorFace.size(); ++faceId)
  {
    CalCoreSubmesh::Face& face = vectorFace[faceId];

    // write the face data
    CalPlatform::writeInteger(file, face.vertexId[0]);
    CalPlatform::writeInteger(file, face.vertexId[1]);
    CalPlatform::writeInteger(file, face.vertexId[2]);

    // check if an error happend
    if(!file)
    {
      CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
      return false;
    }
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core track instance.
  *
  * This function saves a core track instance to a file stream.
  *
  * @param file The file stream to save the core track instance to.
  * @param strFilename The name of the file stream.
  * @param pCoreTrack A pointer to the core track instance that should be saved.
	* @parAM pOptions Optional pointer to save options.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveCoreTrack(std::ofstream& file, const std::string& strFilename, CalCoreTrack *pCoreTrack, CalSaverAnimationOptions *pOptions)
{
  if(!file)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the bone id
  if(!CalPlatform::writeInteger(file, pCoreTrack->getCoreBoneId()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  // write the number of keyframes
  if(!CalPlatform::writeInteger(file, pCoreTrack->getCoreKeyframeCount()))
  {
    CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

	if(pOptions && pOptions->bCompressKeyframes == true) {

		CalVector minp(FLT_MAX, FLT_MAX, FLT_MAX);
		CalVector maxp(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		int nbKeys = pCoreTrack->getCoreKeyframeCount();
		for(int i = 0; i < nbKeys; i++) {
			CalCoreKeyframe *kf = pCoreTrack->getCoreKeyframe(i);
			const CalVector &pos = kf->getTranslation();

			if(pos.x < minp.x)
				minp.x = pos.x;
			if(pos.x > maxp.x)
				maxp.x = pos.x;

			if(pos.y < minp.y)
				minp.y = pos.y;
			if(pos.y > maxp.y)
				maxp.y = pos.y;

			if(pos.z < minp.z)
				minp.z = pos.z;
			if(pos.z > maxp.z)
				maxp.z = pos.z;
		}

		float dx = maxp.x - minp.x;
		float dy = maxp.y - minp.y;
		float dz = maxp.z - minp.z;

		float factorx = 0, factory = 0, factorz = 0;
		if(dx != 0)
			factorx = 1.0f / dx * 2047.0f;

		if(dy != 0)
			factory = 1.0f / dy * 2047.0f;

		if(dz != 0)
			factorz = 1.0f / dz * 1023.0f;

		pOptions->keyframe_min = minp;
		pOptions->keyframe_scale = CalVector(factorx, factory, factorz);

		CalPlatform::writeFloat(file, minp.x);
		CalPlatform::writeFloat(file, minp.y);
		CalPlatform::writeFloat(file, minp.z);
		CalPlatform::writeFloat(file, 1.0f / 2047 * dx);
		CalPlatform::writeFloat(file, 1.0f / 2047 * dy);
		CalPlatform::writeFloat(file, 1.0f / 1023 * dz);
	}

  // save all core keyframes
  for(int i = 0; i < pCoreTrack->getCoreKeyframeCount(); ++i)
  {
    // save the core keyframe
		bool res;
		if(pOptions && pOptions->bCompressKeyframes)
		{
			res = saveCompressedCoreKeyframe(file, strFilename, pCoreTrack->getCoreKeyframe(i), pOptions);
		}
		else
		{
			res = saveCoreKeyframe(file, strFilename, pCoreTrack->getCoreKeyframe(i));
		}

		if(!res) {
      return false;
    }
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core skeleton instance to a XML file
  *
  * This function saves a core skeleton instance to a XML file.
  *
  * @param strFilename The name of the file to save the core skeleton instance
  *                    to.
  * @param pCoreSkeleton A pointer to the core skeleton instance that should be
  *                      saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveXmlCoreSkeleton(const std::string& strFilename, CalCoreSkeleton *pCoreSkeleton)
{
  std::stringstream str;

  TiXmlDocument doc(strFilename.c_str());


  TiXmlElement skeleton("SKELETON");
  //skeleton.SetAttribute("MAGIC",Cal::SKELETON_XMLFILE_MAGIC);
  skeleton.SetAttribute("VERSION",Cal::LIBRARY_VERSION);

  skeleton.SetAttribute("NUMBONES",pCoreSkeleton->getVectorCoreBone().size());



  int boneId;
  for(boneId = 0; boneId < (int)pCoreSkeleton->getVectorCoreBone().size(); ++boneId)
  {
	  CalCoreBone* pCoreBone=pCoreSkeleton->getCoreBone(boneId);

	  TiXmlElement bone("BONE");
	  bone.SetAttribute("ID",boneId);
	  bone.SetAttribute("NAME",pCoreBone->getName().c_str());
	  bone.SetAttribute("NUMCHILDS",pCoreBone->getListChildId().size());

	  TiXmlElement translation("TRANSLATION");
	  const CalVector& translationVector = pCoreBone->getTranslation();


      str.str("");
      str << translationVector.x << " "
    	  << translationVector.y << " "
	      << translationVector.z;

      TiXmlText translationdata(str.str().c_str());

      translation.InsertEndChild(translationdata);
      bone.InsertEndChild(translation);


	  TiXmlElement rotation("ROTATION");
	  const CalQuaternion& rotationQuad = pCoreBone->getRotation();


      str.str("");
      str << rotationQuad.x << " "
    	  << rotationQuad.y << " "
		  << rotationQuad.z << " "
	      << rotationQuad.w;

      TiXmlText rotationdata(str.str().c_str());
      rotation.InsertEndChild(rotationdata);
      bone.InsertEndChild(rotation);


	  TiXmlElement localtranslation("LOCALTRANSLATION");
	  const CalVector& localtranslationVector = pCoreBone->getTranslationBoneSpace();

      str.str("");
      str << localtranslationVector.x << " "
    	  << localtranslationVector.y << " "
	      << localtranslationVector.z;

      TiXmlText localtranslationdata(str.str().c_str());

      localtranslation.InsertEndChild(localtranslationdata);
      bone.InsertEndChild(localtranslation);


	  TiXmlElement localrotation("LOCALROTATION");
	  const CalQuaternion& localrotationQuad = pCoreBone->getRotationBoneSpace();

      str.str("");
      str << localrotationQuad.x << " "
    	  << localrotationQuad.y << " "
		  << localrotationQuad.z << " "
	      << localrotationQuad.w;

      TiXmlText localrotationdata(str.str().c_str());
      localrotation.InsertEndChild(localrotationdata);
      bone.InsertEndChild(localrotation);


	  TiXmlElement parent("PARENTID");
	  str.str("");
      str << pCoreBone->getParentId();
	  TiXmlText parentid(str.str().c_str());
	  parent.InsertEndChild(parentid);
      bone.InsertEndChild(parent);


	  // get children list
      std::list<int>& listChildId = pCoreBone->getListChildId();


	  // write all children ids
      std::list<int>::iterator iteratorChildId;
      for(iteratorChildId = listChildId.begin(); iteratorChildId != listChildId.end(); ++iteratorChildId)
	  {
		  TiXmlElement child("CHILDID");
		  str.str("");
		  //int id=*iteratorChildId;
          str << *iteratorChildId;
	      TiXmlText childid(str.str().c_str());
		  child.InsertEndChild(childid);
          bone.InsertEndChild(child);
	  }
	  skeleton.InsertEndChild(bone);

  }
  doc.InsertEndChild(skeleton);

  if(!doc.SaveFile())
  {
	CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }
  return true;
}

 /*****************************************************************************/
/** Saves a core animation instance in a XML file.
  *
  * This function saves a core animation instance to a XML file.
  *
  * @param strFilename The name of the file to save the core animation instance
  *                    to.
  * @param pCoreAnimation A pointer to the core animation instance that should
  *                       be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveXmlCoreAnimation(const std::string& strFilename, CalCoreAnimation *pCoreAnimation)
{
	std::stringstream str;

	TiXmlDocument doc(strFilename.c_str());

	TiXmlElement animation("ANIMATION");
	//animation.SetAttribute("MAGIC",Cal::ANIMATION_XMLFILE_MAGIC);
	animation.SetAttribute("VERSION",Cal::LIBRARY_VERSION);

	str.str("");
	str << pCoreAnimation->getDuration();
	animation.SetAttribute("DURATION",str.str().c_str());
	animation.SetAttribute("NUMTRACKS", pCoreAnimation->getTrackCount());

	// get core track list
	std::list<CalCoreTrack *>& listCoreTrack = pCoreAnimation->getListCoreTrack();

	// write all core bones
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
	for(iteratorCoreTrack = listCoreTrack.begin(); iteratorCoreTrack != listCoreTrack.end(); ++iteratorCoreTrack)
	{
		CalCoreTrack *pCoreTrack=*iteratorCoreTrack;

		TiXmlElement track("TRACK");
		track.SetAttribute("BONEID",pCoreTrack->getCoreBoneId());

		track.SetAttribute("NUMKEYFRAMES",pCoreTrack->getCoreKeyframeCount());

		// save all core keyframes
		for (int i = 0; i < pCoreTrack->getCoreKeyframeCount(); ++i)
		{
			CalCoreKeyframe *pCoreKeyframe=pCoreTrack->getCoreKeyframe(i);

			TiXmlElement keyframe("KEYFRAME");

			str.str("");
			str << pCoreKeyframe->getTime();
			keyframe.SetAttribute("TIME",str.str().c_str());

			TiXmlElement translation("TRANSLATION");
			const CalVector& translationVector = pCoreKeyframe->getTranslation();

			str.str("");
			str << translationVector.x << " "
				<< translationVector.y << " "
				<< translationVector.z;

			TiXmlText translationdata(str.str().c_str());

			translation.InsertEndChild(translationdata);
			keyframe.InsertEndChild(translation);

			TiXmlElement rotation("ROTATION");
			const CalQuaternion& rotationQuad = pCoreKeyframe->getRotation();

			str.str("");
			str << rotationQuad.x << " "
				<< rotationQuad.y << " "
				<< rotationQuad.z << " "
				<< rotationQuad.w;

			TiXmlText rotationdata(str.str().c_str());
			rotation.InsertEndChild(rotationdata);
			keyframe.InsertEndChild(rotation);

			track.InsertEndChild(keyframe);
		}

		animation.InsertEndChild(track);
	}

	doc.InsertEndChild(animation);

	if(!doc.SaveFile())
	{
		CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
		return false;
	}

	return true;
}


 /*****************************************************************************/
/** Saves a core mesh instance in a XML file.
  *
  * This function saves a core mesh instance to a XML file.
  *
  * @param strFilename The name of the file to save the core mesh instance to.
  * @param pCoreMesh A pointer to the core mesh instance that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveXmlCoreMesh(const std::string& strFilename, CalCoreMesh *pCoreMesh)
{

	std::stringstream str;

    TiXmlDocument doc(strFilename.c_str());


	TiXmlElement mesh("MESH");
	//mesh.SetAttribute("MAGIC",Cal::MESH_XMLFILE_MAGIC);
	mesh.SetAttribute("VERSION",Cal::LIBRARY_VERSION);
	mesh.SetAttribute("NUMSUBMESH",pCoreMesh->getCoreSubmeshCount());

	// get the submesh vector
	std::vector<CalCoreSubmesh *>& vectorCoreSubmesh = pCoreMesh->getVectorCoreSubmesh();

	// write all core submeshes
	int submeshId;
	for(submeshId = 0; submeshId < (int)vectorCoreSubmesh.size(); ++submeshId)
	{
		CalCoreSubmesh *pCoreSubmesh=vectorCoreSubmesh[submeshId];

		TiXmlElement submesh("SUBMESH");

		submesh.SetAttribute("NUMVERTICES",pCoreSubmesh->getVertexCount());
		submesh.SetAttribute("NUMFACES",pCoreSubmesh->getFaceCount());
		submesh.SetAttribute("MATERIAL",pCoreSubmesh->getCoreMaterialThreadId());
		submesh.SetAttribute("NUMLODSTEPS",pCoreSubmesh->getLodCount());
		submesh.SetAttribute("NUMSPRINGS",pCoreSubmesh->getSpringCount());

		submesh.SetAttribute("NUMTEXCOORDS",pCoreSubmesh->getVectorVectorTextureCoordinate().size());


		// get the vertex, face, physical property and spring vector
		std::vector<CalCoreSubmesh::Vertex>& vectorVertex = pCoreSubmesh->getVectorVertex();
		std::vector<CalCoreSubmesh::Face>& vectorFace = pCoreSubmesh->getVectorFace();
		std::vector<CalCoreSubmesh::PhysicalProperty>& vectorPhysicalProperty = pCoreSubmesh->getVectorPhysicalProperty();
		std::vector<CalCoreSubmesh::Spring>& vectorSpring = pCoreSubmesh->getVectorSpring();

		// get the texture coordinate vector vector
        std::vector<std::vector<CalCoreSubmesh::TextureCoordinate> >& vectorvectorTextureCoordinate = pCoreSubmesh->getVectorVectorTextureCoordinate();

		// write all vertices
		int vertexId;
		for(vertexId = 0; vertexId < (int)vectorVertex.size(); ++vertexId)
		{
			CalCoreSubmesh::Vertex& Vertex = vectorVertex[vertexId];

			TiXmlElement vertex("VERTEX");
			vertex.SetAttribute("ID",vertexId);
			vertex.SetAttribute("NUMINFLUENCES",Vertex.vectorInfluence.size());

			// write the vertex data

			TiXmlElement position("POS");

			str.str("");
			str << Vertex.position.x << " "
				<< Vertex.position.y << " "
				<< Vertex.position.z;

			TiXmlText positiondata(str.str().c_str());

			position.InsertEndChild(positiondata);
			vertex.InsertEndChild(position);

			TiXmlElement normal("NORM");

			str.str("");
			str << Vertex.normal.x << " "
				<< Vertex.normal.y << " "
				<< Vertex.normal.z;

			TiXmlText normaldata(str.str().c_str());

			normal.InsertEndChild(normaldata);
			vertex.InsertEndChild(normal);

			if(Vertex.collapseId!=-1)
			{
				TiXmlElement collapse("COLLAPSEID");
				str.str("");
				str << Vertex.collapseId;
				TiXmlText collapseid(str.str().c_str());
				collapse.InsertEndChild(collapseid);
				vertex.InsertEndChild(collapse);

				TiXmlElement collapsecount("COLLAPSECOUNT");
				str.str("");
				str << Vertex.faceCollapseCount;
				TiXmlText collapsecountdata(str.str().c_str());
				collapsecount.InsertEndChild(collapsecountdata);
				vertex.InsertEndChild(collapsecount);
			}

			// write all texture coordinates of this vertex
			int textureCoordinateId;
			for(textureCoordinateId = 0; textureCoordinateId < (int)vectorvectorTextureCoordinate.size(); ++textureCoordinateId)
			{
				CalCoreSubmesh::TextureCoordinate& textureCoordinate = vectorvectorTextureCoordinate[textureCoordinateId][vertexId];

				TiXmlElement tex("TEXCOORD");

				str.str("");
				str << textureCoordinate.u << " "
					<< textureCoordinate.v;

				TiXmlText texdata(str.str().c_str());

				tex.InsertEndChild(texdata);

				vertex.InsertEndChild(tex);
			}

			// write all influences of this vertex
			int influenceId;
			for(influenceId = 0; influenceId < (int)Vertex.vectorInfluence.size(); ++influenceId)
			{
				CalCoreSubmesh::Influence& Influence = Vertex.vectorInfluence[influenceId];

				TiXmlElement influence("INFLUENCE");

				influence.SetAttribute("ID",Influence.boneId);

				str.str("");
				str << Influence.weight;

				TiXmlText influencedata(str.str().c_str());

				influence.InsertEndChild(influencedata);
				vertex.InsertEndChild(influence);
			}

			// save the physical property of the vertex if there are springs in the core submesh
			if(pCoreSubmesh->getSpringCount() > 0)
			{
				// write the physical property of this vertex if there are springs in the core submesh
				CalCoreSubmesh::PhysicalProperty& physicalProperty = vectorPhysicalProperty[vertexId];

				TiXmlElement physique("PHYSIQUE");

				str.str("");
				str << physicalProperty.weight;
				TiXmlText physiquedata(str.str().c_str());
				physique.InsertEndChild(physiquedata);
				vertex.InsertEndChild(physique);
			}

			submesh.InsertEndChild(vertex);
		}

		// write all springs
        int springId;
        for(springId = 0; springId < (int)pCoreSubmesh->getSpringCount(); ++springId)
		{
            CalCoreSubmesh::Spring& Spring = vectorSpring[springId];

			TiXmlElement spring("SPRING");

			str.str("");
			str << Spring.vertexId[0]<< " "
				<< Spring.vertexId[1];
			spring.SetAttribute("VERTEXID",str.str().c_str());


			str.str("");
			str << Spring.springCoefficient;
			spring.SetAttribute("COEF",str.str().c_str());

			str.str("");
			str << Spring.idleLength;
			spring.SetAttribute("LENGTH",str.str().c_str());

			submesh.InsertEndChild(spring);
		}

		// write all faces
		int faceId;
		for(faceId = 0; faceId < (int)vectorFace.size(); ++faceId)
		{
			CalCoreSubmesh::Face& Face = vectorFace[faceId];

			TiXmlElement face("FACE");

			str.str("");
			str << Face.vertexId[0]<< " "
				<< Face.vertexId[1]<< " "
				<< Face.vertexId[2];
			face.SetAttribute("VERTEXID",str.str().c_str());
			submesh.InsertEndChild(face);
		}

		mesh.InsertEndChild(submesh);

     }
	 doc.InsertEndChild(mesh);


	 if(!doc.SaveFile())
	 {
		 CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
         return false;
	 }

  return true;
}



 /*****************************************************************************/
/** Saves a core material instance to a XML file.
  *
  * This function saves a core material instance to a XML file.
  *
  * @param strFilename The name of the file to save the core material instance
  *                    to.
  * @param pCoreMaterial A pointer to the core material instance that should
  *                      be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSaver::saveXmlCoreMaterial(const std::string& strFilename, CalCoreMaterial *pCoreMaterial)
{
  std::stringstream str;

  TiXmlDocument doc(strFilename.c_str());

  TiXmlElement material("MATERIAL");
  //material.SetAttribute("MAGIC",Cal::MATERIAL_XMLFILE_MAGIC);
  material.SetAttribute("VERSION",Cal::LIBRARY_VERSION);

  material.SetAttribute("NUMMAPS",pCoreMaterial->getVectorMap().size());

  TiXmlElement ambient("AMBIENT");

  CalCoreMaterial::Color ambientColor;
  ambientColor = pCoreMaterial->getAmbientColor();


  str.str("");
  str << (int)ambientColor.red << " "
	  << (int)ambientColor.green << " "
	  << (int)ambientColor.blue << " "
	  << (int)ambientColor.alpha;

  TiXmlText ambientdata(str.str().c_str());


  ambient.InsertEndChild(ambientdata);
  material.InsertEndChild(ambient);

  TiXmlElement diffuse("DIFFUSE");

  CalCoreMaterial::Color diffuseColor;
  diffuseColor = pCoreMaterial->getDiffuseColor();

  str.str("");
  str << (int)diffuseColor.red << " "
	  << (int)diffuseColor.green << " "
	  << (int)diffuseColor.blue << " "
	  << (int)diffuseColor.alpha;

  TiXmlText diffusedata(str.str().c_str());


  diffuse.InsertEndChild(diffusedata);
  material.InsertEndChild(diffuse);

  TiXmlElement specular("SPECULAR");

  CalCoreMaterial::Color specularColor;
  specularColor = pCoreMaterial->getSpecularColor();

  str.str("");
  str << (int)specularColor.red << " "
	  << (int)specularColor.green << " "
	  << (int)specularColor.blue << " "
	  << (int)specularColor.alpha;

  TiXmlText speculardata(str.str().c_str());

  specular.InsertEndChild(speculardata);
  material.InsertEndChild(specular);

  TiXmlElement shininess("SHININESS");

  str.str("");
  str << pCoreMaterial->getShininess();

  TiXmlText shininessdata(str.str().c_str());

  shininess.InsertEndChild(shininessdata);
  material.InsertEndChild(shininess);

  std::vector<CalCoreMaterial::Map>& vectorMap = pCoreMaterial->getVectorMap();

  int mapId;
  for(mapId = 0; mapId < (int)vectorMap.size(); ++mapId)
  {
	  TiXmlElement map("MAP");
	  TiXmlText mapdata(vectorMap[mapId].strFilename.c_str());
	  map.InsertEndChild(mapdata);
      material.InsertEndChild(map);
  }

  doc.InsertEndChild(material);

  if(!doc.SaveFile())
  {
	CalError::setLastError(CalError::FILE_WRITING_FAILED, __FILE__, __LINE__, strFilename);
    return false;
  }

  return true;

}
//****************************************************************************//
