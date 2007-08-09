/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once
#ifndef _GEN_SCENE_OBJECT_H
#define _GEN_SCENE_OBJECT_H

#include "../core/baseobject.h"
#include "../utility/Cal3dMathsConversion.h"

namespace ft
{
	/**
	 * Class SceneObject: <describe the behavior and responsibility of this class>
	 **/
	class SceneObject: public virtual BaseObject
	{
	public:
		SceneObject(void);
		virtual~SceneObject(void) {};
		virtual bool Render();

		SceneObject& setPosition(const FTVect &position);
		SceneObject& setOrientation(const FTVect &orientation);
		SceneObject& setColor(const FTVect &color);
		
		const FTVect& getPosition() const;
		const FTVect& getOrientation() const;
		const FTVect& getColor() const;
		void Show();
		void Hide();

		const bool isVisible() const;
		const void PrintInfo(void) const;
		CalQuaternion m_vRotation; //tmp
	protected:
		bool m_visible;
		FTVect m_position, m_orientation;
		FTVect m_color;
		float m_alpha;

	};
};

#endif //_GEN_SCENE_OBJECT_H