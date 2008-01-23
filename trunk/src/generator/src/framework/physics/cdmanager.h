/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#ifndef _GEN_CDMANAGER_H
#define _GEN_CDMANAGER_H

#include "core/updateobject.h"
#include "utility/stlhelper.h"

namespace ft
{
	static const int MAGNETCONTROLLER_CONST = 200;	

	class CollisionDetectionManager : public UpdateObject
	{
	public:
		CollisionDetectionManager(void) {/*empty*/};
		virtual ~CollisionDetectionManager(void) {/*empty*/};
        //! singleton - Returns the only instance of CollisionDetectionManager
		/*! Creates the only instance of CollisionDetectionManager */
        static CollisionDetectionManager * getInstance();
        //! it cerates only instance when called first time
        static CollisionDetectionManager* createInstanceFirstTime();
        //! destroy all resources owned by CollisionDetectionManager
        static void DestroyInstance();

		void OnUpdate(const double elapsedSeconds);  // OVERRIDEN, updates by UpdateManager
		//brief gets cummulative force vector of object given by id
		const CalVector getObjectCummulativeForce(const unsigned int id, const double threshold);

	protected:
		static CollisionDetectionManager* m_instance;


	private:
		//brief updates distances between all dynamic objects
		void UpdateDistanceMatrix();
		DoubleDynamicArray2D m_distanceMatrix;
		DynamicArray2D<CalVector> m_directionMatrix;
	};

}
#endif //_GEN_CDMANAGER_H