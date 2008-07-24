/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_FIELD_H
#define _FT_FIELD_H

#include "scene/sceneobject.h"
#include "scene/traceline.h"
#include "ball.h"

namespace ft
{
	/**
	 * Manages main game flow
	 **/
	class Field: public SceneObject
    {
	public:
		static CalVector NORTH;
		static CalVector SOUTH;
		static CalVector WEST;
		static CalVector EAST;

		Field(void);

		void setCorners(const CalVector &corner_NW, const CalVector &corner_NE, 
								const CalVector &corner_SE, const CalVector &corner_SW);
		void InitFieldObjects();
		void InitLines();
	
		/// \brief virtual base for inherited objects for rendering purposes
		virtual bool Render();

		const CalVector& getLeftGatePos() { return m_leftGatePos; }
		const CalVector& getRightGatePos() { return m_rightGatePos; }

		void setBall(Ball* ball) { m_ball = ball; }
		Ball* getBall() { return m_ball; }

	private:

		Ball* m_ball;
		
		TraceLine* m_traceBorderLines;
		TraceLine* m_halfLine;
		TraceLine* m_leftPenaltyLines;
		TraceLine* m_rightPenaltyLines;

		float m_fScaleLengthFactor;
		float m_fScaleWidthFactor;

		float m_fLength;
		float m_fWidth;


		CalVector m_corner_NW;
		CalVector m_corner_NE;
		CalVector m_corner_SW;
		CalVector m_corner_SE;

		CalVector m_leftGatePos;
		CalVector m_rightGatePos;

		//---------
		TraceLine* m_traceForward;

		

    };

}
#endif //_FT_FIELD_H
