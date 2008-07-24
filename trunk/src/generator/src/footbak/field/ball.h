/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_BALL_H
#define _FT_BALL_H

#include "scene/cal3dobject.h"
#include "scene/traceline.h"

namespace ft
{
	/**
	 * Defines ball behaviour
	 **/
	class Ball: public Cal3DObject
    {
	public:
		Ball(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
		~Ball(void);
		virtual void OnUpdate(const double elapsedTime);

		void setRestY(float restY) { m_restY = restY; }
		float getRestY() { return m_restY; }

		float CalculateSpeed(const CalVector& dest, float time);
		void Move(const CalVector& dest, float speed, float speedUp);
		void MoveByLob(const CalVector& dest, float speed);

		void setSpeed(float speed) { m_speed = speed; }
		void setUpSpeed(float speed) { m_upSpeed = speed; }
		void setDestPoint(const CalVector& dest);
		void setFriction(float friction) { m_friction = friction; }

	private:
		bool isOnTheGround();
		void CheckCollWithGround();
		void CheckIfDestReached();
		void CheckBounce();

		float m_restY;
			
		float m_speed;
		float m_upSpeed;

		CalVector m_vCurrDir;
		CalVector m_vDestPoint;

		float m_gravity;
		float m_vertAccel;
		float m_upAccel;
		float m_friction;
		float m_bounceThresholdSpeed;

		bool m_bGoalReached;

		CalVector m_vLastPos;

		TraceLine* m_traceDest;
    };

}
#endif //_FT_BALL_H
