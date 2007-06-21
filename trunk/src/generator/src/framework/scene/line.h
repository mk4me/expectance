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
		Line(std::string id);
		Line(FTVect &start, FTVect &end, const std::string id);
		Line(FTVect &position, FTVect &orientation, const float length, const std::string id);
		Line(FTVect &start, FTVect &end, const std::string id, FTVect &color);
		virtual ~Line(void);
		virtual bool Render();
		bool isActive();
	protected:
		bool m_active;
		FTVect m_start;
		FTVect m_end;
		float m_length;
	};
};

#endif