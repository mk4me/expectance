/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once
#ifndef _GEN_SCENE_OBJECT_H
#define _GEN_SCENE_OBJECT_H

#define _CRT_SECURE_NO_WARNINGS 1

#include "../core/global.h"


namespace ft
{
	class SceneObject
	{
	public:
		SceneObject(void);
		~SceneObject(void);
		virtual bool Render()=0;

		void setPosition(const FTVect position);
		void setOrientation(const FTVect orientation);
		void setColor(const FTVect color);
		void setID(std::string ID);		
		FTVect getPosition() const;
		FTVect getColor() const;
		std::string getID() const;
		void PrintInfo(void) const;
	protected:
		FTVect m_position, m_orientation;
		FTVect m_color;
		float m_alpha;
		std::string m_id;
	};
};

#endif //_GEN_SCENE_OBJECT_H