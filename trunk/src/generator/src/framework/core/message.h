/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MESSAGE_H
#define _GEN_MESSAGE_H

#include <string>


namespace ft
{
    //TYPES of MEDDAGES
    static const int MSG_CONTROL_PAUSE = 0;  //param: NULL
    static const int MSG_CONTROL_START = 1;  //param: NULL
    static const int MSG_CONTROL_STOP = 2;  //param: NULL

    static const int MSG_PROPERTY_LOD = 3;  //param: float
    static const int MSG_PROPERTY_RENDER_METHOD = 4;  //param: NULL  , changes render method into next one



    ///////////////////////////////

    class MessageParam
    {
    public:
        MessageParam(int value) { m_nValue = value; }
        MessageParam(float value) { m_fValue = value; }
        MessageParam(std::string & value) { m_strValue = value; }
        MessageParam(void * value) { m_ptrValue = value; }

        int getIntValue() { return m_nValue; }
        float getFloatValue() { return m_fValue; }
        std::string getStrValue() { return m_strValue; }
        void* getObjecttValue() { return m_ptrValue; }
        
    private:
        int m_nValue;
        float m_fValue;
        std::string m_strValue;
        void* m_ptrValue;
    };

    ///////////////////////////////

    /* Base class for message */
          
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
