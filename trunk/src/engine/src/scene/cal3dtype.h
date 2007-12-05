/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _SCENE_CAL3DTYPE_H
#define _SCENE_CAL3DTYPE_H

#include "transform.h"


namespace ft
{
	/* 
     * An Cal3dType class is extension of CalCoreModel for Cal3d objects
	 */
    class Cal3dType : public CalCoreModel
    {
    public:
		Cal3dType(const std::string &typeName);
	    virtual ~Cal3dType(void);

        void setTransform(Transform* transform) { m_transform = transform; }
        Transform* getTransform() { return m_transform; }

        virtual void InitTransform(bool source_3dsmax);

        virtual void Dump();
    
    private:
        Transform* m_transform;
      
    };
};

#endif //_SCENE_CAL3DTYPE_H
