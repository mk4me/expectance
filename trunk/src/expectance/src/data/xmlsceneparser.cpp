/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include <iostream>

#include "xmlsceneparser.h"
#include "evolution/factory.h"
#include "../control/controlmanager.h"
#include "../timeline/lcsmodifier.h"
#include "../control/controlmanager.h"
#include "../ai/goals/randommovegoal.h"
#include "../ai/goals/changedirgoal.h"
#include "../ai/goals/limitedareagoal.h"
#include "../ai/constraints/limitedareaconstraint.h"
#include"../avatar/osgavatarfactory.h"


using namespace ft;
using namespace std;

bool XMLSceneParser::LoadScene(const std::string& file, World* world, osg::MatrixTransform* worldTransformNode)
{
	TiXmlDocument doc( file.c_str() );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		cout << "Could not load test file " << file << " Error = " << doc.ErrorDesc() << endl;
	}
	else
	{
		cout << "File " << file << " load succesfully " << endl; 
		//doc.Print( stdout );
	}

	TiXmlElement* childElement = 0;
	for( childElement = doc.FirstChildElement();	childElement;	 childElement = childElement->NextSiblingElement() )
	{
		std::string element_str(childElement->Value());
		if (element_str.compare(TAG_OBJECTS) == 0)
		{
			ParseObjects(childElement, world, worldTransformNode);
		} 
	}

	return loadOkay;
}

void XMLSceneParser::ParseObjects(TiXmlElement *element, World* world,  osg::MatrixTransform* worldTransformNode)
{
	TiXmlElement* childElement = 0;
	for( childElement = element->FirstChildElement();	childElement;	 childElement = childElement->NextSiblingElement() )
	{
		std::string element_str(childElement->Value());
		if (element_str.compare(TAG_AVATAR) == 0)
		{
			std::string avName;
			std::string avatarType;

			int error =  childElement->QueryValueAttribute(ATTR_NAME, &avName);

			if (error)
					avName = "<UNDEFINED_NAME>";

			error =  childElement->QueryValueAttribute(ATTR_TYPE, &avatarType);


			if (!error)
			{
				avatarType = OsgAvatarFactory::getAvatarPath(avatarType);
				Avatar* av = ParseAvatar(childElement, worldTransformNode, avName, avatarType);
				if (av != NULL)
					world->AddAvatar(av);

			}
		}
	}
}

Avatar* XMLSceneParser::ParseAvatar(TiXmlElement *element,  osg::MatrixTransform* worldTransformNode, const std::string& avatarName, const std::string& avatarType)
{
	
	
	Avatar* av = dynamic_cast<Avatar*>(ft::Factory::getInstance()->createAvatar(avatarName, avatarType)); 

	if (av != NULL)  
    {

		osg::Vec3d vStartPos(0,0,0);
		osg::Quat  qStartRotation;

		TiXmlElement* childElement = 0;
		for( childElement = element->FirstChildElement();	childElement;	 childElement = childElement->NextSiblingElement() )
		{
			std::string element_str(childElement->Value());
			if (element_str.compare(TAG_POSITION) == 0)
			{
				float x,y,z;
				int error =  childElement->QueryFloatAttribute(ATTR_X.c_str(), &x);
				if (error)  x = 0.0f;
				error =  childElement->QueryFloatAttribute(ATTR_Y.c_str(), &y);
				if (error)  y = 0.0f;
				error =  childElement->QueryFloatAttribute(ATTR_Z.c_str(), &z);
				if (error)  z = 0.0f;
				vStartPos = osg::Vec3d(x,y,z);
			}
			else if (element_str.compare(TAG_ROTATION) == 0)
			{
				float angle;
				int error =  childElement->QueryFloatAttribute(ATTR_ANGLE.c_str(), &angle);
				if (error)  angle = 0.0f;
				qStartRotation = osg::Quat(angle, osg::Vec3d(0,0,1));
			}
		}

		ControlManager::getInstance()->AddAvatar(av);
		av->AddController(new LCSModifier());

		OsgAvatar* avImpl = static_cast<OsgAvatar*>(av->getImplementation());
		avImpl->setName(avatarName); //mka 2008.09.06
		av->AddController(avImpl->getFootDetector());
		av->AddController(avImpl->getStopController()); //mka 2008.08.19
		OsgAvatarFactory::AddAvatarToScene(av, worldTransformNode, vStartPos, qStartRotation);
		av->StartSimulation();
	}

	return av;
}

