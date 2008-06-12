/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_ACTION_H
#define _FT_ACTION_H

#include "avatar.h"

namespace ft
{
	/**
	 * Base class for actions that avatar can perform
	 **/
    class EVOLUTION_API Action
    {
    public:
		/**
		 * Identifier for 'Idle' action
		 **/
		static const int ACTION_IDLE_ID = 0; 
		/**
		 * Identifier for 'Walk' action
		 **/
		static const int ACTION_WALK_ID = 2; 
		/**
		 * Identifier for 'Run' action
		 **/
		static const int ACTION_RUN_ID = 1; 

		/**
	     * \brief Constructor
         * \param int action_id - action identifier
         **/
        Action(int action_id) { m_action_id = action_id; }

		/**
	     * \brief Default destructor
         **/
        virtual ~Action(void) { /* empty */ }

		/**
	     * \brief Returns identifier of this action object
         * \return int - action identifier
         **/
        int getId() { return m_action_id; }

		/**
	     * \brief Inits action object.
		 * This callback can be overriden in deriven specific action  but it is recommended to 
		 * call from overriden method the base method form ft::Action class.
         * \param Avatar* avatar - avatar on which this action is executed
         **/
        virtual void Init(Avatar* avatar);

		/**
	     * \brief Callback called on the beginning of the action execution
		 * This callback is intended be overriden in deriven specific action but it is recommended to 
		 * call from overriden method the base method form ft::Action class.
         * \param Avatar* avatar - avatar on which this action is executed
         **/
        virtual void Entry(Avatar* avatar, Action* oldAction);

		/**
	     * \brief Callback called at the end of the action execution
		 * This callback is intended be overriden in deriven specific action but it is recommended to 
		 * call from overriden method the base method form ft::Action class.
         * \param Avatar* avatar - avatar on which this action is executed
         **/
        virtual void Exit(Avatar* avatar, Action* newAction);

		/**
	     * \brief Callback called to request the action to reset its state
		 * This callback is intended be overriden in deriven specific action but it is recommended to 
		 * call from overriden method the base method form ft::Action class.
         * \param Avatar* avatar - avatar on which this action is executed
         **/
        virtual void Reset(Avatar* avatar);

		/**
	     * \brief Callback called to request the action to reset its state.
		 * This callback can be overriden in deriven specific action but it is recommended to 
		 * call from overriden method the base method form ft::Action class.
         * \param Avatar* avatar - avatar on which this action is executed
         **/
        virtual std::string toString();

    private:
        int m_action_id;
    };

}
#endif //_FT_ACTION_H
