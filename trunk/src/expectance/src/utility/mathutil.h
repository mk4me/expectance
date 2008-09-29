/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_MATHUTIL_DATA_H
#define _FT_MATHUTIL_DATA_H

#include "cal3d/vector.h"
#include "cal3d/quaternion.h"
#include <osg/Quat>
//#include <osg/vec3d>

CalQuaternion&  UTIL_diff_quats(CalQuaternion& rot, const CalQuaternion& val);
float UTIL_GetSignForDirChange(CalVector& oldDir, CalVector& newDir, CalVector& normal);
float UTIL_GetRightArgForArcCos(float arg);
float UTIL_GetVectorsAngle(const CalVector& vect1, const CalVector& vect2);
//	void UTIL_DumpQuat(const CalQuaternion& quat, std::string caption = "");
//	void UTIL_DumpVect(const CalVector& vect, std::string caption = "");
bool UTIL_IsValidFloat(float number);

//! convert quaternion from Cal3D to ft::Quat representation  
inline osg::Quat CalQuatToQuat(const CalQuaternion &cq) {
	return osg::Quat(cq.w, cq.x, cq.y, cq.z);
};

//! convert quaternion from ft::Quat to Cal3D representation  
inline CalQuaternion QuatToCalQuat(const osg::Quat &q) {
	return CalQuaternion(q.x(), q.y(), q.z(), q.w());
};

//! Pi the constant
const float Pi = 3.14159265358979323846f;

//! convert and angle in degrees to radians
inline float degToRad(float angle){return angle*Pi/180.0f;};

//! convert and angle in radians to degrees
inline float radToDeg(float angle){return angle*180.0f/Pi;};

float getXangle(const osg::Quat& q);
float getYangle(const osg::Quat& q);
float getZangle(const osg::Quat& q);


#endif //_FT_MATHUTIL_DATA_H

