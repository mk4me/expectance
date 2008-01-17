/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak
 */

#ifndef _GEN_MESHOBJECT_FACTORY_H
#define _GEN_MESHOBJECT_FACTORY_H



#include "cal3dobject.h"



namespace ft
{
	/**
	 * Class Cal3DObjectFactory: is responsible for creation of meshobjects. It is also responsible for creation 
     * and use of CalModel-s and CalCoreModel-s to which specific cal3dobject refers.
     * 
	 **/
    class ENGINE_API Cal3DObjectFactory
    {
    public:
        Cal3DObjectFactory (void) { /*empty*/ }
	    virtual ~Cal3DObjectFactory (void) { /*empty*/ }

        static Cal3DObjectFactory * getMeshObjectFactoryInstance();
        static void DestroyInstance();

        Cal3DObject* CreateMeshObject(const std::string modelName, const std::string objectName);

    protected:
        virtual Cal3DObject* CreateMeshObjectInstance(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName);
        virtual Cal3dType* CreateCoreModel(const std::string &typeName);
        virtual Cal3dType* LoadCalCoreModel(const std::string modelName);

        bool m_source_3dsmax;

    private:
        static Cal3DObjectFactory* m_instance;
        
        std::map<std::string,CalCoreModel*> m_coreModels;


        void ReleaseCalCoreModels();

        bool ParseModelConfiguration(const std::string& modelName, CalCoreModel* coreModel);

        void InitCoreModelMaterials(CalCoreModel* coreModel);

        void InitModelMeshes(CalCoreModel* coreModel, CalModel* model);

        GLuint LoadTexture(const std::string& strFilename);
    };
}

#endif //_GEN_MESHOBJECT_FACTORY_H