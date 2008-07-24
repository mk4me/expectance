/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_PLAYER_H
#define _FT_PLAYER_H

#include "../framework/avatar/calavatar.h"

namespace ft
{
	/** 
	 * Specific type of CalAvatar
	 */
	class Player : public CalAvatar
    {
	public:
		Player(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
	    virtual ~Player(void);		

		void setRestPos(const CalVector& pos) { m_restPosition = pos; }
		const CalVector& getRestPos() { return m_restPosition; }

	private:
		CalVector m_restPosition;
    };
}
#endif //_FT_PLAYER_H