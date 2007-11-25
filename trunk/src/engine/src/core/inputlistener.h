/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_INPUT_LISTENER_H
#define _GEN_INPUT_LISTENER_H

namespace ft
{
	/**
	 * Class InputListener: class to implement by listeners for input events
	 **/
    class InputListener
    {
    public:
        InputListener(void) { /*empty*/}
	    virtual ~InputListener(void) { /*empty*/}
        
        virtual void OnKey(unsigned char key, int x, int y) { /*empty*/ }
        virtual void OnSpecial(int key, int x, int y) { /*empty*/ }
        virtual void OnMouseButtonDown(int button, int x, int y) { /*empty*/ }
        virtual void OnMouseButtonUp(int button, int x, int y) { /*empty*/ }
        virtual void OnMouseMove(int x, int y) { /*empty*/ }
    };
};

#endif //_GEN_INPUT_LISTENER_H