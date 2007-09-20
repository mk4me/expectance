/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once
#ifndef _GEN_SCENE_OBJECT_H
#define _GEN_SCENE_OBJECT_H

#include "../core/baseobject.h"
#include "../utility/mathconversions.h"

namespace ft
{
	//! A SceneObject class
	/*!
	 *	This is the base class for rendering. It contains all basic properties utilized by other scene objects 
	 */
	class SceneObject: public virtual BaseObject
	{
	public:
		/// \brief Default constructor. Creates object on the scene. Sets position on (0,0,0), 
		///		   orientation on (0,0,1), white color and object as visible
		SceneObject(void);
		virtual~SceneObject(void) {};
		/// \brief virtual base for inherited objects for rendering purposes
		virtual bool Render();		
		/// \brief virtual base for inherited objects for rendering shadows
		virtual bool RenderShadow();
		
		/// \brief Sets object position by 3D vector
		SceneObject& setPosition(const CalVector &position);
		/// \brief Sets object orientation by quaternion 
		SceneObject& setOrientation(const CalQuaternion &orientation);
		/// \brief Sets object color by vector of RGB values 
		SceneObject& setColor(const CalVector &color);
		//! get the value of object position 
		const CalVector& getPosition() const;
		//! get the value of object orientation by 3D vector 
		//const CalVector& getOrientation() const;
		////! get the value of object orientation by quaternion
		const CalQuaternion& getOrientation() const;

		//! get the value of object color
		const CalVector& getColor() const;
		//! show object
		void Show();
		//! hide object
		void Hide();

		/// \brief Tests if object is visible (for rendering purposes)
		const bool isVisible() const;
		/// \brief Prints general information about values of object properties  
		const void PrintInfo(void) const;

	protected:
		bool m_visible;
		CalVector m_position, m_orient;
		CalQuaternion m_orientation;
		CalVector m_color;
		float m_alpha;
	};
};

#endif //_GEN_SCENE_OBJECT_H