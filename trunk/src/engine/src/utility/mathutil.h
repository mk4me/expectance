/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "mathconversions.h"

CalQuaternion&  UTIL_diff_quats(CalQuaternion& rot, const CalQuaternion& val);
float UTIL_GetSignForDirChange(CalVector& oldDir, CalVector& newDir);
float UTIL_GetRightArgForArcCos(float arg);
float UTIL_GetVectorsAngle(const CalVector& vect1, const CalVector& vect2);
void UTIL_DumpQuat(const CalQuaternion& quat, std::string caption = "");
void UTIL_DumpVect(const CalVector& vect, std::string caption = "");
bool UTIL_IsValidFloat(float number);