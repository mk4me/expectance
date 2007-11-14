/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_AVATAR_H
#define _GEN_AVATAR_H

#include "../scene/meshobject.h"

namespace ft
{
	/* An Avatar class 
	 * Base class for all humanoid characters managed by Generator
     * \param CalModel * calModel -- model in Cal3d which is referenced by this avatar
	 */
    class Avatar : public MeshObject
    {
    public:
		Avatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName);
	    virtual ~Avatar(void);
        virtual void Destroy(void);
    };
};

#endif //_GEN_AVATAR_H
