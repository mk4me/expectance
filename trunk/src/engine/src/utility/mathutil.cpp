/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "mathutil.h"
#include "debug.h"

using namespace ft;
/**
 * \brief Helper method which substracts some rotation component from Quaternion
 * \param CalQuaternion& rot - rotation from which substraction will be done
 * \param CalQuaternion& val - rotation component whic will be substracted from 'rot'
 * \return CalQuaternion& - result of substraction
 */
CalQuaternion&  UTIL_diff_quats(CalQuaternion& rot, const CalQuaternion& val)
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

/*
 * Returns angle between two vectors
 * \param
 * \param
 * \return float - angle bewteen vectors, (-) when vect2 is on on the right side of vect1, (+) otherwise
 */
float UTIL_GetVectorsAngle(const CalVector& vect1, const CalVector& vect2)
{
    float dot = vect1*vect2;
    dot = UTIL_GetRightArgForArcCos(dot);

    return acos(dot);
}

void UTIL_DumpQuat(const CalQuaternion& quat, std::string caption)
{
    Quat rotQuat = CalQuatToQuat(quat);

    if (!caption.empty())
        _dbg << caption << ": ";

    _dbg << "Q(" << radToDeg(rotQuat.Xangle()) <<  "," << radToDeg(rotQuat.Yangle())
        << "," << radToDeg(rotQuat.Zangle()) << ")" << std::endl;
}

void UTIL_DumpVect(const CalVector& vect, std::string caption)
{
    if (!caption.empty())
        _dbg << caption << ": ";

    _dbg << "V(" << vect.x <<  "," << vect.y  << "," << vect.z << ")"  << std::endl;
}

