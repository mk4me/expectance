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
		///		   orientation on (0,0,0), direction according to positive Z (0,0,1), white color and object as visible
		SceneObject(void);
		virtual~SceneObject(void) {};

		/// \brief virtual base for inherited objects for rendering purposes
		virtual bool Render();		
		/// \brief virtual base for inherited objects for rendering shadows
		virtual bool RenderShadow();

		//! get the value of object rendering order: 1 - static objects, 2 - tracers and other dodads, 3 - dynamic objects
		const byte getRenderingOrder();
		//! set the value of object rendering order: 1 - static objects, 2 - tracers and other dodads, 3 - dynamic objects
		SceneObject& setRenderingOrder(const RenderingOrder ro);

		/// \brief Sets object position to given position value
		SceneObject& setPosition(const CalVector &position);
		/// \brief Changes object position by given deltaPosition value
		SceneObject& changePosition(const CalVector &deltaPosition);

		/// \brief Sets object orientation to given rotation value
		SceneObject& setOrientation(const CalQuaternion &rotation);
		/// \brief Changes object orientation by given deltaRotation value
		SceneObject& changeOrientation(const CalQuaternion &deltaRotation);

		/// \brief Sets object global rotation offset to given rotation value
		SceneObject& setGlobalRotationOffset(const CalQuaternion &rotation);
		/// \brief Changes object global rotation offset by given deltaRotation value
		SceneObject& changeGlobalRotationOffset(const CalQuaternion &deltaRotation);


		/// \brief Sets object direction to given direction value
		SceneObject& setDirection(const CalVector &direction);
		/// \brief Sets object direction to given direction value
		SceneObject& setDirection(const CalQuaternion &direction);
		/// \brief Changes object direction by given direction value
		SceneObject& changeDirection(const CalVector &deltaDirection);
		/// \brief Changes object direction by 3D unit vector
		SceneObject& changeDirection(const CalQuaternion &deltaDirection);

		/// \brief Sets object color by vector of RGB values 
		SceneObject& setColor(const CalVector &color);

		//! get the value of object position 
		const CalVector& getPosition() const;
		////! get the value of object orientation by quaternion
		const CalQuaternion& getOrientation() const;
		////! get the value of object orientation by quaternion
		const CalQuaternion& getGlobalRotationOffset() const;
		//! get the value of object direction by 3D vector 
		const CalVector& getDirection() const;

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
		RenderingOrder m_renderingOrder;
		bool m_visible;
		CalVector m_position,            // position (x,y,z)
			      m_direction;	         // direction of object (ux,uy,uz)
		CalQuaternion m_orientation;     // rotation of object (axis of rotation and angle of rotation)
        CalQuaternion m_globalRotationOffset;     // offset added to rotation of object 
		CalVector m_color;
		float m_alpha;
	};
};

#endif //_GEN_SCENE_OBJECT_H