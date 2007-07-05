/* LICENCE BLOCK
Cal3DMathsConversion.h
Copyright (C) 2005 Marco (Mark) Gillies

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <limits>

//#include "PlatformDefs.h"
#include "Quat.h"
#include "Vec.h"

using ft::Vec;
using ft::Quat;

#include <cal3d/cal3d.h>


inline Vec CalVecToVec(const CalVector &cv) {
  return Vec(cv.x, cv.y, cv.z);
};

inline CalVector VecToCalVec(const Vec &v) {
  return CalVector(v[0], v[1], v[2]);
};

inline Quat CalQuatToQuat(const CalQuaternion &cq) {
  return Quat(cq.w, cq.x, cq.y, cq.z);
};

inline CalQuaternion QuatToCalQuat(const Quat &q) {
  return CalQuaternion(q.I(), q.J(), q.K(), q.S());
};

//! Pi the constant
const float Pi = 3.14159265358979323846f;
//! convert and angle in degrees to radians
inline float degToRad(float angle){return angle*Pi/180.0f;};
//! convert and angle in radians to degrees
inline float radToDeg(float angle){return angle*180.0f/Pi;};
