/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_AVATAR_H
#define _GEN_AVATAR_H

#include "cal3d/cal3d.h"
#include "../core/controlobject.h"
#include "../scene/sceneobject.h"
#include "../scene/OGLContext.h"

namespace ft
{
    class Avatar : public ControlObject, public SceneObject
    {
    public:
		Avatar::Avatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName);
		Avatar(CalModel* calModel, CalCoreModel* calCoreModel); 
	    virtual ~Avatar(void) { /*empty*/}

        void SetCalModel(CalModel* calModel);
        CalModel* GetCalModel();

        void SetCalCoreModel(CalCoreModel* calCoreModel);
        CalCoreModel* GetCalCoreModel();


        void OnMessage(Message* msg);  // OVERRIDEN, receives a message from ControlManager
		virtual bool Render();

        void ChangeRenderMethod();
		void setRenderMethod(const int renderMethod);

		void ChangeShadow();
		void setShadow(const bool shadow);

        void SetLodLevel(float level);

    protected:
        CalModel* m_calModel;  //cal3d model which is represented by this avatar object
        CalCoreModel* m_calCoreModel;

    private:
        std::string m_name;

   	    int m_vertexCount;  //tmp
		int m_faceCount;    //tmp
		bool m_shadow; 
		int m_renderMethod; //tmp
		void RenderAvatar(const int renderMethod, const bool shadow);
		void RenderModelMesh(const bool shadow);
		void RenderModelSkeleton(const bool shadow);
		void RenderModelBoundingBox(const bool shadow);

      
    };
};

#endif //_GEN_AVATAR_H
