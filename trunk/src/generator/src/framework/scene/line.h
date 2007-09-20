/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once

#ifndef LINE_H
#define LINE_H

#include "../core/global.h"
#include "sceneobject.h"

namespace ft {
	//! A Line class
	/*!
	 *	This class creates line. It is possible to define: start and end of the line, color, length, arrow. 
	 */
	class Line : public SceneObject  
	{
	public:
		//! construct line given by name
		Line(const std::string& name);
		/// \brief create line given by id and set if arrow is visible
		Line(const std::string& id, bool arrow);
		/// \brief create line given by start, end and label
		Line(const CalVector& start, const CalVector& end, const std::string& name);
		/// \brief create line given by position, orientation, length and label
		Line(const CalVector& position, const Quat& orientation, float length, const std::string& name);
		virtual ~Line(void);
		//! render line
		virtual bool Render();
		//! set the value of line length
		Line& setLenght(const float lenght);
		//! set the value of line start point
		Line& setStart(const CalVector &start);
		//! set the value of line end point
		Line& setEnd(const CalVector &end);
		//! set Arrow visible or not
		Line& setArrow(const bool arrow);

	private:
		CalVector m_start;
		CalVector m_end;
		float m_lenght;
		bool m_arrow;
	};
};

#endif