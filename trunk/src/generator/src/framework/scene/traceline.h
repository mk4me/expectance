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
	class TraceLine : public SceneObject
	{
	public:
		TraceLine(const std::string& name);
		virtual ~TraceLine(void) { /* empty */};
		virtual bool Render();

		void AddPoint(const FTVect& point);
	    void ClearTrace();
		
		void setColorMix(bool mix);
		void ShowMarker();
		void HideMarker();
	protected:
		std::vector <FTVect> m_traces;
		bool m_colorMix;
		bool m_marker;
	};
};

#endif // GEN_TRACE_LINE