/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once

#ifndef GEN_TRACE_LINE_H
#define GEN_TRACE_LINE_H

#include "../core/global.h"
#include "sceneobject.h"
#include "line.h"

namespace ft {
	//! A TraceLine class
	/*!
	 *	This class creates collection of points/lines reciprocally conected. This is useful for graphics objects debugging on the scene. 
	 */
	class TraceLine : public SceneObject
	{
	public:
		/*! Constructs object with a given name */
		TraceLine(const std::string& name);
		virtual ~TraceLine(void) { /* empty */};
		/*! \brief Renders line with point at the end of it */
		virtual bool Render();
		/*! \brief Adds new point to the collection for rendering*/
		void AddPoint(const FTVect& point);
		//! clear collection of existing points
	    void ClearTrace();
		//! set multicolor mode (true)
		void setColorMix(bool mix);
		//! set marker visible 
		void ShowMarker();
		//! set marker invisible
		void HideMarker();
	protected:
		std::vector <FTVect> m_traces;
		bool m_colorMix;
		bool m_marker;
	};
};

#endif // GEN_TRACE_LINE