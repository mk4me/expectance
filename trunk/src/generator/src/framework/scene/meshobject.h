/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_MESHOBJECT_H
#define _GEN_MESHOBJECT_H

#include "cal3d/cal3d.h"
#include "../core/UpdateObject.h"
#include "sceneobject.h"
#include "OGLContext.h"

#define MAXBONESPERMESH 29

namespace ft
{
	/* A MeshObject class 
	 *! Base class for all objects that has refernce in Cal3d (which contain meshes, materials, skeleton and animations)
     * \param CalModel * calModel -- model in Cal3d which is referenced by this MeshObject
	 **/
    class MeshObject : public UpdateObject, public SceneObject
    {
    public:
		MeshObject(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName);
	    virtual ~MeshObject(void);
        virtual void Destroy(void);

		bool InitHardwareAcceleration();
        void SetCalModel(CalModel* calModel);
        CalModel* GetCalModel();
		
        void SetCalCoreModel(CalCoreModel* calCoreModel);
        CalCoreModel* GetCalCoreModel();


        void OnMessage(Message* msg);  // OVERRIDEN, receives a message from UpdateManager
		virtual bool Render();
		virtual bool RenderShadow();

        void ChangeRenderMethod();
		void setRenderMethod(const int renderMethod);

		void ChangeShadow();
		void setShadow(const bool shadow);

        void SetLodLevel(float level);

        void setStartPosition(const CalVector& pos) { m_vStartPosition = pos; }  //TODO : should be move to MovablaAvatar
        const CalVector& getStartPosition() { return m_vStartPosition; }

    protected:
        CalVector m_vStartPosition; //TODO : should be move to MovablaAvatar        
		bool loadBufferObject();
        CalModel* m_calModel;  //cal3d model which is represented by this mesh object
        CalCoreModel* m_calCoreModel;
		CalHardwareModel* m_calHardwareModel;
		unsigned int m_vertexProgramId;
		unsigned int m_fragmentProgramId;
		unsigned int m_bufferObject[6];

    private:
   	    int m_vertexCount;  //tmp
		int m_faceCount;    //tmp
		bool m_shadow; 
		bool m_hardwareRendering;
		int m_renderMethod; //tmp
		void RenderMeshObject(const int renderMethod, const bool shadow);
		void SoftwareRenderModelMesh(const bool shadow);
		void HardwareRenderModelMesh(const bool shadow);
		void RenderModelSkeleton(const bool shadow);
		void RenderModelBoundingBox(const bool shadow);
    };
};

#endif //_GEN_MESHOBJECT_H