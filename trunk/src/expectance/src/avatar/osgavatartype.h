/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_OSGAVATARTYPE_H
#define _GEN_OSGAVATARTYPE_H

#include <osgCal/CoreModel>
#include "../motion/animation.h"



namespace ft
{
	/* 
     * An OsgAvatarType represetns Cal3d core model and all data related to OSG
	 */
	class OsgAvatarType
    {
    public:
		
		OsgAvatarType(const std::string &typeName, osgCal::CoreModel* coreModel);
	    virtual ~OsgAvatarType(void);

		osgCal::CoreModel* getCoreModel() { return m_osgCoreModel; }

        bool AddMotion(Animation* motion);
        bool  RemoveMotion(Animation* motion);
        Animation* GetMotion(std::string motionName);
        std::map<std::string,Animation*>& getMotionsMap() { return m_motions; }
        void InitMotions();

        virtual void InitTransforms();

        //virtual void Dump();
    
    private:
		osgCal::CoreModel* m_osgCoreModel;
        std::map<std::string,Animation*> m_motions;
       
    };
}

#endif //_GEN_OSGAVATARTYPE_H
