/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_PHYSICS_AVATAR_H
#define _GEN_PHYSICS_AVATAR_H

#include "../motion/movableavatar.h"
#include "speedcontroller.h"
#include "magnetcontroller.h"

namespace ft
{
	/**
	 * Class PhysicsAvatar: this class is to handle physical aspects of avatar
     * 
     **/
    class PhysicsAvatar : public MovableAvatar                                 
    {
    public:
        PhysicsAvatar(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
        virtual ~PhysicsAvatar(void);
        virtual void Destroy(void);

        void Init();

        virtual void Reset();

        void setSpeedController(SpeedController* ctl) { m_speedController = ctl; }
        SpeedController* getSpeedController() { return m_speedController; }

		void setMagnetController(MagnetController* ctl) { m_magnetController = ctl; }
        MagnetController* getMagnetController() { return m_magnetController; }

    protected:
        virtual TimeLine* InitTimeLine();
        void InitSpeedFactor();

        SpeedController* m_speedController;
		MagnetController* m_magnetController;

    };
}

#endif //_GEN_PHYSICS_AVATAR_H
