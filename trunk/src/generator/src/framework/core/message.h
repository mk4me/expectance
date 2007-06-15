/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MESSAGE_H
#define _GEN_MESSAGE_H


#include <string>

namespace ft
{
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
        Message(const int type, const MessageParam& param = NULL) { /*empty*/}
    };
};

#endif //_GEN_MESSAGE_H
