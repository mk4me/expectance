/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */


#pragma once
#ifndef _GEN_STRING_HELPER_H
#define _GEN_STRING_HELPER_H


#include <vector>
#include <string>
#include <iostream>


using namespace std;

namespace ft 
{
	class StringHelper
	{
		public:
		static vector<string> Tokens(const string& str,const string& delimiters);
		static string ClearBrakets(const string& str, const char start_bracket, const char end_bracket);
		static string RemoveChar(const string& str, const char character);
	};







};

#endif // _GEN_STRING_HELPER_H