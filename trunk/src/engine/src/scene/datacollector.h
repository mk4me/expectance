/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#pragma once

#ifndef GEN_DATA_COLLECTOR_H
#define GEN_DATA_COLLECTOR_H

#include "../core/global.h"
#include "sceneobject.h"
#include "line.h"

namespace ft {
	//! A DataCollector class
	/*!
	 *	This class creates collection of data and creates points/lines reciprocally conected. This is useful for graphics objects debugging on the scene. 
	 */
	class DataCollector : public SceneObject
	{
	public:
		/*! Constructs object with a given name */
		DataCollector(const std::string& name);
		virtual ~DataCollector(void) { /* empty */};

		/*! \brief Renders line with point at the end of it */
		virtual bool Render();
		/*! \brief Add new value to the collection for rendering*/
		void AddValue(const float value);
		
		/*\brief get max value from data set */
		const float getMax(){return m_max;};
		/*\brief get max value from data set */
		const float getMin(){return m_min;};

		//! clear collection of existing points
	    void Clear();
		//! set points visible 
		void ShowPoints();
		//! set points invisible
		void HidePoints();
		//! set blending for tracer on or off
		void setBlending(bool blending);
		//! set size of buffer for data 0 means neverending buffer size
		void setBufferSize(const long bufferSize);

        void setDrawScale(float scale) { m_drawScale = scale; }
        float getDrawScale() { return m_drawScale; }

        void setDrawOffset(float offset) { m_drawOffset = offset; }
        float getDrawOffset() { return m_drawOffset; }


	protected:
		std::list <float> m_DataList;
		float m_min, m_max, m_scope;
		bool m_blending, m_points;
		unsigned long m_bufferSize;

        float m_drawScale;
        float m_drawOffset;
	};
};

#endif //GEN_DATA_COLLECTOR_H
