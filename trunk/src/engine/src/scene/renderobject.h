/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once
#ifndef _GEN_RENDERING_OBJECT_H
#define _GEN_RENDERING_OBJECT_H

#include "../core/global.h"

namespace ft
{
	//! A RenderObject class
	/*!
	 *	This is the base class for rendering. It contains all basic rendering interface utilized by other scene objects 
	 */
	class RenderObject
	{
	public:
		RenderObject(void);
		virtual ~RenderObject(void){};
		/// \brief virtual base for inherited objects for rendering purposes
		virtual bool Render();		
		/// \brief virtual base for inherited objects for rendering shadows
		virtual bool RenderShadow();
		//! get the value of object rendering order: 1 - static objects, 2 - tracers and other dodads, 3 - dynamic objects
		const byte getRenderingOrder();
		//! set the value of object rendering order: 1 - static objects, 2 - tracers and other dodads, 3 - dynamic objects
		RenderObject& setRenderingOrder(const RenderingOrder ro);

		//! get the value of object color
		const CalVector& getColor() const;
		/// \brief Sets object color by vector of RGB values 
		RenderObject& setColor(const CalVector &color);

		/// \brief Sets object visible or not
		RenderObject& setVisible( bool vis);

		/// \brief Tests if object is visible (for rendering purposes)
		const bool isVisible() const;

	protected:
		RenderingOrder m_renderingOrder;
		bool m_visible;
		CalVector m_color;
	};
}

#endif //_GEN_RENDERING_OBJECT_H