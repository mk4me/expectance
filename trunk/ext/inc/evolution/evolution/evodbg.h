/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_EVODBG_H
#define _FT_EVODBG_H

#include <fstream>
#include <iostream>

//#define _DEBUGS_TO_FILE   //uncomment it to write logs into a file

#ifdef _DEBUGS_TO_FILE
    #define _dbg Debug::FILE_OUT
#else
    #define _dbg std::cout
#endif

#include "../platform.h"

namespace ft
{
	//! A Debug class
	/*!
	 *	Provides a set of methods and properties that help debug your code. 
	 */
    class EVOLUTION_API EvoDBG
    {
    public:

		/**
	     * \brief Sets level for debugs related to timeline execution
		 * \param int level - level of debugs
		 */
		static void setTimelineLevel(int level) { TIMELINE = level; }

		/**
	     * \brief Sets level for debugs related to motions management
		 * \param int level - level of debugs
		 */
		static void setMotionLevel(int level) { MOTION = level; }

		/**
	     * \brief Sets level for debugs related to actions flow
		 * \param int level - level of debugs
		 */
		static void setActionLevel(int level) { ACTION = level; }

		/**
	     * \brief Sets level for debugs related to AI
		 * \param int level - level of debugs
		 */
		static void setAILevel(int level) { AI = level; }


		/**
	     * \brief Level of debugs related to timeline execution
		 */
        static int TIMELINE;

		/**
	     * \brief Level of debugs related to motions management
		 */
        static int MOTION;

		/**
	     * \brief Level of debugs related to actions flow
		 */
        static int ACTION;

		/**
	     * \brief Level of debugs related to AI
		 */
        static int AI;
		
		//--------------

		/**
	     * \brief Flag defining if debugs are switched ON/OFF
		 */
        static bool ON;
		/**
	     * \brief Flag defining if warnings should be dumped
		 */
        static bool WARN;
		/**
	     * \brief Flag defining if errors should be dumped
		 */
        static bool ERR;

		/**
	     * \brief Stream defing file to which debugs are written
		 */
        static std::ofstream FILE_OUT;

		/**
	     * \brief String used for marking errors
		 */
        static std::string ERR_STR;

		/**
	     * \brief String used for marking warnings
		 */
        static std::string WARN_STR;
    };
}

#endif //_FT_EVODBG_H
