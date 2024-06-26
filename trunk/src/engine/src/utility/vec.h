/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Vec.h.
 *
 * The Initial Developer of the Original Code is Marco (Mark) Gillies.
 * Portions created by the Initial Developer are Copyright (C) BT plc. 2004
 * All Rights Reserved.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef PIAVCA_VEC_H
#define PIAVCA_VEC_H

//#include "PiavcaDefs.h"

#include <cmath>
#include <float.h>
#include <vector>
#include "../core/platform.h"

#ifdef WIN32
	#define isnan _isnan
#endif

namespace ft {

//! A Vector class
/*!
 *	This should be a fairly standard implementation of 3-space vectors, with most normal operations.
 *	They are represented as an array of three floats.
 */ 
class ENGINE_API Vec
{
	float vals[3];
public:

	Vec();
	Vec(float x, float y, float z);
	Vec(const float v[3]);

	Vec(const Vec &v);
	const Vec &operator=(const Vec &v)		
	{
		vals[0] = v[0]; 
		vals[1] = v[1]; 
		vals[2] = v[2]; 
		return (*this);
	};

	~Vec();

	//! \name use inline methods to access components (better encapsulation)
	//!@{
	float &X()								{return vals[0];};
	float &Y()								{return vals[1];};
	float &Z()								{return vals[2];};

	const float &X() const					{return vals[0];};
	const float &Y() const					{return vals[1];};
	const float &Z() const					{return vals[2];};
    //!@}

	//! conveneience functions, get the axes
	//!@{
	static Vec XAxis(){return Vec(1.0, 0.0, 0.0);};
	static Vec YAxis(){return Vec(0.0, 1.0, 0.0);};
	static Vec ZAxis(){return Vec(0.0, 0.0, 1.0);};
	//!@}
	
	//! access members by index
	float &operator[](int i)				{return vals[i];};
	const float &operator[](int i) const	{return vals[i];};

	//! length of the vector
	float mag()	const;

	//! \name algebraic operators
	//!@{
	const Vec operator+(const Vec &v) const	{Vec retVal(X(), Y(), Z()); return retVal += v;};
	const Vec &operator+=(const Vec &v)		{X() += v.X(); Y() += v.Y(); Z() += v.Z(); return *this;};
	const Vec operator+(float f) const		{Vec retVal(X(), Y(), Z()); return retVal += f;};
	const Vec &operator+=(float f)			{X() += f; Y() += f; Z() += f; return *this;};

	const Vec operator-(const Vec &v) const	{Vec retVal(X(), Y(), Z()); return retVal -= v;};
	const Vec &operator-=(const Vec &v)		{X() -= v.X(); Y() -= v.Y(); Z() -= v.Z(); return *this;};
	const Vec operator-(float f) const		{Vec retVal(X(), Y(), Z()); return retVal -= f;};
	const Vec &operator-=(float f)			{X() -= f; Y() -= f; Z() -= f; return *this;};
	const Vec operator-() const				{return Vec(-X(), -Y(), -Z());};

	const Vec operator*(const Vec &v) const	{Vec retVal(X(), Y(), Z()); return retVal *= v;};
	const Vec &operator*=(const Vec &v)		{X() *= v.X(); Y() *= v.Y(); Z() *= v.Z(); return *this;};
	const Vec operator*(float f) const		{Vec retVal(X(), Y(), Z()); return retVal *= f;};
	const Vec &operator*=(float f)			{X() *= f; Y() *= f; Z() *= f; return *this;};

	const Vec operator/(const Vec &v) const {Vec retVal(X(), Y(), Z()); return retVal /= v;};
	const Vec &operator/=(const Vec &v)		{X() /= v.X(); Y() /= v.Y(); Z() /= v.Z(); return *this;};
	const Vec operator/(float f) const		{Vec retVal(X(), Y(), Z()); return retVal /= f;};
	const Vec &operator/=(float f)			{X() /= f; Y() /= f; Z() /= f; return *this;};
	//!@}

	//! \name comparison operators
	//!@{
	bool operator==(const Vec &v)const {return X() == v.X() && Y() == v.Y() && Z() == v.Z();};
	bool operator!=(const Vec &v)const {return !((*this) == v);};
	//!@}

	/*! \name invert the vector 
	 *	I use a convention that a noun (inverse) returns a value but does not 
	 *	change the vector while a verb (invert) performs the operation on the
	 *	vector itself (not sure if its a good convention but its the best I could
	 *	think of)
	 */
	//!@{
	//! returns the inverse of the vector while leaving it unchanged
	Vec inverse() const						{return Vec(-X(), -Y(), -Z());};
	//! inverts the vector in place
	void invert()							{X() = -X(); Y() = -Y(); Z() = -Z();};
	//!@}

	//! scalar (dot) product
	float dot(const Vec &v) const;
	/*! vector (cross) product */
	Vec cross(const Vec &v) const ;
	//! in place cross product
	const Vec &crossinplace(const Vec &v)	{return (*this) = cross(v);};

	/*!	\name normalize the vector
	 *	I use a convention that a noun (normalize) returns a value but does not 
	 *	change the vector while a verb (normalized) performs the operation on the
	 *	vector itself (not sure if its a good convention but its the best I could
	 *	think of)
	 */
	//!@{
	//! gets a normalized version of the Vec while leaving it unchanged
	Vec normalized() const					{return (*this)/mag();};
	//! normalize the Vec in place
	void normalize()						{(*this) /= mag();};
	//!@}
};



/**
 * \name verify if there is any NaN element of vector
 *
 * @param ft::Vec & v - vector
 * @return bool - True if not NaN otherwise False
 **/
inline bool checkNaN(Vec &v)
{
	return isnan(v[0]) || isnan(v[1]) || isnan(v[2]);
}

}


//#include <iostream>

//std::ostream &operator<<(std::ostream &os, const Vec &v);
//std::istream &operator>>(std::istream &is, Vec &v);


#endif //PIAVCA_VEC_H
