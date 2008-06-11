//****************************************************************************//
// coremodel.h                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_COREMODEL_H
#define CAL_COREMODEL_H


#include "cal3d/src/coreanimation.h"
#include "cal3d/src/corematerial.h"
#include "cal3d/src/coremesh.h"
#include "cal3d/src/coremorphanimation.h"
#include "cal3d/src/coreskeleton.h"
#include "cal3d/src/global.h"




class CAL3D_API CalCoreModel
{
public:
  CalCoreModel(const std::string& name);
  CalCoreModel(const CalCoreModel& inOther);
  ~CalCoreModel();

  Cal::UserData getUserData();
  void setUserData(Cal::UserData userData);
  const std::string& getName() const;
  void setName( const char* inName );

  void scale(float factor);

  // animations
  int addCoreAnimation(CalCoreAnimation *pCoreAnimation);
  CalCoreAnimation *getCoreAnimation(int coreAnimationId);
  int getCoreAnimationCount() const;
  int loadCoreAnimation(const std::string& strFilename);
  int loadCoreAnimation(const std::string& strFilename, const std::string& strAnimationName);
  int loadCoreAnimation(void* buffer);
  int unloadCoreAnimation(const std::string& name);
  int unloadCoreAnimation(int coreAnimationId);
  bool saveCoreAnimation(const std::string& strFilename, int coreAnimationId);
  bool addAnimationName(const std::string& strAnimationName, int coreAnimationId);
  int getCoreAnimationId(const std::string& strAnimationName);

  // morph animations
  int addCoreMorphAnimation(CalCoreMorphAnimation *pCoreMorphAnimation);
  CalCoreMorphAnimation *getCoreMorphAnimation(int coreMorphAnimationId);
  int getCoreMorphAnimationCount() const;

  // materials
  int addCoreMaterial(CalCoreMaterial *pCoreMaterial);
  void cloneCoreMaterials();
  bool createCoreMaterialThread(int coreMaterialThreadId);
  CalCoreMaterial *getCoreMaterial(int coreMaterialId);
  int getCoreMaterialCount() const;
  int getCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId);
  int loadCoreMaterial(const std::string& strFilename);
  int loadCoreMaterial(const std::string& strFilename, const std::string& strMaterialName);
  int loadCoreMaterial(void* buffer);
  int unloadCoreMaterial(const std::string& name);
  int unloadCoreMaterial(int coreMaterialId);
  bool saveCoreMaterial(const std::string& strFilename, int coreMaterialId);
  bool setCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId);
  bool addMaterialName(const std::string& strMaterialName, int coreMaterialId);
  int getCoreMaterialId(const std::string& strMaterialName);

  // meshes
  int addCoreMesh(CalCoreMesh *pCoreMesh);
  void replaceCoreMesh( int coreMeshId, CalCoreMesh *pCoreMesh );
  CalCoreMesh *getCoreMesh(int coreMeshId);
  int getCoreMeshCount() const;
  int loadCoreMesh(const std::string& strFilename);
  int loadCoreMesh(const std::string& strFilename, const std::string& strMeshName);
  int loadCoreMesh(void* buffer);
  int unloadCoreMesh(const std::string& name);
  int unloadCoreMesh(int coreMeshId);
  bool saveCoreMesh(const std::string& strFilename, int coreMeshId);
  bool addMeshName(const std::string& strMeshName, int coreMeshId);
  int getCoreMeshId(const std::string& strMeshName);

  // skeleton
  CalCoreSkeleton *getCoreSkeleton();
  bool loadCoreSkeleton(const std::string& strFilename);
  bool loadCoreSkeleton(void* buffer);
  bool saveCoreSkeleton(const std::string& strFilename);
  void setCoreSkeleton(CalCoreSkeleton *pCoreSkeleton);
  void addBoneName(const std::string& strBoneName, int boneId);
  int getBoneId(const std::string& strBoneName) const;

// member variables
private:
  std::string m_strName;
  CalCoreSkeletonPtr m_pCoreSkeleton;
  std::vector<CalCoreAnimationPtr> m_vectorCoreAnimation;
  std::vector<CalCoreMorphAnimationPtr> m_vectorCoreMorphAnimation;
  std::vector<CalCoreMeshPtr> m_vectorCoreMesh;
  std::vector<CalCoreMaterialPtr> m_vectorCoreMaterial;
  std::map<int, std::map<int, int> > m_mapmapCoreMaterialThread;
  Cal::UserData m_userData;
  std::map<std::string, int> m_animationName;
  std::map<std::string, int> m_materialName;
  std::map<std::string, int> m_meshName;
};

#endif

//****************************************************************************//
