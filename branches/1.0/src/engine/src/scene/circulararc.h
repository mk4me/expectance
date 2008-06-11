/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once

#ifndef CIRCULAR_ARC_H
#define CIRCULAR_ARC_H

#include "sceneobject.h"

namespace ft {
	//! A CircularArc class
	/*!
	 *	This class creates circularArc or disk (filled circularArc). It is possible to define: radius, color and segments number. 
	 */
	class ENGINE_API CircularArc : public SceneObject  
	{
	public:
		//! construct circularArc given by name
		CircularArc(const std::string& name);
		virtual ~CircularArc(void);

		//! render circularArc
		virtual bool Render();


		//! set the value of circularArc radius
		CircularArc& setRadius(const float radius);

		//! set the sort of circularArc shape: false - circularArc, true - disk
		CircularArc& setDisk(const bool filled);

		//! set the value of segments (20 by default)
		CircularArc& setSegmentsNumber(const int segments);

		//! set the value of start vector 
		CircularArc& setStart(const CalVector& start);

		//! set the value of end vector
		CircularArc& setEnd(const CalVector& end);


		//! set all parameters for circularArc
		void setParameters(const float radius,  const CalVector& start, const CalVector& end, const CalVector& center, 
			const CalVector& color = CalVector(1,1,1), const int segments = 20, const bool filled = false);



	private:
		float m_radius;
		bool m_filled;
		int m_segments;
		CalVector m_start, m_end;
	};
}

#endif //CIRCULAR_ARC_H
