/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _LCSMODIFIER_H
#define _LCSMODIFIER_H

#include "evolution/controller.h"
#include "evolution/timelinecontext.h"



namespace ft
{
	/**
	 * Class LCSModifier: specific TimeLineModifier responsible for global translation of avatar on the scene
	 **/
	class LCSModifier : public Controller
    {
    public:
        LCSModifier();
        virtual ~LCSModifier(void);

        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);
        void UpdateRotation(float elapsedSeconds, TimeLineContext * timeLineContext);
        void UpdateTranslation(float elapsedSeconds, TimeLineContext * timeLineContext);

        //virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children

        //virtual std::string toString(); //OVERRIDEN
    };
}


#endif //_LCSMODIFIER_H
