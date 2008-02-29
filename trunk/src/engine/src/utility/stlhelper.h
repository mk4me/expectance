/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_STLHELPER_H
#define _GEN_STLHELPER_H

#include <vector>
#include <map>
#include "../core/platform.h"

namespace ft
{
	//! Wrapper for map<string,T>
	/*!
	 *	This struct is useful when one wants to export STL collection to DLL without warnings
	 */ 
    template <typename T1, typename T2> struct STL_WrappedMap
    {
         std::map<T1,T2> map;
    };

	//! Wrapper for vector<T>
	/*!
	 *	This struct is useful when one wants to export STL collection to DLL without warnings
	 */ 
    template <typename T> struct STL_WrappedVector
    {
        std::vector<T> vector;
    };

    
    //StringMap_t<std::string> StringMap;

	//! A Dynamic Array class
	/*!
	 *	This class is responsible creating, releasing and operating on dynamic array based on stl vector. 
	 */ 

	template <typename T>
	class DynamicArray2D
	{
	public:
	  DynamicArray2D(){};
	  DynamicArray2D(int rows, int cols)
	  {
		for(int i=0; i<rows; ++i)
		{
		  _data.push_back(std::vector<T>(cols));
		}
	  }
	  

	  inline std::vector<T> & operator[](int i) { return _data[i]; }

	  inline const std::vector<T> & operator[] (int i) const { return _data[i]; }
	  
	  inline unsigned long size(){ return _data.size(); }
	  // other accessors, like at() ...

	  void resize(int rows, int cols)
	  {
		_data.resize(rows);
		for(int i = 0; i < rows; ++i)
		  _data[i].resize(cols);
	  }
	  
	  void add(const double value)
	  {
		_data.push_back(value);
		for(int i = 0; i < _data.size(); ++i)
		  _data[i].push_back(value); //for each column
	  }
	
	  void erase (int index)
	  {
		_data.erase(_data.begin()+index);
		for(int i = 0; i < _data.size(); ++i)
		  _data[i].erase(_data.begin()+index); //for each column	
	  }
	  // other member functions, like reserve()....

	private:
	  std::vector<std::vector<T> > _data;  
	};

	typedef DynamicArray2D <double> DoubleDynamicArray2D;

}
#endif //_GEN_STLHELPER_H
