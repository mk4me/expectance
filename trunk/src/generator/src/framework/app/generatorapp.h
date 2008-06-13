/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_GENERATORAPP_H
#define _GEN_GENERATORAPP_H

#include "core/application.h"
#include "../avatar/calavatar.h"
#include "platform.h"
#include "gamehandler.h"

namespace ft
{
	/**
	 * Class Application: is responsible for lifecycle of Generator application
	 **/
    class GENERATOR_API GeneratorApp : public Application, public UpdateObject
    {
    public:
		GeneratorApp(void);
		virtual ~GeneratorApp(void);
        virtual bool InitModules();
		virtual void InitScene();
        virtual bool CreateVisualizationManager();

		void printSomeText();  //to test python iface

		virtual void OnUpdate(const double elapsedTime);  // updates by UpdateManager

		void setGameHandler(GameHandler* handler) { m_gameHandler = handler; }
		GameHandler* getGameHandler() { return m_gameHandler; }

	private:
		GameHandler* m_gameHandler;
    };
}

#endif //_GEN_GENERATORAPP_H

