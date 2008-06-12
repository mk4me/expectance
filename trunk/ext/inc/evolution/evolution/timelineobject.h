/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_TIMELINE_OBJECT_H
#define _GEN_TIMELINE_OBJECT_H

#include "entity.h"
#include "timelinecontext.h"
#include <string>
#include <vector>

namespace ft
{
    static const int TIME_UNDEFINED = -1;

    static const int ADD_OBJECT_AS_LAST = 0;

	/**
	 * Class TimeLineObject: base object for all objects that can be part of TimeLine
	 **/
    class EVOLUTION_API TimeLineObject : public Entity
    {
    public:
		/**
	     * \brief Default constructor
		 */
        TimeLineObject();
		/**
	     * \brief Default destructor
		 */
        virtual ~TimeLineObject(void) { /* empty */ }

		/**
	     * \brief Release all resources related to this object
		 * This callback can be overriden in deriven specific timeline object but it is recommended to 
		 * call from overriden method the base method form ft::TimeLineObject class.
         **/
        virtual void Destroy(void);

		/**
	     * \brief Add sub-object to this timeline object
		 * \param TimeLineObject* object - object to add
		 * \param int where_to_add - defines rule pointing where to add the object in the list of sub-objects
		 */
        virtual bool AddSubObject(TimeLineObject* object, int where_to_add = ADD_OBJECT_AS_LAST);

		/**
	     * \brief Removes sub-object from this timeline object
		 * \param TimeLineObject* obj_to_delete - object to delete
		 * \param bool deleteFromMemory - defines if object should be deleted from memory after removal from list of sub-objects
		 */
        virtual void RemoveSubObject(TimeLineObject* obj_to_delete, bool deleteFromMemory = false);

		/**
	     * \brief Gets last objects from sub-object list
		 * \return TimeLineObject* - last object 
		 */
        TimeLineObject* GetLastSubObject();

		/**
	     * \brief Informs if list of sub-objects is empty
		 * \return bool - true if sub-objects list is empty, false otherwise
		 */
        bool isEmpty() { return (m_first == NULL); }

		/**
	     * \brief Dumps all sub-objects
		 */
        void DumpSubObjects(int depth);

		/**
	     * \brief Returns object that is succesor of this object on the list of its parent
		 * \return TimeLineObject* - next object
		 */
        TimeLineObject* getNextObject() { return m_next; }

		/**
	     * \brief Sets object that is succesor of this object on the list of its parent
		 * \param TimeLineObject* obj - next object
		 */
        void setNextObject(TimeLineObject* obj) { m_next = obj; }

		/**
	     * \brief Returns first sub-object object on the list of sub-objects
		 * \return TimeLineObject* - first sub-object
		 */
        TimeLineObject* getFirstObject() { return m_first; }
		/**
	     * \brief Sets first sub-object object on the list of sub-objects
		 * \param TimeLineObject* obj - first sub-object
		 */
        void setFirstObject(TimeLineObject* obj) { m_first = obj; }

		/**
	     * \brief Returns parent of this object
		 * \return TimeLineObject* - parent of object
		 */
        TimeLineObject* getParent() { return m_parent; }
		/**
	     * \brief Sets parent of this object
		 * \return TimeLineObject* obj - parent of object
		 */
        void setParent(TimeLineObject* obj) { m_parent = obj; }

		/**
	     * \brief Callback called to request object to reset its state
		 * This callback is intended be overriden in deriven specific timeline object but it is recommended to 
		 * call from overriden method the base method form ft::TimeLineObject class.
         * \param TimeLineContext* timeLineContext - context of timeline at given frame of simulation
         **/
        virtual void Reset(TimeLineContext* timeLineContext); // resets current object and its children

		/**
	     * \brief Returns string that corresponds  indentation in dumping text
		 * \param std::string  - indentation string
		 */
        std::string getDepthStr(int depth);

		/**
	     * \brief Dumps current state of motion.
		 * \param int depth - indentation in dumping text
		 */
        virtual void Dump(int depth);

		/**
	     * \brief Creates new timeline object which can be used for clonning of this object
         * \return TimeLineObject* - new timeline object
         **/
        virtual TimeLineObject* CreateInstance();

		/**
	     * \brief Clones this object
         * \return TimeLineObject* - new timeline object tha is clone of this one
         **/
        virtual TimeLineObject* Clone();
        

    protected:
        //list
        TimeLineObject* m_first;

        TimeLineObject* m_next;

        TimeLineObject* m_parent;
    };
}


#endif //_GEN_TIMELINE_OBJECT_H
