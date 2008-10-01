/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#ifndef _FT_XMLSCENEPARSER_H
#define _FT_XMLSCENEPARSER_H

#include "tinyxml/tinyxml.h"
#include "evolution/world.h"
#include <osg/MatrixTransform>

namespace ft
{
	static const std::string TAG_OBJECTS = "Objects";
	static const std::string TAG_AVATAR = "Avatar";
	static const std::string TAG_POSITION = "Position";
	static const std::string TAG_ROTATION = "Rotation";

	static const std::string ATTR_NAME = "name";
	static const std::string ATTR_TYPE = "type";
	static const std::string ATTR_X = "x";
	static const std::string ATTR_Y = "y";
	static const std::string ATTR_Z = "z";
	static const std::string ATTR_ANGLE = "angle";


	/**
	 * 
	 */
	class XMLSceneParser
    {
	public:

		XMLSceneParser() {}
		virtual ~XMLSceneParser(void ) {}

		bool LoadScene(const std::string& file, World* world, osg::MatrixTransform* worldTransformNode);

		void ParseObjects(TiXmlElement *element, World* world,  osg::MatrixTransform* worldTransformNode);
		Avatar* ParseAvatar(TiXmlElement *element,  osg::MatrixTransform* worldTransformNode, const std::string& avatarName, const std::string& avatarType);
    };

}
#endif //_FT_XMLSCENEPARSER_H
