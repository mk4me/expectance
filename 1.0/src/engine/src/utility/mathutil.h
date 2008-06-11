/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "mathconversions.h"
#include "../core/platform.h"

ENGINE_API CalQuaternion&  UTIL_diff_quats(CalQuaternion& rot, const CalQuaternion& val);
ENGINE_API float UTIL_GetSignForDirChange(CalVector& oldDir, CalVector& newDir);
ENGINE_API float UTIL_GetRightArgForArcCos(float arg);
ENGINE_API float UTIL_GetVectorsAngle(const CalVector& vect1, const CalVector& vect2);
ENGINE_API void UTIL_DumpQuat(const CalQuaternion& quat, std::string caption = "");
ENGINE_API void UTIL_DumpVect(const CalVector& vect, std::string caption = "");
ENGINE_API bool UTIL_IsValidFloat(float number);
