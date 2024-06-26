//****************************************************************************//
// saver.h                                                                    //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_SAVER_H
#define CAL_SAVER_H

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "cal3d/src/global.h"
#include "cal3d/src/vector.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalCoreModel;
class CalCoreSkeleton;
class CalCoreBone;
class CalCoreAnimation;
class CalCoreMesh;
class CalCoreSubmesh;
class CalCoreMaterial;
class CalCoreKeyframe;
class CalCoreTrack;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The saver class.
  *****************************************************************************/

class CalSaverAnimationOptions {
public:
	bool bCompressKeyframes;

protected:
	CalVector keyframe_min;
	CalVector keyframe_scale;
	float duration;

	friend class CalSaver;
};

class CAL3D_API CalSaver
{
public:
  static bool saveCoreAnimation(const std::string& strFilename, CalCoreAnimation *pCoreAnimation, CalSaverAnimationOptions *pOptions = NULL);
  static bool saveCoreMaterial(const std::string& strFilename, CalCoreMaterial *pCoreMaterial);
  static bool saveCoreMesh(const std::string& strFilename, CalCoreMesh *pCoreMesh);
  static bool saveCoreSkeleton(const std::string& strFilename, CalCoreSkeleton *pCoreSkeleton);

protected:
  static bool saveCoreBones(std::ofstream& file, const std::string& strFilename, CalCoreBone *pCoreBone);
  static bool saveCoreKeyframe(std::ofstream& file, const std::string& strFilename, CalCoreKeyframe *pCoreKeyframe);
  static bool saveCompressedCoreKeyframe(std::ofstream& file, const std::string& strFilename, CalCoreKeyframe *pCoreKeyframe, CalSaverAnimationOptions *pOptions);
  static bool saveCoreSubmesh(std::ofstream& file, const std::string& strFilename, CalCoreSubmesh *pCoreSubmesh);
  static bool saveCoreTrack(std::ofstream& file, const std::string& strFilename, CalCoreTrack *pCoreTrack, CalSaverAnimationOptions *pOptions = NULL);

  static bool saveXmlCoreSkeleton(const std::string& strFilename, CalCoreSkeleton *pCoreSkeleton);
  static bool saveXmlCoreAnimation(const std::string& strFilename, CalCoreAnimation *pCoreAnimation);
  static bool saveXmlCoreMesh(const std::string& strFilename, CalCoreMesh *pCoreMesh);
  static bool saveXmlCoreMaterial(const std::string& strFilename, CalCoreMaterial *pCoreMaterial);  
};

#endif

//****************************************************************************//
