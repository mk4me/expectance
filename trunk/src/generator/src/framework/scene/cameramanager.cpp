/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "cameramanager.h"
#include "oglcontext.h"

using namespace ft;

CameraManager* CameraManager::m_instance = NULL;

CameraManager* CameraManager::getInstance()
{
    if (m_instance == NULL)
    {
        if (Debug::CAMERA>0)
            cout << "CameraManager::getInstace(): instance of CameraManager created " << endl;

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

bool CameraManager::Init()
{
	//read camera configurations
	if (!m_cameraConfiguration.Init())
	{
		return false;
	}

	m_bLeftMouseButtonDown = false;
	m_bRightMouseButtonDown = false;
	m_bMiddleMouseButtonDown = false;
	
	AddCamera(new Camera("mainCamera"));	
	setCurrentCamera("mainCamera");
	m_currentCamera->setOrbitCameraRadius(1000.0f); // bigger radius for global scene view for mainCamera
	m_currentSceneObjectID = "mainCamera"; //set for current camera to avoid current empty object


	return true;
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

bool CameraManager::AddCamera(std::string camName, float yaw, float pitch, float roll, float dist, CameraMode mode)
{
	Camera *cam = new Camera(camName);
	cam->Init(yaw,pitch,roll,dist,mode);
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
		////m_pitchAngle = cam->getPitchAngle();
		////m_yawAngle = cam->getYawAngle();
		////m_distance = cam->getDistance();
		////m_camUpDown = cam->getCamUpDown();
		////m_camLeftRight = cam->getCamLeftRight();

		m_currentCamera = cam;
		setCurrentCameraIndex(id);
	}
	else
	{
		////m_pitchAngle = 20.0f;
		////m_yawAngle = 0.0f;
		////m_rollAngle = 0.0f;
		////m_distance = 800.0f;
		////m_camUpDown = 0.0f;
		////m_camLeftRight = 0.0f;
		if ((m_currentCamera = getCamera("mainCamera")) == NULL) 
			AddCamera(new Camera("mainCamera"));
		m_currentCamera = new Camera("mainCamera");
		setCurrentCameraIndex("mainCamera");
	}
}

//! set current camera according to choosen configuration
void CameraManager::setCurrentCameraFromConfiguration(int key)
{
	Camera *_cam;
	std::string _id; 
	float _yaw = 0, _pitch = 20, _roll = 0;
	_id.clear();

	//1. check configuration KEY
	CameraDefinition *_camDef = m_cameraConfiguration.getConfiguration(key);
	if (_camDef==NULL) return; // wrong key pressed?
	
	//2. select apropriate action for camera type
	switch(_camDef->type)
	{
	case ft_MainCamera:
			_id = "mainCamera";
		break;
	case ft_ActiveAvatarCamera:
			_id = m_currentSceneObjectID;
		break;
	default:
		break;
	}
	//3. check if camera exists
	_cam = getCamera(_id);
	if (_cam==NULL)
		return; //there is no such camera in camera container
	
	//4. change camera parameters
	_cam->setCameraMode(_camDef->mode);
	
	switch(_camDef->location)
	{
		case ft_AutoLocation:		break;

		case ft_FrontCenter:		_pitch = 0;		_yaw = 0.0f;   break;
		case ft_TopFrontCenter:		_pitch = 20;	_yaw = 0.0f;   break;
		case ft_BottomFrontCenter:	_pitch = -20;	_yaw = 0.0f;   break;
			
		case ft_FrontLeft:			_pitch = 0;		_yaw = 45.0f;  break;
		case ft_TopFrontLeft:		_pitch = 20;	_yaw = 45.0f;  break;
		case ft_BottomFrontLeft:	_pitch = -20;	_yaw = 45.0f;  break;

		case ft_Left:				_pitch = 0;		_yaw = 90.0f;  break;
		case ft_TopLeft:			_pitch = 20;	_yaw = 90.0f;  break;
		case ft_BottomLeft:			_pitch = -20;	_yaw = 90.0f;  break;

		case ft_BackLeft:			_pitch = 0;		_yaw = 135.0f; break;
		case ft_TopBackLeft:		_pitch = 20;	_yaw = 135.0f; break;
		case ft_BottomBackLeft:		_pitch = -20;	_yaw = 135.0f; break;

		case ft_BackCenter:			_pitch = 0;		_yaw = 180.0f; break;
		case ft_TopBackCenter:		_pitch = 20;	_yaw = 180.0f; break;
		case ft_BottomBackCenter:	_pitch = -20;	_yaw = 180.0f; break;

		case ft_BackRight:			_pitch = 0;		_yaw = 225.0f; break;
		case ft_TopBackRight:		_pitch = 20;	_yaw = 225.0f; break;
		case ft_BottomBackRight:	_pitch = -20;	_yaw = 225.0f; break;

		case ft_Right:				_pitch = 0;		_yaw = 270.0f; break;
		case ft_TopRight:			_pitch = 20;	_yaw = 270.0f; break;
		case ft_BottomRight:		_pitch = -20;	_yaw = 270.0f; break;

		case ft_FrontRight:			_pitch = 0;		_yaw = 315.0f; break;
		case ft_TopFrontRight:		_pitch = 20;	_yaw = 315.0f; break;
		case ft_BottomFrontRight:	_pitch = -20;	_yaw = 315.0f; break;

		case ft_Center:				_pitch = 0;		_yaw = 0.0f; break;
		case ft_TopCenter:			_pitch = 90;	_yaw = 90.0f; break;
		case ft_BottomCenter:		_pitch = -90;	_yaw = 90.0f; break;
		
		default:	break;	
	}
	

	//enum CameraLocation {ft_FrontLeft, ft_FrontCenter, ft_FrontRight, ft_Left, ft_Center, ft_Right, ft_BackLeft, ft_BackCenter, ft_BackRight,
	//					 ft_TopFrontLeft, ft_TopFrontCenter, ft_TopFrontRight, ft_TopLeft, ft_TopCenter, ft_TopRight, ft_TopBackLeft, 
	//					 ft_TopBackCenter, ft_TopBackRight, ft_BottomFrontLeft, ft_BottomFrontCenter, ft_BottomFrontRight, ft_BottomLeft,
	//					 ft_BottomCenter, ft_BottomRight, ft_BottomBackLeft, ft_BottomBackCenter, ft_BottomBackRight, ft_AutoLocation
	//					 };

	if ( (_camDef->mode == ft_StaticCamera) || (_camDef->mode == ft_FlyCamera) )
	_cam->Init(_yaw, _pitch,_cam->getRoll(),_cam->getDistance(),_camDef->mode);

	//5. set that camera active unless it is already active
	if (_cam != m_currentCamera)
	{
		m_currentCamera = _cam;
		setCurrentCameraIndex(_id);
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

const std::string CameraManager::getCurrentCameraInfo()
{
	return m_currentCamera->getCameraInfo();
}
const void CameraManager::RenderCurrentCamera()
{
	m_currentCamera->Render();
}


const std::string CameraManager::getCurrentSceneObjectID()
{
	return m_currentSceneObjectID;
}

const void CameraManager::setCurrentSceneObjectID(const std::string id)
{
	m_currentSceneObjectID = id;
	if (m_currentCamera->getCameraMode()==ft_ThirdPersonCamera) // swich only when camera is in third person mode
	setCurrentCameraFromConfiguration(GLUT_KEY_F10); //update tracing camera
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
		  m_currentCamera->PrintInfo();
	    break;
	  case '[':
		  changeCurrentCamera(ft_Backward);
		  m_currentCamera->PrintInfo();
	    break;
	  case '\\':
		  m_currentCamera->ChangeCameraMode();
		  m_currentCamera->PrintInfo();
		break;
	  case '|':
		  m_currentCamera->ChangeZoom();
	  break;
	  case 'w':
	  case 's':
	  case 'a':
	  case 'd':
	  case 'r':
	  case 'f':
		  m_currentCamera->OnKey(key);
	  break;

	  default:
	    break;
	}
}


void CameraManager::OnSpecial(int key, int x, int y)
{
	switch(key)
	{
	  case GLUT_KEY_F9:
	  case GLUT_KEY_F10:
	  case GLUT_KEY_F11:
	  case GLUT_KEY_F12:
		setCurrentCameraFromConfiguration(key);
	    break;
	  default:
	    break;
	}

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
}


void CameraManager::OnMouseMove(int x, int y)
{
	if ((m_currentCamera!=NULL)&&(m_currentCamera->getCameraMode() == ft_FlyCamera)&&(m_bLeftMouseButtonDown)) //update only if FlyCamera
	{
		m_currentCamera->OnMouseMove((float)x/(float)OGLContext::getInstance()->getWidth(), (float)y/(float)OGLContext::getInstance()->getHeight());
	}
}




