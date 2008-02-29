/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once

#ifndef GEN_TRACE_LINE_H
#define GEN_TRACE_LINE_H

#include "sceneobject.h"


namespace ft {
	//! A TraceLine class
	/*!
	 *	This class creates collection of points/lines reciprocally conected. This is useful for graphics objects debugging on the scene. 
	 */
	class ENGINE_API TraceLine : public SceneObject
	{
	public:
		/*! Constructs object with a given name */
		TraceLine(const std::string& name);
		virtual ~TraceLine(void) { /* empty */};

		/*! \brief Renders line with point at the end of it */
		virtual bool Render();
		/*! \brief Adds new point to the collection for rendering*/
		void AddPoint(const CalVector& point);
		//! clear collection of existing points
	        void ClearTrace();
		//! set multicolor mode (true)
		void setColorMix(bool mix);
		//! set marker visible 
		void ShowMarker();
		//! set marker invisible
		void HideMarker();
		//! set line visible 
		void ShowLine();
		//! set line invisible
		void HideLine();
		//! set shape of marker
		void setMarkerShape(const MarkerShape shape);
		//! set marker Color
		void setMarkerColor(const CalVector color);
		//! set marker scale
		void setMarkerScale (const int scale);
		//! set blending for tracer on or off
		void setBlending(bool blending);
		//! set size of buffer for data 0 means neverending buffer size
		void setBufferSize(const long bufferSize);

	protected:
		std::vector <CalVector> m_traces;
		bool m_colorMix;
		bool m_marker;
		bool m_line;
		bool m_blending;
		unsigned long m_bufferSize;
		int m_markerScale;
		MarkerShape m_shape;
		CalVector m_markerColor;
	};
}

#endif // GEN_TRACE_LINE
