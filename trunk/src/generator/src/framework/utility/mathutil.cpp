/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "mathutil.h"

using namespace ft;
/**
 * \brief Helper method which substracts some rotation component from Quaternion
 * \param CalQuaternion& rot - rotation from which substraction will be done
 * \param CalQuaternion& val - rotation component whic will be substracted from 'rot'
 * \return CalQuaternion& - result of substraction
 */
CalQuaternion&  UTIL_diff_quats(CalQuaternion& rot, CalQuaternion& val)
{
    //NOTE: inverted components     
    Quat restRotQuat = CalQuatToQuat(rot); 
    //restRotQuat.normalise();
    Quat valQuat = CalQuatToQuat(val);
    //valQuat.normalise();
    restRotQuat /= valQuat;
    rot = QuatToCalQuat(restRotQuat);

    return rot;
}

/**
 * \brief Helper method which determine sign of angle for rotation of oldDir to newDir
 *      NOTE that passed vector arguments should be normalized
 * \param CalVector& oldDir - vertor which is the base for angle sign determiation
 * \param CalVector& newDir - vector for which angle sign in relation to oldDir
 * \return float - value -1 or 1 representing sign
 */
float UTIL_GetSignForDirChange(CalVector& oldDir, CalVector& newDir)
{
    CalVector rightDir = CalVector(0,1,0)%oldDir;
    rightDir.normalize();
    float dot = newDir * rightDir;
    float angle = radToDeg( acos(UTIL_GetRightArgForArcCos(dot)) );
    if (angle < 90.0f)
        return -1;
    else
        return 1;
}

/**
 * \brief Helper method which tunes arccos argument to correct scope (-1,1)
 * \param float arg - correct argument for arccos fucntion
 */
float UTIL_GetRightArgForArcCos(float arg)
{
    if (arg > 1.0f)
        arg = 1.0f;
    else if (arg < -1.0f)
        arg = -1.0f;
    return arg;
}

