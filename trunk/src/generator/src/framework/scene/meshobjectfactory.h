/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MESHOBJECT_FACTORY_H
#define _GEN_MESHOBJECT_FACTORY_H


#include "../core/global.h"
#include "meshobject.h"
#include <map>

namespace ft
{
	/**
	 * Class MeshObjectFactory: is responsible for creation of meshobjects. It is also responsible for creation 
     * and use of CalModel-s and CalCoreModel-s to which specific meshobject refers.
     * 
	 **/
    class MeshObjectFactory
    {
    public:
        MeshObjectFactory (void) { /*empty*/ }
	    virtual ~MeshObjectFactory (void) { /*empty*/ }

        static MeshObjectFactory * getMeshObjectFactoryInstance();
        static void DestroyInstance();

        MeshObject* CreateMeshObject(const std::string modelName, const std::string objectName);

    protected:
        virtual MeshObject* CreateMeshObjectInstance(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName);

    private:
        static MeshObjectFactory* m_instance;
        
        std::map<std::string,CalCoreModel*> m_coreModels;

        CalCoreModel* LoadCalCoreModel(const std::string modelName);

        void ReleaseCalCoreModels();

        bool ParseModelConfiguration(const std::string& strFilename, CalCoreModel* coreModel);

        void InitCoreModelMaterials(CalCoreModel* coreModel);

        void InitModelMeshes(CalCoreModel* coreModel, CalModel* model);

        GLuint LoadTexture(const std::string& strFilename);
    };
};

#endif //_GEN_MESHOBJECT_FACTORY_H