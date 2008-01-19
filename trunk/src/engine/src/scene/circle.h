/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once

#ifndef CIRCLE_H
#define CIRCLE_H

#include "sceneobject.h"

namespace ft {
	//! A Circle class
	/*!
	 *	This class creates circle or disk (filled circle). It is possible to define: radius, color and segments number. 
	 */
	class ENGINE_API Circle : public SceneObject  
	{
	public:
		//! construct circle given by name
		Circle(const std::string& name);
		virtual ~Circle(void);

		//! render circle
		virtual bool Render();

		//! set all parameters for circle
		void setParameters(const float radius, const CalVector& center, 
			const CalVector& color = CalVector(1,1,1), const int segments = 20, const bool filled = false);

		//! set the value of circle radius
		Circle& setRadius(const float radius);

		//! set the sort of circle shape: false - circle, true - disk
		Circle& setDisk(const bool filled);

		//! set the value of segments (20 by default)
		Circle& setSegmentsNumber(const int segments);

	private:
		float m_radius;
		bool m_filled;
		int m_segments;
	};
}

#endif //CIRCLE_H