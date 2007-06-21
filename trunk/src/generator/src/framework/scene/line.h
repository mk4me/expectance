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
	class Line : public SceneObject  
	{
	public:
		Line(const std::string& id);
		Line(const std::string& id, bool arrow);
		Line(const FTVect& start, const FTVect& end, const std::string& id);
		Line(const FTVect& position, const FTVect& orientation, float length, const std::string& id);
		virtual ~Line(void);
		virtual bool Render();
		Line& setLenght(const float lenght);
		Line& setStart(const FTVect &start);
		Line& setEnd(const FTVect &end);
		Line& setArrow(const bool arrow);
		void Show();
		void Hide();


	protected:
		FTVect m_start;
		FTVect m_end;
		float m_lenght;
		bool m_arrow;
	};
};

#endif