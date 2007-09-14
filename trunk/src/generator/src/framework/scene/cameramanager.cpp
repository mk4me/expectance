/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "cameramanager.h"

using namespace ft;

CameraManager* CameraManager::m_instance = NULL;

CameraManager* CameraManager::getInstance()
{
    if (m_instance == NULL)
    {
        DBG("CameraManager::getInstace(): instance of CameraManager created ");
        m_instance = new CameraManager();
    }

    return m_instance;
}

void CameraManager::DestroyInstance()
{
    if (m_instance != NULL)
	{
    	std::map<std::string,Camera*>::iterator it = m_instance->m_cameraContainer.begin(); //clean camera list
		for( ; it != m_instance->m_cameraContainer.end(); ++it ) {
			delete it->second;
	    }
	    m_instance->m_cameraContainer.clear();
		m_instance->m_cameraIndexContainer.clear();
        delete m_instance;
	}
}

void CameraManager::Init()
{
	m_mouseX = 0;
	m_mouseY = 0;
	m_bLeftMouseButtonDown = false;
	m_bRightMouseButtonDown = false;
	m_bMiddleMouseButtonDown = false;

	AddCamera(new Camera("mainCamera"));	
	setCurrentCamera("mainCamera");	
}


void CameraManager::UpdateView()
{
	if(m_currentCamera!=NULL)
	{
		m_currentCamera->UpdateView();
	}
}

void CameraManager::OnUpdate(const double elapsedSeconds)  // OVERRIDEN, updates by UpdateManager 
{
	if(m_currentCamera!=NULL)
	{
		m_currentCamera->OnUpdate(elapsedSeconds);
	}
}

bool CameraManager::AddCamera(SceneObject *pScObj)
{
	std::string _id = pScObj->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,Camera*>::iterator it = m_cameraContainer.find(_id);
		if ( it!=m_cameraContainer.end()) { 
			return false;
		}
		else
		{
			Camera *_pCamObj = new Camera(pScObj);
			m_cameraContainer.insert( std::make_pair( std::string(_id), _pCamObj ) );
			m_cameraIndexContainer.push_back(std::string(_id)); //add index
		}
	}
	return true;
}

bool CameraManager::AddCamera(Camera *pCamObj)
{
	std::string _id = pCamObj->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,Camera*>::iterator it = m_cameraContainer.find(_id);
		if ( it!=m_cameraContainer.end()) { 
			return false;
		}
		else
		{
			m_cameraContainer.insert( std::make_pair( std::string(_id), pCamObj ) );
			m_cameraIndexContainer.push_back(std::string(_id)); //add index
		}
	}
	return true;
}

bool CameraManager::AddCamera(std::string camName, float pitch, float yaw, float roll, float dist, float leftRight, float upDown)
{
	Camera *cam = new Camera(camName);
	cam->Init(pitch,yaw,roll,dist,leftRight,upDown);
	return AddCamera(cam);
}

Camera* CameraManager::getCamera(std::string id)
{
 	std::map<std::string,Camera*>::iterator it = m_cameraContainer.find(id);
	if ( it!=m_cameraContainer.end()) { 
		return it->second;
	}
	return NULL;
}

void CameraManager::changeCurrentCamera(ft::Direction direction)
{
	if(direction == ft_Forward)
	{
		if (m_currentCameraIndex == m_instance->m_cameraContainer.size()-1)
			m_currentCameraIndex  = 0;
		else
			++m_currentCameraIndex;

	}
	else if (direction == ft_Backward)
	{
		if (m_currentCameraIndex == 0)
			m_currentCameraIndex  = m_instance->m_cameraContainer.size()-1;
		else
			--m_currentCameraIndex;
	}
	
	setCurrentCamera(m_cameraIndexContainer[m_currentCameraIndex]);
}

void CameraManager::setCurrentCamera(std::string id)
{
	Camera *cam = getCamera(id);
	if (cam!=NULL)
	{
		m_pitchAngle = cam->getPitchAngle();
		m_yawAngle = cam->getYawAngle();
		m_distance = cam->getDistance();
		m_camUpDown = cam->getCamUpDown();
		m_camLeftRight = cam->getCamLeftRight();

		m_currentCamera = cam;
		setCurrentCameraIndex(id);
	}
	else
	{
		m_pitchAngle = 20.0f;
		m_yawAngle = 0.0f;
		m_rollAngle = 0.0f;
		m_distance = 800.0f;
		m_camUpDown = 0.0f;
		m_camLeftRight = 0.0f;
		if ((m_currentCamera = getCamera("mainCamera")) == NULL) 
			AddCamera(new Camera("mainCamera"));
		m_currentCamera = new Camera("mainCamera");
		setCurrentCameraIndex("mainCamera");
	}
}


