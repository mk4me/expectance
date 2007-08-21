/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MESSAGE_H
#define _GEN_MESSAGE_H

#include <string>


namespace ft
{
    //TYPES of MESSAGES
    static const int MSG_TEST = -1; // only for test
    static const int MSG_CONTROL_PAUSE = 0;  //param: NULL
    static const int MSG_CONTROL_START = 1;  //param: NULL
    static const int MSG_CONTROL_STOP = 2;  //param: NULL
    static const int MSG_CONTROL_TURN_LEFT = 3;  //param: NULL
    static const int MSG_CONTROL_TURN_RIGHT = 4;  //param: NULL


    static const int MSG_PROPERTY_LOD = 5;  //param: float
    static const int MSG_PROPERTY_RENDER_METHOD = 6;  //param: NULL  , changes render method into next one
	static const int MSG_PROPERTY_SHADOW = 7; //param bool

    //
 	  static const int MSG_MENU_ITEM_SELECTED = 8;
	//static const int MSG_INPUT_KEY_PRESSED = 9;
	//static const int MSG_INPUT_BUTTON_PRESSED = 10;
	//static const int MSG_INPUT_BUTTON_RELEASED = 11;
	//static const int MSG_INPUT_MOUSE_MOVED = 12;




    ///////////////////////////////

	/**
	 * Class MessageParam: represents specific parameters of message
	 **/
    class MessageParam
    {
    public:
        MessageParam(int value) { m_nValue = value; }
        MessageParam(float value) { m_fValue = value; }
		MessageParam(const std::string value) { m_strValue = value; }
//        MessageParam(const std::string value) { m_strValue = value; }
//        MessageParam(void * value) { m_ptrValue = value; } //WARNING : this consructos is improperly called for string values 

        int getIntValue() { return m_nValue; }
        float getFloatValue() { return m_fValue; }
        std::string getStrValue() { return m_strValue; }
//        void* getObjecttValue() { return m_ptrValue; } 
        
    private:
        int m_nValue;
        float m_fValue;
        std::string m_strValue;
        void* m_ptrValue;
    };

    ///////////////////////////////

    /* Base class for message */
          
	/**
	 * represents messages sent by ft::UpdateManager to ft::UpdateObject -s
	 **/
    class Message
    {
    public:
        Message(const int type, MessageParam* param = NULL) { m_type = type; m_param = param; }

        int getType() { return m_type; }
        MessageParam* getParam() { return m_param; }

        static std::string _GET_MSG_NAME(int msg);
    private:
        int m_type;
        MessageParam* m_param;
    };
};

#endif //_GEN_MESSAGE_H
