/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#ifndef _GEN_AVATAR_H
#define _GEN_AVATAR_H

#include "cal3d/cal3d.h"
#include "../core/controlobject.h"
#include "../scene/sceneobject.h"

namespace ft
{
    class Avatar : public ControlObject
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

        void ChangeRendeMethod();
		void setRenderMethod(const int renderMethod);

        void SetLodLevel(float level);

    protected:
        CalModel* m_calModel;  //cal3d model which is represented by this avatar object
        CalCoreModel* m_calCoreModel;

    private:
        std::string m_name;

   	    int m_vertexCount;  //tmp
		int m_faceCount;    //tmp
		bool m_shadow;      //tmp
		int m_renderMethod; //tmp
		void RenderModelMesh(bool shadow);
		void RenderModelSkeleton(bool shadow);
		void RenderModelBoundingBox(bool shadow);
      
    };
};

#endif //_GEN_AVATAR_H