bool CameraManager::RemoveCamera(ft::Camera *pObj)
{
	std::string _id = pObj->getID();
	bool done;
	if (_id=="mainCamera")
		return false;
	done = RemoveCamera(_id);
	return done;
}

bool CameraManager::RemoveCamera(std::string id)
{
	if (!id.empty())
	{
	 	std::map<std::string,Camera*>::iterator it = m_cameraContainer.find(id);
		if ( it!=m_cameraContainer.end()) 
		{ 
			delete it->second;
			m_cameraContainer.erase(it);
			RemoveCameraIndex(id);
			return true;
		}
	}
	return false;
}

int CameraManager::getCurrentCameraIndex()
{
	return m_currentCameraIndex;	
}

void CameraManager::setCurrentCameraIndex(const std::string id)
{	
	std::vector< std::string >::iterator location;
	location = std::find( m_cameraIndexContainer.begin(), m_cameraIndexContainer.end(), id );
	if ( location != m_cameraIndexContainer.end() ) // founded
		m_currentCameraIndex = location - m_cameraIndexContainer.begin();
}


void CameraManager::RemoveCameraIndex(const std::string id)
{
	int _index;
	std::vector< std::string >::iterator location;
	location = std::find( m_cameraIndexContainer.begin(), m_cameraIndexContainer.end(), id );
	if ( location != m_cameraIndexContainer.end() ) // founded
	{
		_index = location - m_cameraIndexContainer.begin();
		//check if current camera index isn't equal to founded camera
		if ( m_currentCameraIndex == _index )
			setCurrentCameraIndex("mainCamera");
		
		//remove selected camera index
		m_cameraIndexContainer.erase(m_cameraIndexContainer.begin()+ _index);
	}
}

/*----- Handle a keys and mouse events -----*/
void CameraManager::OnKey(unsigned char key, int x, int y)
{
	switch(key)
	{
	  case ']':
		  changeCurrentCamera(ft_Forward);
	    break;
	  case '[':
		  changeCurrentCamera(ft_Backward);
	    break;
	  case '\\':
		  // TODO change camera Type ( StaticCamera, SpringCamera, FlyCamera, Tracing Camera )
		  m_currentCamera->changeCameraMode();
		  m_currentCamera->PrintInfo();
		break;
	  //case 'x': RemoveCamera("frontLeft");
	  //default:
	    break;
	}
}


void CameraManager::OnSpecial(int key, int x, int y)
{

}

void CameraManager::OnMouseButtonDown(int button, int x, int y)
{
	// update mouse button states
	if(button == GLUT_LEFT_BUTTON)
	{
	m_bLeftMouseButtonDown = true;
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
	m_bRightMouseButtonDown = true;
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
	m_bMiddleMouseButtonDown = true;
	}
	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;    
}

void CameraManager::OnMouseButtonUp(int button, int x, int y)
{
	// update mouse button states
	if(button == GLUT_LEFT_BUTTON)
	{
		m_bLeftMouseButtonDown = false;
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		m_bRightMouseButtonDown = false;
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
		m_bMiddleMouseButtonDown = false;
	}

	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;
}


void CameraManager::OnMouseMove(int x, int y)
{
	// update pitch/yaw angles
	if(m_bLeftMouseButtonDown)
	{
		// calculate new angles
		m_yawAngle += (float)(x - m_mouseX);
		m_pitchAngle -= (float)(y - m_mouseY);
	}

	// update distance
	if(m_bRightMouseButtonDown)
	{
		// calculate new distance
		m_distance -= (float)(y - m_mouseY); 
		if(m_distance < 0.0f) m_distance = 0.0f;
	}

	// update Y screen position
	if(m_bMiddleMouseButtonDown)
	{
		//calculate new Y position
		m_camUpDown +=(float)(y-m_mouseY);	
	}
	
	if ((m_currentCamera!=NULL)&&(m_currentCamera->getCameraMode() == ft_FlyCamera)) //update only if FlyCamera
	{
		m_currentCamera->setYawAngle(m_yawAngle);
		m_currentCamera->setPitchAngle(m_pitchAngle);
		m_currentCamera->setDistance(m_distance);
		m_currentCamera->setCamUpDown(m_camUpDown);
	}
	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;
}




