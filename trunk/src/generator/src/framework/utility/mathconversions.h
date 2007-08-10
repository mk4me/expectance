/**
 * File        : mathconversions.h
 * Project     : generator
 * Description : Bases on Marco Gillies code
 *
 * @author abak, mka
 **/

#include <limits>

#include "Quat.h"
#include "Vec.h"
#include <cal3d/cal3d.h>

using ft::Vec;
using ft::Quat;

namespace ft
{
	//! convert vector from Cal3D to ft::Vec representation  
	inline Vec CalVecToVec(const CalVector &cv) {
	  return Vec(cv.x, cv.y, cv.z);
	};

	//! convert vector from ft::Vec to Cal3D representation  
	inline CalVector VecToCalVec(const Vec &v) {
	  return CalVector(v[0], v[1], v[2]);
	};

	//! convert quaternion from Cal3D to ft::Quat representation  
	inline Quat CalQuatToQuat(const CalQuaternion &cq) {
	  return Quat(cq.w, cq.x, cq.y, cq.z);
	};

	//! convert quaternion from ft::Quat to Cal3D representation  
	inline CalQuaternion QuatToCalQuat(const Quat &q) {
	  return CalQuaternion(q.I(), q.J(), q.K(), q.S());
	};

	//! Pi the constant
	const float Pi = 3.14159265358979323846f;

	//! convert and angle in degrees to radians
	inline float degToRad(float angle){return angle*Pi/180.0f;};

	//! convert and angle in radians to degrees
	inline float radToDeg(float angle){return angle*180.0f/Pi;};
};