/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_AVATAR_H
#define _GEN_AVATAR_H

#include "scene/cal3dobject.h"
#include "core/msgsender.h"

namespace ft
{
	/* An Avatar class 
	 * Base class for all humanoid characters managed by Generator
     * \param CalModel * calModel -- model in Cal3d which is referenced by this avatar
	 */
    class Avatar : public Cal3DObject
    {
    public:
		Avatar(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
	    virtual ~Avatar(void);
        virtual void Destroy(void);

        MsgSender* getLocalMsgSender() { return m_localMsgSender; }

        virtual void Dump();


    private:
        MsgSender* m_localMsgSender;
    };
};

#endif //_GEN_AVATAR_H
