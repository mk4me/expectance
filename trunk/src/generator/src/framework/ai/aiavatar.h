/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_AI_AVATAR_H
#define _GEN_AI_AVATAR_H

#include "../action/actionavatar.h"

namespace ft
{
	/**
	 * Class AIAvatar: this class represents AI part in avatar stack 
     * 
     **/
    class AIAvatar : public ActionAvatar                                 
    {
    public:
        AIAvatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName);
        virtual ~AIAvatar(void);
        virtual void Destroy(void);

        void Init();

        virtual void Reset();
    };
};

#endif //_GEN_AI_AVATAR_H