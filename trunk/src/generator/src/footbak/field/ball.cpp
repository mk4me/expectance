/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "ball.h"
#include "scene/scenemanager.h"
#include "utility/vishelper.h"
#include "scene/transformmanager.h"

using namespace ft;

Ball::Ball(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName)
:Cal3DObject(calModel, calCoreModel, modelName)
{
	setRestY(20);

	m_speed = 0.0f;
	m_upSpeed = 0.0f;
	
	m_vDestPoint = CalVector();
	m_vCurrDir = CalVector();
	m_vLastPos = CalVector();

	m_gravity = 980;
	m_vertAccel = 0.0f;
	m_upAccel = 0.0f;
	m_friction = 600;
	m_bounceThresholdSpeed = -20;
	m_bGoalReached = false;

	m_traceDest = new TraceLine("Ball-m_traceDest");
	SceneManager::getInstance()->AddObject(m_traceDest);
	m_traceDest->setColor(VisualizationHelper::COLOR_YELLOW);
	m_traceDest->setBlending(false); //m_traceDest->HideMarker();

}

Ball::~Ball(void)
{
}

float Ball::CalculateSpeed(const CalVector& dest, float time)
{
	CalVector dist = getPosition() - dest;
	float speed = dist.length()/time;
	return speed;
}
/**
 * Move ball in the air to destination
 */
void Ball::MoveByLob(const CalVector& dest, float speed)
{
	CalVector dist = getPosition() - dest;
	float time = dist.length() / speed;
	float speedUp = (time/2.0f)*m_gravity;
	Move(dest, speed, speedUp);
}

void Ball::Move(const CalVector& dest, float speed, float speedUp)
{
	setDestPoint(dest);
	m_vCurrDir = dest - getPosition();
	m_vCurrDir.normalize();

	CalVector pos = getPosition();
	setPosition(CalVector(pos.x, getRestY(), pos.z));
	m_bGoalReached = false;

	setSpeed(speed);
	setUpSpeed(speedUp);
}

void Ball::setDestPoint(const CalVector& dest)
{ 
	m_vDestPoint = dest;
	
	m_traceDest->AddPoint(m_vDestPoint);
	CalVector up(m_vDestPoint);
	up += CalVector(0,100,0);
	m_traceDest->AddPoint(up);
}

void Ball::OnUpdate(const double elapsedTime)
{
	if (m_speed > 0 || m_upSpeed > 0)
	{
		bool onTheGround = isOnTheGround();

		//apply friction 
		if (m_bGoalReached)
		{
			if (onTheGround)
			{
				m_speed -= m_friction * elapsedTime;
				if (m_speed < 0)
				{
					m_speed = 0;
				}
			}
		}

		//apply gravity	
		if (!onTheGround)
		{
			m_upSpeed -= m_gravity*elapsedTime;
		}

		// calculate move vactor on basis of of speed and speed up 
		CalVector vMove(m_vCurrDir * (m_speed*elapsedTime) );
		CalVector vMoveUp(TransformManager::SCENE_UP * (m_upSpeed*elapsedTime));
		vMove += vMoveUp;

		// set new position
		CalVector pos = getPosition();
		m_vLastPos = pos;
		CalVector newPos = getPosition() + (vMove);
		setPosition(newPos);

		CheckCollWithGround();
		CheckIfDestReached();
		CheckBounce();
	}

	Cal3DObject::OnUpdate(elapsedTime);
}

bool Ball::isOnTheGround()
{
	return getPosition().y <= getRestY();
}

void Ball::CheckCollWithGround()
{
	CalVector currPos(getPosition());
	if (currPos.y < getRestY())
	{
		currPos.y = getRestY();
		setPosition(currPos);
	}
}

void Ball::CheckIfDestReached()
{
	CalVector pos = getPosition();
	pos.y = 0;

	CalVector vDiff = pos - m_vDestPoint;

	if (vDiff.length() < 10)
	{
		m_bGoalReached = true;
	}
	else
	{
		float lastGoalDist = (m_vLastPos - m_vDestPoint).length();
		float currGoalDist = (pos - m_vDestPoint).length();
		if ( (lastGoalDist - currGoalDist) < 0 )
		{
			// there is no chance to reach the goal
			m_bGoalReached = true;
		}
	}
}

void Ball::CheckBounce()
{
	if (isOnTheGround())
	{
		if (m_upSpeed < m_bounceThresholdSpeed)
		{
			setUpSpeed(-m_upSpeed/3);
			if (m_bGoalReached)
			{
				setSpeed(m_speed*0.7f);
			}
		}
		else
		{
			m_upSpeed = 0;
		}
	}
}
