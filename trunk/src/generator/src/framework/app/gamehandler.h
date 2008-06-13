/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_GAMEHANDLER_H
#define _FT_GAMEHANDLER_H

namespace ft
{
	/**
	 * An object that is used to communicate with game based on generator
	 **/
	class GameHandler
    {
	public:
		virtual void InitScene() = 0;
		virtual void OnUpdate(const double elapsedTime) = 0;
    };

}
#endif //_FT_GAMEHANDLER_H
