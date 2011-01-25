#include "datacollector.h"
#include <osg/Vec3d>
#include "../avatar/osgavatar.h"
#include "xmlsceneparser.h"
#include "../app/global.h"

using namespace ft;

//! gets current avatar instances from application and prepares configuration file onto that information
void DataCollector::Init()
{
	Dump2File();
}

void DataCollector::Refresh()
{
	ReadFromFile(FT_DATA_PATH +"datacollector.xml");
}

void DataCollector::Dump2File()
{
	std::vector<ft::Avatar*> avatarsCollection  =  ControlManager::getInstance()->getAvatarsCollection();
	// init xml structure
	TiXmlDocument _doc;
 	TiXmlDeclaration* _decl = new TiXmlDeclaration( "1.0", "", "" );  
	_doc.LinkEndChild( _decl ); 

	TiXmlElement *_avatarsNode = new TiXmlElement( "Avatars" );
	_doc.LinkEndChild(_avatarsNode);


	// dump actual avatars parameters to file.
	for(unsigned int i = 0; i< avatarsCollection.size(); i++)
	{

		OsgAvatar* avImpl = (OsgAvatar*)avatarsCollection[i]->getImplementation();
		double angle;
		avImpl->getGlobalRotation().getRotate(angle, osg::Vec3d(0,0,1));
		
		TiXmlElement * _avatar = new TiXmlElement( TAG_AVATAR );  
		TiXmlElement * _position = new TiXmlElement( TAG_POSITION );
		TiXmlElement * _rotation = new TiXmlElement( TAG_ROTATION );
		TiXmlElement * _speedFactor = new TiXmlElement( TAG_SPEEDFACTOR );
		_avatar->LinkEndChild( _position );
		_avatar->LinkEndChild( _rotation );
		_avatar->LinkEndChild( _speedFactor );

		_avatarsNode->LinkEndChild( _avatar );

		_avatar->SetAttribute(ATTR_NAME, avImpl->getName());
		_avatar->SetAttribute(ATTR_TYPE, avImpl->getType());
		_position->SetDoubleAttribute(ATTR_X.c_str(), avImpl->getPosition()[0]);
		_position->SetDoubleAttribute(ATTR_Y.c_str(), avImpl->getPosition()[1]);
		_position->SetDoubleAttribute(ATTR_Z.c_str(), avImpl->getPosition()[2]);
		_rotation->SetDoubleAttribute(ATTR_ANGLE.c_str(), radToDeg(angle)); // is it necessary to change radian to degrees here?
		_speedFactor->SetDoubleAttribute(ATTR_SPEED.c_str(), avImpl->getCurrSpeedFactor());
		_speedFactor->SetDoubleAttribute(ATTR_MINSPEED.c_str(), avImpl->getSpeedFactorMin());
		_speedFactor->SetDoubleAttribute(ATTR_MAXSPEED.c_str(), avImpl->getSpeedFactorMax());
		
	}
	// create other parameters for application (maybe create special interface for that in the future)
	_doc.SaveFile(FT_DATA_PATH+"datacollector.xml");
}


OsgAvatar* DataCollector::getAvatar(const std::string& name)
{
	std::vector<ft::Avatar*> avatarsCollection  =  ControlManager::getInstance()->getAvatarsCollection();
	OsgAvatar* av = NULL;
	for(unsigned int i = 0; i < avatarsCollection.size(); i++)
	{
		av = (OsgAvatar*)avatarsCollection[i]->getImplementation();
		if (av->getName().compare(name) == 0 ) // founded !!
			break;
	}
	return av;
}


void DataCollector::ReadFromFile(const std::string& file)
{
	TiXmlDocument doc(file.c_str());
	if (!doc.LoadFile()) return;

	//avatars
	{
	
	//OsgAvatar* avImpl = (OsgAvatar*)avatarsCollection[i]->getImplementation();
	TiXmlElement* childElement = 0;
	for( childElement = doc.FirstChildElement();	childElement;	 childElement = childElement->NextSiblingElement() )
	{
		std::string element_str(childElement->Value());
		if (element_str.compare("Avatars") == 0)
		{
			// avatar
		TiXmlElement* avElement = 0;
		for( avElement = childElement->FirstChildElement();	avElement;	 avElement = avElement->NextSiblingElement() )
			{
				std::string element_str(avElement->Value());
				if (element_str.compare(TAG_AVATAR) == 0)
				{
					std::string avName;
					std::string avatarType;
					int error =  avElement->QueryValueAttribute(ATTR_NAME, &avName);
					if (error)
							avName = "<UNDEFINED_NAME>";
					error =  avElement->QueryValueAttribute(ATTR_TYPE, &avatarType);

					if (!error)
					// rotation and translation
					{
						OsgAvatar* avImpl = getAvatar(avName);
						if (avImpl!=NULL)
						{
							TiXmlElement* avSubElement = 0;
							for( avSubElement = avElement->FirstChildElement();	avSubElement;	 avSubElement = avSubElement->NextSiblingElement() )
							{	
								std::string element_str(avSubElement->Value());
								if (element_str.compare(TAG_POSITION) == 0)
								{
									float x,y,z;
									int error = avSubElement->QueryFloatAttribute(ATTR_X.c_str(), &x);
									if (error)  x = 0.0f;
									error = avSubElement->QueryFloatAttribute(ATTR_Y.c_str(), &y);
									if (error)  y = 0.0f;
									error = avSubElement->QueryFloatAttribute(ATTR_Z.c_str(), &z);
									if (error)  z = 0.0f;

									avImpl->setPosition(osg::Vec3d(x,y,z));
								}
								else if (element_str.compare(TAG_ROTATION) == 0)
								{
									float angle;
									int error =  avSubElement->QueryFloatAttribute(ATTR_ANGLE.c_str(), &angle);
									if (error)  angle = 0.0f;
									avImpl->setGlobalRotation( osg::Quat(degToRad(angle), osg::Vec3d(0,0,1)) );
								}
								else if (element_str.compare(TAG_SPEEDFACTOR) == 0)
								{
									float min,value,max;
									int error = avSubElement->QueryFloatAttribute(ATTR_SPEED.c_str(), &value);
									if (error) value = 0.0f;
									error = avSubElement->QueryFloatAttribute(ATTR_MINSPEED.c_str(), &min);
									if (error)  min = 0.0f;
									error = avSubElement->QueryFloatAttribute(ATTR_MAXSPEED.c_str(), &max);
									if (error)  max = 0.0f;

									avImpl->setCurrSpeedFactor(value);
									avImpl->setSpeedFactorMin(min);
									avImpl->setSpeedFactorMax(max);
								}
	
							}

						}
						//else read next structure element
					}
					
					////Objects(childElement, world, worldTransformNode);
				} // TAG_AVATAR
			}
		} // TAG_Avatars
	}

	}

}

