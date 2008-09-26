/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#include "mathutil.h"

/**
 * \brief Helper method which substracts some rotation component from Quaternion
 * \param CalQuaternion& rot - rotation from which substraction will be done
 * \param CalQuaternion& val - rotation component whic will be substracted from 'rot'
 * \return CalQuaternion& - result of substraction
 */
CalQuaternion&  UTIL_diff_quats(CalQuaternion& rot, const CalQuaternion& val)
{
    //NOTE: inverted components     
	osg::Quat restRotQuat = CalQuatToQuat(rot); 
    //restRotQuat.normalise();
	osg::Quat valQuat = CalQuatToQuat(val);
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
 * \param CalVector& normal - normal of surface on which the angle is calculated
 * \return float - value -1 or 1 representing sign
 */
float UTIL_GetSignForDirChange(CalVector& oldDir, CalVector& newDir, CalVector& normal)
{
    CalVector rightDir = normal%oldDir;
    rightDir.normalize();
    float dot = newDir * rightDir;
    float angle = radToDeg( acos(UTIL_GetRightArgForArcCos(dot)) );
    if (angle < 90.0f)
        return -1.0f;
    else
        return 1.0f;
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

/*
void UTIL_DumpQuat(const CalQuaternion& quat, std::string caption)
{
    Quat rotQuat = CalQuatToQuat(quat);

    if (!caption.empty())
        _dbg << caption << ": ";

    _dbg << "Q(" << radToDeg(rotQuat.Xangle()) <<  "," << radToDeg(rotQuat.Yangle())
        << "," << radToDeg(rotQuat.Zangle()) << ")" << std::endl;
}
*/
/*void UTIL_DumpVect(const CalVector& vect, std::string caption)
{
    if (!caption.empty())
        _dbg << caption << ": ";

    _dbg << "V(" << vect.x <<  "," << vect.y  << "," << vect.z << ")"  << std::endl;
}
*/

/*
 * Checks if float number is valid 
 * \param loaft number to check
 * \return bool - true if is number is valid float, false otherwise (e.g. 1.#QNAN)
 */
bool UTIL_IsValidFloat(float number)
{
    return (number>0 || number<=0);
}

// get the x-rotation of the quaternion
// Few, finally found an easy way to do this
// it was in Ken Shoemake's original paper all along
// In this version I just call all the individual Euler
// the method works by combining the fomulae to go from 
// a quaternion to a matrix and then that to go from
// a matrix to Euler angles
// See Ken Shoemake "Animating Rotation with Quaternion 
// curves" (siggraph 85)
// I return minus the angle because the quaternion definition
// I use is the inverse of Shoemake's
float getXangle(const osg::Quat& q)
{
	float sinYangle = -(2.0f * q.x() * q.z() - 2.0f * q.y() * q.w());
	float cosYangle = sqrtf(1.0f - sinYangle * sinYangle);
	
	float sinXangle, cosXangle;
	if(fabs(cosYangle) < 0.0001)
	{
		sinXangle = -(2.0f * q.y() * q.z() - 2.0f * q.x() * q.w());
		cosXangle = 1.0f - 2.0f * q.x() * q.x() - 2.0f * q.z() * q.z();
	}
	else
	{
		sinXangle = 2.0f * q.y() * q.z() + 2.0f * q.x() * q.z();
		sinXangle /= cosYangle;
		cosXangle = 1.0f - 2.0f * q.x() * q.x() - 2.0f * q.y() * q.y();
		cosXangle /= cosYangle;
	}
	return -atan2f(sinXangle, cosXangle);
}
// get the y-rotation of the quaternion
// Few finally found an easy way to do this
// it was in Ken Shoemake's original paper all along
// In this version I just call all the individual Euler
// the method works by combining the fomulae to go from 
// a quaternion to a matrix and then that to go from
// a matrix to Euler angles
// See Ken Shoemake "Animating Rotation with Quaternion 
// curves" (siggraph 85)
// I return minus the angle because the quaternion definition
// I use is the inverse of Shoemake's
float getYangle(const osg::Quat& q)
{
	float sinYangle = -(2.0f * q.x() * q.z() - 2.0f * q.y() * q.w());
	return -asinf(sinYangle);
}

// get the z-rotation of the quaternion
// Few finally found an easy way to do this
// it was in Ken Shoemake's original paper all along
// In this version I just call all the individual Euler
// the method works by combining the fomulae to go from 
// a quaternion to a matrix and then that to go from
// a matrix to Euler angles
// See Ken Shoemake "Animating Rotation with Quaternion 
// curves" (siggraph 85)
// I return minus the angle because the quaternion definition
// I use is the inverse of Shoemake's
float getZangle(const osg::Quat& q)
{
	float sinYangle = -(2.0f * q.x() * q.z() - 2.0f * q.y() * q.w());
	float cosYangle = sqrtf(1.0f -sinYangle * sinYangle);
	
	if(fabs(cosYangle) < 0.0001)
	{
		return 0.0f;
	}
	else
	{
		float sinZangle, cosZangle;
		sinZangle = 2.0f * q.x() * q.y() + 2.0f * q.z() * q.w();
		sinZangle /= cosYangle;
		cosZangle = 1.0f - 2.0f * q.y() * q.y() - 2.0f * q.z() * q.z();
		cosZangle /= cosYangle;
		return -atan2f(sinZangle, cosZangle);
	}
};
