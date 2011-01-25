/*
 * Copyright (c) 2007-2008, FutureToday. All rights reserved.
 * author: mka
 */
#ifndef _FT_DATA_COLLECTOR
#define _FT_DATA_COLLECTOR

#include "../control/controlmanager.h"
#include "tinyxml/tinyxml.h"


namespace ft 
{
	class DataCollector
	{
	public:
		DataCollector(void){/* null */};
		virtual ~DataCollector(void){};
        
		//! load initial world parameters used in program //moze wystarczy zrobic kopie world do lokalnie przechowywanej struktury albo strukture ubozsza xml
		void Init();
		//! refresh parameters values inside the application
		void Refresh();

		/// XML part
		//! dumps data configuration to file
		void Dump2File();
		//! reads data configuration from file
		void ReadFromFile(const std::string& file);

    private:
		OsgAvatar* getAvatar(const std::string& name);
		
	};

}
#endif //_FT_DATA_COLLECTOR

