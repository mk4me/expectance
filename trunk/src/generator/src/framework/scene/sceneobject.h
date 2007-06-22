/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once
#ifndef _GEN_SCENE_OBJECT_H
#define _GEN_SCENE_OBJECT_H

#define _CRT_SECURE_NO_WARNINGS 1

#include "../core/baseobject.h"


namespace ft
{
	class SceneObject: public BaseObject
	{
	public:
		SceneObject(void);
		~SceneObject(void);
		virtual bool Render()=0;

		SceneObject& setPosition(const FTVect &position);
		SceneObject& setOrientation(const FTVect &orientation);
		SceneObject& setColor(const FTVect &color);
		
		const FTVect& getPosition() const;
		const FTVect& getOrientation() const;
		const FTVect& getColor() const;

		const bool isVisible() const;

		void PrintInfo(void) const;
	protected:
		bool m_visible;
		FTVect m_position, m_orientation;
		FTVect m_color;
		float m_alpha;

	};
};

#endif //_GEN_SCENE_OBJECT_H