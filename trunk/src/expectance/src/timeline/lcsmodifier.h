/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _LCSMODIFIER_H
#define _LCSMODIFIER_H

#include "evolution/controller.h"
#include "evolution/timelinecontext.h"
#include "../avatar/osgavatar.h"
#include "../motion/transform.h"
#include "../scene/object/traceline.h"


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

        bool LOCAL_DEBUG;

        bool INTERPOLATION;
        bool REST_TRANS_CALC;  //if rest of translation lost at anim changes should be applied


        void Apply(float elapsedSeconds, TimeLineContext * timeLineContext);
        void UpdateRotation(float elapsedSeconds, TimeLineContext * timeLineContext);
        void UpdateTranslation__old(float elapsedSeconds, TimeLineContext * timeLineContext);

		void UpdateTranslation(float elapsedSeconds, TimeLineContext * timeLineContext);

        //virtual void Reset(TimeLineContext * timeLineContext); // resets current object and its children

        //virtual std::string toString(); //OVERRIDEN
	private:
        CalVector m_vLastPos;
        CalVector m_vLastPrevPos;  //used only for OVERLAP state
		float m_fLastAnimTime;
		osg::ref_ptr<ft::TraceLine> m_tracer;
//        CalVector m_vLastPos;
		Transform* GetTransformForAnim(CalAnimation* anim, OsgAvatar* avatar);
    };
}


#endif //_LCSMODIFIER_H
