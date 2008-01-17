#include "cdmanager.h"
#include "scene/scenemanager.h"

using namespace ft;

CollisionDetectionManager* CollisionDetectionManager::m_instance = NULL;

CollisionDetectionManager* CollisionDetectionManager::getInstance()
{
    if (m_instance == NULL)
    {
		_dbg << "CollisionDetectionManager::getInstace(): instance of CollisionDetectionManager created " << std::endl;
        m_instance = new CollisionDetectionManager();
    }

    return m_instance;
}

CollisionDetectionManager* CollisionDetectionManager::createInstanceFirstTime()
{
    if (m_instance == NULL)
    {
        m_instance = new CollisionDetectionManager();
    }
    else
    {
        if (Debug::ERR)
			_dbg << "CollisionDetectionManager::createInstanceFirstTime(): instance alraedy created!!!! " << std::endl;
    }
    return m_instance;
}

void CollisionDetectionManager::DestroyInstance()
{
    if (m_instance != NULL)
    {
  //  	std::map<std::string,SceneObject*>::iterator it = m_instance->m_SceneObjectsMap.begin();
		//for( ; it != m_instance->m_SceneObjectsMap.end(); ++it ) {
		//	delete it->second;
	 //   }
	 //   m_instance->m_SceneObjectsMap.clear();

        delete m_instance;
    }
}

void CollisionDetectionManager::OnUpdate(const double elapsedSeconds)  // OVERRIDEN, updates by UpdateManager 
{
	UpdateDistanceMatrix();
}

void CollisionDetectionManager::UpdateDistanceMatrix()
{
	unsigned long _sgs = SceneManager::getInstance()->getSceneGraph().size();
	unsigned long _dms = m_distanceMatrix.size();
	if (_dms < _sgs)
	{
			m_distanceMatrix.resize(_sgs,_sgs);
			m_directionMatrix.resize(_sgs,_sgs);
			_dms = m_distanceMatrix.size(); // new size
	}
	else if (_dms == _sgs)
	{
		//std::cout << "updating ....." << std::endl;
		//calculate distances between each pair of dynamic object
			for(unsigned long i = 0; i < _sgs; i++)
			{
				for (unsigned long j = i+1; j< _sgs; j++)
				{
					//count direction vector (from j-th point to i-th point) to the center
					m_directionMatrix[i][j] =  SceneManager::getInstance()->getSceneGraph()[i]->getPosition() 
						- SceneManager::getInstance()->getSceneGraph()[j]->getPosition();
					m_directionMatrix[i][j].y = 0;
					m_directionMatrix[j][i] = m_directionMatrix[i][j]*(-1);

					// compute distance
					double _dx = m_directionMatrix[i][j].x*m_directionMatrix[i][j].x;
					double _dz = m_directionMatrix[i][j].z*m_directionMatrix[i][j].z;

					m_distanceMatrix[i][j] = sqrt(_dx + _dz);
					m_distanceMatrix[j][i] = m_distanceMatrix[i][j];

					//calculate distance for given set of constraints
					// todo mka - cumulative normalized here or in magnet controller (on the base of dynamicID
				}

			}

	}
}

const CalVector CollisionDetectionManager::getObjectCummulativeForce(const unsigned int id, const double threshold)
{
	CalVector _cumForce; // cummulative force
	CalVector _tmp;
	double _mdm;
	unsigned long _dms = m_distanceMatrix.size();

	if (id < _dms)
	{
		for(unsigned long j = 0; j < _dms; j++)
		{
			_mdm = m_distanceMatrix[id][j];
			if ( _mdm < threshold )
			{
				if (_mdm < 0.0000001) _mdm = 1;
				_tmp = m_directionMatrix[id][j]/(_mdm*_mdm); //normalization
				_tmp *= MAGNETCONTROLLER_CONST; // const force component value addition
				_cumForce += _tmp;
			}
		}
		_cumForce.y = 0; 
	}
	return _cumForce;

}