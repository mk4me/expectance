/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */

#include "cameramanager.h"

using namespace ft;

CameraConfiguration::~CameraConfiguration()
{
	std::map<std::string,CameraDefinition*>::iterator it = m_cameraConfigurationContainer.begin(); //clean camera definition list
	for( ; it != m_cameraConfigurationContainer.end(); ++it ) {
		delete it->second;
    }
    m_cameraConfigurationContainer.clear();
}



bool CameraConfiguration::Init()
{
	bool _final = true;
	vector<string> _tokens;
	std::string _cameraList;
	
	//1. read main camera configuration list
	_cameraList = Config::getInstance()->GetStrVal("camera_configurations");
	_cameraList = StringHelper::ClearDelimiters(_cameraList, '(',')');
	_cameraList = StringHelper::RemoveChar(_cameraList,' ');
	_tokens = StringHelper::Split(_cameraList, ",");
	if (_tokens[0] == "<KEY_NOT_FOUND>")
		return false;
	for (unsigned int i = 0; i <_tokens.size(); i++) //create cameras configuration map
	{
		vector<string> _cameraParameters; //parameters vector (hotkey, camera type, camera mode, camera location)
		std::string _cameraEntity;
		_cameraEntity = Config::getInstance()->GetStrVal(_tokens[i]);
		_cameraEntity = StringHelper::ClearDelimiters(_cameraEntity, '(', ')');
		_cameraEntity = StringHelper::RemoveChar(_cameraEntity, ' ');
		_cameraEntity = StringHelper::RemoveChar(_cameraEntity, '\t');
		_cameraParameters = StringHelper::Split(_cameraEntity,",");
		if (_cameraParameters.size() != 4) 
			continue;
		else
		{
			////// 2. dispatch each option
			int _key = GETKeyCode(_cameraParameters[0]);
			CameraType _camType = GETCameraType(_cameraParameters[1]);
			CameraMode _camMode = GETCameraMode(_cameraParameters[2]);
			CameraLocation _camLocation = GETCameraLocation(_cameraParameters[3]);

			std::cout << _cameraParameters[0] <<", "<< _cameraParameters[1] <<", "<< _cameraParameters[2] <<", "<< _cameraParameters[3] << "\n";

			//3. add option to menu
			AddConfiguration(_key, _camType, _camMode, _camLocation);
			//m_mainMenu->AddObject(mi);
			//listID++;

		}
		_cameraParameters.clear();
		_cameraEntity.clear();
	}

	return _final;
}


bool CameraConfiguration::AddConfiguration(CameraDefinition *pCamObj)
{
	std::string _id = pCamObj->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,CameraDefinition*>::iterator it = m_cameraConfigurationContainer.find(_id);
		if ( it!=m_cameraConfigurationContainer.end()) { 
			return false;
		}
		else
		{
			m_cameraConfigurationContainer.insert( std::make_pair( std::string(_id), pCamObj ) );
		}
	}
	return true;
}

bool CameraConfiguration::AddConfiguration(int hotKey, CameraType type, CameraMode mode, CameraLocation location)
{
	CameraDefinition* _def = new CameraDefinition();
    std::ostringstream _oss;
	_def->hotKey = hotKey;
	_def->type = type;
	_def->mode = mode;
	_def->location = location;

	_oss << hotKey;
	_def->setID(_oss.str());

	return AddConfiguration(_def);
}

CameraDefinition* CameraConfiguration::getConfiguration(std::string id)
{
 	std::map<std::string,CameraDefinition*>::iterator it = m_cameraConfigurationContainer.find(id);
	if ( it!=m_cameraConfigurationContainer.end()) { 
		return it->second;
	}
	return NULL;
}

CameraDefinition* CameraConfiguration::getConfiguration(int id)
{
	std::ostringstream _oss;
	_oss << id;
	return getConfiguration(_oss.str());
}