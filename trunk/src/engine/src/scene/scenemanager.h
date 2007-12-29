/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_SCENEMANAGER_H
#define _GEN_SCENEMANAGER_H


#include "sceneobject.h"
#include "../core/updatemanager.h" //temporary before collision separation





namespace ft
{
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
	  
	  // other ctors ....

	  inline std::vector<T> & operator[](int i) { return _data[i]; }

	  inline const std::vector<T> & operator[] (int i) const { return _data[i]; }

	  // other accessors, like at() ...

	  void resize(int rows, int cols)
	  {
		_data.resize(rows);
		for(int i = 0; i < rows; ++i)
		  _data[i].resize(cols);
	  }
	  
	  void push_back(const double value)
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
	
	//! A SceneManager class
	/*!
	 *	This class is responsible for registering, releasing and communication with the rest parts of the system in the scope of sceneobjects. 
	 *  It creates visual objects collection. Registers all requiring objects to scene.  
	 */ 
	class SceneManager : public UpdateObject
    {
    public:
		SceneManager (void) { }//m_distanceMatrix.reserve(100,100); } //reserve one continous memory block 
	    virtual ~SceneManager (void) { /*empty*/ }
        //! singleton - Returns the only instance of SceneManager
		/*! Creates the only instance of SceneManager */
        static SceneManager * getInstance();
        //! it cerates only instance when called first time
        static SceneManager* createInstanceFirstTime();
        //! destroy all resources owned by SceneManager
        static void DestroyInstance();

		//! register SceneObject
		/*! Registers object to be managed by SceneManager */
		virtual bool AddObject(SceneObject* pObj);   

		/*! Registers data object to be rendered */
		bool AddDataObject(SceneObject* pObj);   

		//! get the pointer value of SceneObject by unique id
		SceneObject* getObject(std::string id);
		//! get the pointer value of SceneObject by name
		SceneObject* getObjectByName(std::string name);
		//! remove SceneObject
		/*! Unregisters SceneObject from SceneManager queue and call their destructor */
		bool RemoveObject(SceneObject*);
		//! unregister SceneObject and destroy it
		bool RemoveObject(std::string id);
		
		/*! Unregisters DataObject from Rendering queue and call their destructor */
		bool RemoveDataObject(SceneObject*);
		//! unregister DataObject and destroy it
		bool RemoveDataObject(std::string id);

        void OnUpdate(const double elapsedSeconds);  // OVERRIDEN, updates by UpdateManager 
		std::map<std::string,SceneObject*>& getSceneObjectsMap(void){ return m_SceneObjectsMap;}; //tmp
			
		//DoubleDynamicArray2D DistanceMtx(){ return m_distanceMatrix;}; 
		DoubleDynamicArray2D m_distanceMatrix;
    protected:
		
        static SceneManager* m_instance;
		std::map<std::string,SceneObject*> m_SceneObjectsMap;
		std::vector<SceneObject*> m_SceneGraph; // for all dynamic objects
	private:
		void UpdateDistanceMatrix();
    };


}

#endif //_GEN_SCENEMANAGER_H