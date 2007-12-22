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
#include <sstream>


using namespace std;

namespace ft 
{

	/**
	 * Class StringHelper:
	 * Utilities for String formatting, manipulation, and queries
	 **/
	class StringHelper
	{
		public:
		static vector<string> Split(const string& str,const string& delimiters);
		static string ClearDelimiters(const string& str, const char start_bracket, const char end_bracket);
		static string RemoveChar(const string& str, const char character);
        static bool ReadFloatArrayFromString(const string& str, float* floatArray, int arraySize);
		template <typename T> static T FromStringEx( const std::string& s, bool& bSuccess);
		static string itos(int i);
	};
}

#endif // _GEN_STRING_HELPER_H