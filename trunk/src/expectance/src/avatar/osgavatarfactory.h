/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _OSGAVATARFACTORY_H
#define _OSGAVATARFACTORY_H

#include "osgavatartype.h"
#include "osgavatar.h"
#include <osg/MatrixTransform>
#include "../app/global.h"

namespace ft
{
	/**
	 * Class OsgAvatarFactory: is responsible for creation of avatars. object refers.
     * 
	 **/
    class OsgAvatarFactory
    {
    public:
        OsgAvatarFactory(void) { /*empty*/ }
	    virtual ~OsgAvatarFactory(void) { /*empty*/ }

        static OsgAvatarFactory* getOsgAvatarFactoryInstance();
        static void DestroyInstance();

        OsgAvatar* CreateAvatar(const std::string avatarTypeName, const std::string objectName);
		
		OsgAvatarType* getAvatarType(const std::string &typeName);

		static void AddAvatarToScene(Avatar* av, osg::MatrixTransform* worldTransformNode,
						const osg::Vec3d& vStartPos, const osg::Quat  vStartRotation);

		static std::string getAvatarPath(const std::string& modelName);

    protected:
		        
        virtual OsgAvatarType* CreateAvatarType(const std::string &typeName);

//        bool m_source_3dsmax;

    private:
        static OsgAvatarFactory* m_instance;
        
        std::map<std::string,OsgAvatarType*> m_avatarTypes;

        void ReleaseAvatarTypes();
    };
}

#endif //_OSGAVATARFACTORY_H
