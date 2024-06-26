/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "cal3dobject.h"
#include "oglcontext.h"

using namespace ft;

int Cal3DObject::m_counter = 0;

/**
 * \brief Constructor for Cal3DObject
 *
 * \param CalModel * calModel - model in Cal3d which is referenced by this mesh object
 * \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this mesh object
 * \param const std::string modelName - name defined in ft::BaseObject which is the base class for mesh object
 **/
Cal3DObject::Cal3DObject(CalModel* calModel, Cal3dType* calCoreModel, const std::string modelName):
m_renderMethod(0),
m_shadow(true),
DynamicObjectID(m_counter++)
{
    m_calModel = calModel;
    m_calCoreModel = calCoreModel;
	setName(modelName);
	setRenderingOrder(ft_Rendering_Objects_Level);
	if (OGLContext::getInstance()->HardwareAcceleration == 1) //check Vertex Shader and load shaders
	{
		m_hardwareRendering = InitHardwareAcceleration();
	}
	else
		m_hardwareRendering = false;

	RENDER_CONTROL_PARAMETERS = ((Config::getInstance()->IsKey("RENDER_CONTROL_PARAMETERS")) && (Config::getInstance()->GetIntVal("RENDER_CONTROL_PARAMETERS")==1));
	std::ostringstream _oss;
	_oss << "id " << DynamicObjectID  << std::endl;
	setAnnotation(_oss.str());
	
	quadric = gluNewQuadric();
	if(!quadric)
		cerr << "Axis::Axis: quadric is zero." << endl;

}

/**
 * \brief Destructor
 *
 **/
Cal3DObject::~Cal3DObject()
{
	delete m_calHardwareModel;
	delete m_calModel;
	delete m_calCoreModel;

	if (m_hardwareRendering)
	{
		glDeleteProgramsARB(1, &m_vertexProgramId);
		glDeleteBuffersARB(6, m_bufferObject);
	}
	if(quadric)
		gluDeleteQuadric(quadric);

}

/// \brief Releases all resources and objects related to this Cal3DObject
void Cal3DObject::Destroy(void)
{

}

/**
 * \brief This method is called when new frame is updated from ft::UpdateManager
 *
 * \param float elapsedSeconds - time elapsed sice last update
 **/
void Cal3DObject::OnUpdate(const double elapsedSeconds)
{
    m_calModel->update(elapsedSeconds);

    //set position
    CalSkeleton *skel = GetCalModel()->getSkeleton();
    CalBone *bone = skel->getBone(0);
    CalVector pos = getPosition();
    bone->setTranslation(pos);

    //TODO: set orientation
    bone->calculateState();
}


/**
 * \brief Tries to reserve memory for object data in GPU memory and loads vertex and fragment program of shader from file
 *
 * \return bool - true if graphics accelerator suports vertex shader otherwise returns false
 **/
bool Cal3DObject::InitHardwareAcceleration()
{

	if(!loadBufferObject())
	{
      if (Debug::ERR)
          _dbg << Debug::ERR_STR << "Error loading vertex buffer object." << std::endl;

	  return false;
	}

	//m_vertexProgramId = OGLContext::getInstance()->VertexProgramId;
	if ((m_vertexProgramId = loadARBVertexProgram(FT_SHADERPATH + Config::getInstance()->GetStrVal("mesh_vertex_program"))) == 0)
	{
      if (Debug::ERR)
          _dbg << Debug::ERR_STR << "Error loading vertex program." << std::endl;

	  return false;
	}
//	if ((m_fragmentProgramId = loadARBFragmentProgram(FT_SHADERPATH + Config::getInstance()->GetStrVal("mesh_fragment_program"))) == 0)
//	{
//	  _dbg << "Error loading fragment program." << std::endl;
//	}

	return true;
}

/**
 * \brief Sets model in Cal3d which is referenced by this mesh object
 *
 * \param CalModel * calModel - Cal3d object
 **/
void Cal3DObject::SetCalModel(CalModel* calModel)
{
    m_calModel = calModel;
}

/**
 * \brief Returns model from Cal3d which is referenced by this mesh object
 *
 * \return CalModel * Cal3d model
 **/
CalModel* Cal3DObject::GetCalModel()
{
    return m_calModel;
}

/**
 * \brief Sets object representing type defined in Cal3d that is used to create CalModel for this mesh object
 *
 * \param Cal3dType * calCoreModel - object representing type of CalModel
 **/
void Cal3DObject::SetCalCoreModel(Cal3dType* calCoreModel)
{
    m_calCoreModel = calCoreModel;
}

/**
 * \brief Returns object representing type defined in Cal3d that is used to create CalModel for this mesh object
 *
 * \return Cal3dType * - object representing type of CalModel
 **/
Cal3dType* Cal3DObject::GetCalCoreModel()
{
    return m_calCoreModel;
}

/**
 * \brief This method is called when message is sent to this object from ft::UpdateManager
 *
 * \param ft::Message * msg - message from ft::UpdateManager
 **/
void Cal3DObject::OnMessage(Message* msg)
{
    if (Debug::UPDATE>0)
        _dbg << toString() << " received message: " << Message::_GET_MSG_NAME(msg->getType()) << std::endl;

    if (msg->getType() == MSG_PROPERTY_LOD)
    {
        SetLodLevel(msg->getParam()->getFloatValue());
    }
    else if (msg->getType() == MSG_PROPERTY_RENDER_METHOD)
    {
        ChangeRenderMethod();
    }
	else if (msg->getType() == MSG_PROPERTY_SHADOW)
	{
		ChangeShadow();
	}
}

/**
 * \brief Main rendering routine for Cal3DObject
 **/
bool Cal3DObject::Render()
{
	// draw the object that casts the shadow
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	//
	glPushMatrix();
		if (RENDER_CONTROL_PARAMETERS)
			RenderControlParameters();
		//glRotatef(-90,1.0f,0.0f,0.0f); //unnecessary while XYZ order in cal files

		if (OGLContext::getInstance()->HardwareAcceleration != 1)
		{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		}
		RenderMeshObject(m_renderMethod, false);
	glPopMatrix();

	// clear light
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_DEPTH_TEST);


	return true;
}

/**
 * \brief Main rendering routine for Cal3DObject's shadow
 **/
bool Cal3DObject::RenderShadow()
{
	if (m_shadow) //draw the shadow
	{
		m_calModel->setLodLevel(0.0f);
		glPushMatrix();
			GlShadowProjection();
			glColor4f(0.1f,0.1f,0.1f,0.5f);
			//glRotatef(-90,1.0f,0.0f,0.0f); //unnecessary while XYZ order in cal files
			RenderMeshObject(m_renderMethod, m_shadow);
		glPopMatrix();
		m_calModel->setLodLevel(1.0f);
	}
	return true;
}


/**
 * \brief Changes between three rendering methods: Mesh Skeleton and BoundingBox consecutivevely
 *
 **/
void Cal3DObject::ChangeRenderMethod()
{
    m_renderMethod = (m_renderMethod+1) % 3;
}

/**
 * \brief Sets between three rendering methods: Mesh Skeleton and BoundingBox
 *
 * \param const int renderMethod: 0 - Mesh, 1 - Skeleton, 2 - BoundingBox
 **/
void Cal3DObject::setRenderMethod(const int renderMethod)
{
	m_renderMethod = renderMethod;
}

/**
 * \brief Switches shadow on when it is off or switches shadow off when it is on.
 *
 **/
void Cal3DObject::ChangeShadow()
{
    m_shadow = !m_shadow;
}

/**
 * \brief Switches the shadow on or off.
 *
 * \param const bool - defines if shadow should be swithced on or off
 **/
void Cal3DObject::setShadow(const bool shadow)
{
	m_shadow = shadow;
}

/**
 * \brief Renders Cal3DObject taking into consideration specific parameters
 *
 * \param const int renderMethod: 0 - Mesh, 1 - Skeleton, 2 - BoundingBox
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Cal3DObject::RenderMeshObject(const int renderMethod, const bool shadow)
{
	if (renderMethod == 0)
	{
		if ((m_hardwareRendering)&&(!shadow))
			HardwareRenderModelMesh(shadow);
		else
			SoftwareRenderModelMesh(shadow);
	}
	else if (renderMethod == 1)
	{
		RenderModelSkeleton(shadow);
	}
	else if (renderMethod == 2)
	{
  		m_calModel->getSkeleton()->calculateBoundingBoxes();
		RenderModelBoundingBox(shadow);
	}
}


/**
 * \brief Renders Cal3DObject Mesh using software rendering method
 *
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Cal3DObject::SoftwareRenderModelMesh(const bool shadow)
{
	// get the renderer of the model
	CalRenderer *pCalRenderer = m_calModel->getRenderer();

	// begin the rendering loop
	if(pCalRenderer->beginRendering())
	{
		// we will use vertex arrays, so enable them
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		// get the number of meshes
		int meshCount = pCalRenderer->getMeshCount();

		// render all meshes of the model
		for(int meshId = 0; meshId < meshCount; meshId++)
		{
			// get the number of submeshes
			int submeshCount = pCalRenderer->getSubmeshCount(meshId);

			// render all submeshes of the mesh
			for(int submeshId = 0; submeshId < submeshCount; submeshId++)
			{
				// select mesh and submesh for further data access
				if(pCalRenderer->selectMeshSubmesh(meshId, submeshId))
				{
					unsigned char meshColor[4];
					GLfloat materialColor[4];

					if (!shadow)
					{
					// set the material ambient color
					pCalRenderer->getAmbientColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

					// set the material diffuse color
					pCalRenderer->getDiffuseColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

					// set the material specular color
					pCalRenderer->getSpecularColor(&meshColor[0]);
					materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
					glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

					// set the material shininess factor
					float shininess;
					shininess = 50.0f; //pCalRenderer->getShininess();
					glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
					}


					// get the transformed vertices of the submesh
					static float meshVertices[30000][3];
					int vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

					// get the transformed normals of the submesh
					static float meshNormals[30000][3];
					pCalRenderer->getNormals(&meshNormals[0][0]);

					// get the texture coordinates of the submesh
					static float meshTextureCoordinates[30000][2];
					int textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

					// get the faces of the submesh
					static CalIndex meshFaces[50000][3];
					int faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

					// set the vertex and normal buffers
					glVertexPointer(3, GL_FLOAT, 0, &meshVertices[0][0]);
					glNormalPointer(GL_FLOAT, 0, &meshNormals[0][0]);

					if(!shadow)
					{
						// set the texture coordinate buffer and state if necessary
						if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
						{
							glEnable(GL_TEXTURE_2D);
							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							glEnable(GL_COLOR_MATERIAL);

							// set the texture id we stored in the map user data
							glBindTexture(GL_TEXTURE_2D, (GLuint)pCalRenderer->getMapUserData(0));

							// set the texture coordinate buffer
							glTexCoordPointer(2, GL_FLOAT, 0, &meshTextureCoordinates[0][0]);
							glColor3f(1.0f, 1.0f, 1.0f);
						}
					}

					// draw the submesh
					if(sizeof(CalIndex)==2)
						glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_SHORT, &meshFaces[0][0]);
					else
						glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &meshFaces[0][0]);

					// disable the texture coordinate state if necessary
					if((pCalRenderer->getMapCount() > 0) && (textureCoordinateCount > 0))
					{
						glDisable(GL_COLOR_MATERIAL);
						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
						glDisable(GL_TEXTURE_2D);
					}

					// adjust the vertex and face counter
					m_vertexCount += vertexCount;
					m_faceCount += faceCount;

				}
			}
		}

		// clear vertex array state
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		// end the rendering
		pCalRenderer->endRendering();
	}
}

/**
 * \brief Renders Cal3DObject Mesh using hardware rendering method
 *
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Cal3DObject::HardwareRenderModelMesh(const bool shadow)
{
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_vertexProgramId );
    //glBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, m_fragmentProgramId ); // mka 2007-08-29

	glEnableVertexAttribArrayARB(0);
	glEnableVertexAttribArrayARB(1);
    glEnableVertexAttribArrayARB(2);
	glEnableVertexAttribArrayARB(3);
    glEnableVertexAttribArrayARB(8);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_VERTEX_PROGRAM_ARB);
    //if (m_fragmentProgramId) glEnable(GL_FRAGMENT_PROGRAM_ARB);


    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[0]);
	glVertexAttribPointerARB(0, 3 , GL_FLOAT, false, 0, NULL);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[1]);
	glVertexAttribPointerARB(1, 4 , GL_FLOAT, false, 0, NULL);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[2]);
    glVertexAttribPointerARB(2, 3 , GL_FLOAT, false, 0, NULL);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[3]);

	glVertexAttribPointerARB(3, 4 , GL_FLOAT, false, 0, NULL);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[4]);
	glVertexAttribPointerARB(8, 2 , GL_FLOAT, false, 0, NULL);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferObject[5]);


	int hardwareMeshId;

	for(hardwareMeshId=0;hardwareMeshId<m_calHardwareModel->getHardwareMeshCount() ; hardwareMeshId++)
	{
		m_calHardwareModel->selectHardwareMesh(hardwareMeshId);

		unsigned char meshColor[4];
		float materialColor[4];



		// set the material ambient color
		m_calHardwareModel->getAmbientColor(&meshColor[0]);
		materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);

		// set the material diffuse color
		m_calHardwareModel->getDiffuseColor(&meshColor[0]);
		materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

		// set the material specular color
		m_calHardwareModel->getSpecularColor(&meshColor[0]);
		materialColor[0] = meshColor[0] / 255.0f;  materialColor[1] = meshColor[1] / 255.0f; materialColor[2] = meshColor[2] / 255.0f; materialColor[3] = meshColor[3] / 255.0f;
		glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);

		// set the material shininess factor
		float shininess;
		shininess = 50.0f; //m_calHardwareModel->getShininess();
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);


		int boneId;
		for(boneId = 0; boneId < m_calHardwareModel->getBoneCount(); boneId++)
		{
			CalQuaternion rotationBoneSpace = m_calHardwareModel->getRotationBoneSpace(boneId, m_calModel->getSkeleton());
			CalVector translationBoneSpace = m_calHardwareModel->getTranslationBoneSpace(boneId, m_calModel->getSkeleton());

			CalMatrix rotationMatrix = rotationBoneSpace;

			float transformation[12];

			transformation[0]=rotationMatrix.dxdx;transformation[1]=rotationMatrix.dxdy;transformation[2]=rotationMatrix.dxdz;transformation[3]=translationBoneSpace.x;
			transformation[4]=rotationMatrix.dydx;transformation[5]=rotationMatrix.dydy;transformation[6]=rotationMatrix.dydz;transformation[7]=translationBoneSpace.y;
			transformation[8]=rotationMatrix.dzdx;transformation[9]=rotationMatrix.dzdy;transformation[10]=rotationMatrix.dzdz;transformation[11]=translationBoneSpace.z;


			glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB,boneId*3,&transformation[0]);
			glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB,boneId*3+1,&transformation[4]);
			glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB,boneId*3+2,&transformation[8]);

            // set the texture id we stored in the map user data
            glBindTexture(GL_TEXTURE_2D, (GLuint)m_calHardwareModel->getMapUserData(0));
		}

		if(sizeof(CalIndex)==2)
			glDrawElements(GL_TRIANGLES, m_calHardwareModel->getFaceCount() * 3, GL_UNSIGNED_SHORT, (((CalIndex *)NULL)+ m_calHardwareModel->getStartIndex()));
		else
			glDrawElements(GL_TRIANGLES, m_calHardwareModel->getFaceCount() * 3, GL_UNSIGNED_INT, (((CalIndex *)NULL)+ m_calHardwareModel->getStartIndex()));


	}

    // clear vertex array state

	glDisableVertexAttribArrayARB(0);
	glDisableVertexAttribArrayARB(1);
    glDisableVertexAttribArrayARB(2);
	glDisableVertexAttribArrayARB(3);
    glDisableVertexAttribArrayARB(8);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	//glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_VERTEX_PROGRAM_ARB);
	//glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, 0 );
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, 0 );
}

/**
 * \brief Renders Cal3DObject Skeleton using software rendering method
 *
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Cal3DObject::RenderModelSkeleton(const bool shadow)
{
	glEnable(GL_COLOR_MATERIAL);
	
	gluQuadricDrawStyle(quadric, (GLenum) GLU_FILL);
    gluQuadricNormals(quadric, (GLenum) GLU_SMOOTH);

	// draw the bone lines
	float lines[1024][2][3];
	int nrLines = m_calModel->getSkeleton()->getBoneLines(&lines[0][0][0]);
	static int _starting_index = ((nrLines > 3)?3:0);

	glLineWidth(1.0f);
	if (!shadow)
		glColor3f(1.0f, 1.0f, 0.0f);
	else
		glColor3f(0.0f, 0.0f, 0.0f);

	for(int currLine = _starting_index; currLine < nrLines; currLine++)
	{
		glBegin(GL_LINES);
			glVertex3f(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
			glVertex3f(lines[currLine][1][0], lines[currLine][1][1], lines[currLine][1][2]);
		glEnd();
        CalVector v, v1;
		v.x = lines[currLine][1][0]- lines[currLine][0][0];
        v.y = lines[currLine][1][1]- lines[currLine][0][1];
        v.z = lines[currLine][1][2]- lines[currLine][0][2];
		v1=v;
		float height = v.normalize();
		float angle = 0.0f;
		double _lenXY = sqrt(v.x*v.x+v.y*v.y); 
		if( _lenXY > 1)
			angle = 90.0f;
		else
			angle = (float)asin(_lenXY)/3.14159f*180.0f;
		if(v.z < 0.0f)
			angle = 180.0f-angle;

        // the rotation vector
        CalVector rot(v.y,-v.x,0.0f);

		GLdouble radius = 5;
		// the arrow (cone)
		glPushMatrix();
		glTranslatef(lines[currLine][0][0]+v1.x/2, lines[currLine][0][1]+v1.y/2, lines[currLine][0][2]+v1.z/2);
			glPushMatrix();
			  glRotatef(-angle,rot.x,rot.y,rot.z);
			  //gluCylinder(quadric,radius,0,v.length()*height,10,1);
			  glScalef(v.length()/3, v.length()/4, 1);
			  gluSphere(quadric, v.length()/2*height, 20, 20);
			glPopMatrix();
		glPopMatrix();
		
		//glPushMatrix();
		//glTranslatef(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
		////glScalef(len/_sx, len/_sy, 1);
		//gluSphere(quadric, len/5, 20, 20);
		//glPopMatrix();
		//float
		//glScalef(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
        //glTranslatef(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);

	}
	glLineWidth(1.0f);

	// draw the bone points
	float points[1024][3];
	int nrPoints = m_calModel->getSkeleton()->getBonePoints(&points[0][0]);

	glPointSize(4.0f);
	glBegin(GL_POINTS);
	if (!shadow)
		glColor3f(1.0f, 0.0f, 0.0f);
	else
		glColor3f(0.0f, 0.0f, 0.0f);
	for(int currPoint = _starting_index; currPoint < nrPoints; currPoint++)
	{
		glVertex3f(points[currPoint][0], points[currPoint][1], points[currPoint][2]);
	}
	//for(int currPoint = _starting_index; currPoint < nrPoints; currPoint++)
	//{
	//	glPushMatrix();
	//	glTranslatef(points[currPoint][0], points[currPoint][1], points[currPoint][2]);
	//	gluSphere(quadric, 10, 20, 20);
	//	glPopMatrix();
	//}
	glEnd();
	glPointSize(1.0f);

	glDisable(GL_COLOR_MATERIAL);

}

/**
 * \brief Renders Cal3DObject BoundingBox using software rendering method
 *
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Cal3DObject::RenderModelBoundingBox(const bool shadow)
{
	CalSkeleton *pCalSkeleton = m_calModel->getSkeleton();

	std::vector<CalBone*> &vectorCoreBone = pCalSkeleton->getVectorBone();

	glEnable(GL_COLOR_MATERIAL);
	if (!shadow)
		glColor3f(1.0f, 1.0f, 0.0f);
	else
		glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);

	for(size_t boneId=0;boneId<vectorCoreBone.size();++boneId)
	{
		CalBoundingBox & calBoundingBox  = vectorCoreBone[boneId]->getBoundingBox();

		CalVector p[8];
		calBoundingBox.computePoints(p);


		glVertex3f(p[0].x,p[0].y,p[0].z);
		glVertex3f(p[1].x,p[1].y,p[1].z);

		glVertex3f(p[0].x,p[0].y,p[0].z);
		glVertex3f(p[2].x,p[2].y,p[2].z);

		glVertex3f(p[1].x,p[1].y,p[1].z);
		glVertex3f(p[3].x,p[3].y,p[3].z);

		glVertex3f(p[2].x,p[2].y,p[2].z);
		glVertex3f(p[3].x,p[3].y,p[3].z);

		glVertex3f(p[4].x,p[4].y,p[4].z);
		glVertex3f(p[5].x,p[5].y,p[5].z);

		glVertex3f(p[4].x,p[4].y,p[4].z);
		glVertex3f(p[6].x,p[6].y,p[6].z);

		glVertex3f(p[5].x,p[5].y,p[5].z);
		glVertex3f(p[7].x,p[7].y,p[7].z);

		glVertex3f(p[6].x,p[6].y,p[6].z);
		glVertex3f(p[7].x,p[7].y,p[7].z);

		glVertex3f(p[0].x,p[0].y,p[0].z);
		glVertex3f(p[4].x,p[4].y,p[4].z);

		glVertex3f(p[1].x,p[1].y,p[1].z);
		glVertex3f(p[5].x,p[5].y,p[5].z);

		glVertex3f(p[2].x,p[2].y,p[2].z);
		glVertex3f(p[6].x,p[6].y,p[6].z);

		glVertex3f(p[3].x,p[3].y,p[3].z);
		glVertex3f(p[7].x,p[7].y,p[7].z);
	}

	glEnd();
	glDisable(GL_COLOR_MATERIAL);

}



/**
 * \brief Renders Cal3DObject Parameters using software rendering method
 **/
void Cal3DObject::RenderControlParameters()
{
	glEnable(GL_COLOR_MATERIAL);

	const char* _annote = getAnnotation().c_str();

	//OGLdrawCircleXZ(50,getPosition(), CalVector(1,1,1),20,false);

	//OGLdrawArcXZ(CalVector(50,0,50),getPosition(), CalVector(0,0,1),90,20,true);

	//OGLdrawArcXZ(CalVector(0,0,50), getDirection(),  getPosition(), CalVector(0,1,0), 0.5f, 20, true);

	OGLdraw2DTextAt3D(*_annote ,getPosition()+CalVector(1,100,1),CalVector(1,1,0) );

	glDisable(GL_COLOR_MATERIAL);
}

/**
 * \brief Sets level of details for rendering
 *
 * \param float level - level of details
 **/
void Cal3DObject::SetLodLevel(float level)
{
    if (m_calModel != NULL)
    {

        if (Debug::MODEL_LOADING>0)
            _dbg << "Cal3DObject<" << getID() <<"> SetLodLevel " << level << std::endl;

        m_calModel->setLodLevel(level);
    }
}


/**
 * \brief Allocates memory for vertex shader and load object data into accelerator memory
 *
 * \return bool - if allocation problems return false
 **/
bool Cal3DObject::loadBufferObject()
{

  float *pVertexBuffer = (float*)malloc(30000*3*sizeof(float));
  float *pWeightBuffer = (float*)malloc(30000*4*sizeof(float));
  float *pMatrixIndexBuffer = (float*)malloc(30000*4*sizeof(float));
  float *pNormalBuffer = (float*)malloc(30000*3*sizeof(float));
  float *pTexCoordBuffer = (float*)malloc(30000*2*sizeof(float));
  CalIndex *pIndexBuffer = (CalIndex*)malloc(50000*3*sizeof(CalIndex));

  if(pVertexBuffer==NULL || pWeightBuffer == NULL
	 || pMatrixIndexBuffer==NULL || pNormalBuffer == NULL
	 || pTexCoordBuffer==NULL || pIndexBuffer == NULL)
  {
	  return false;
  }


  m_calHardwareModel = new CalHardwareModel(m_calCoreModel);
  m_calHardwareModel->setVertexBuffer((char*)pVertexBuffer,3*sizeof(float));
  m_calHardwareModel->setNormalBuffer((char*)pNormalBuffer,3*sizeof(float));
  m_calHardwareModel->setWeightBuffer((char*)pWeightBuffer,4*sizeof(float));
  m_calHardwareModel->setMatrixIndexBuffer((char*)pMatrixIndexBuffer,4*sizeof(float));
  m_calHardwareModel->setTextureCoordNum(1);
  m_calHardwareModel->setTextureCoordBuffer(0,(char*)pTexCoordBuffer,2*sizeof(float));
  m_calHardwareModel->setIndexBuffer(pIndexBuffer);

  m_calHardwareModel->load( 0, 0, MAXBONESPERMESH);



  // the index index in pIndexBuffer are relative to the begining of the hardware mesh,
  // we make them relative to the begining of the buffer.

  int meshId;
  for(meshId = 0; meshId < m_calHardwareModel->getHardwareMeshCount(); meshId++)
  {
	  m_calHardwareModel->selectHardwareMesh(meshId);

	  int faceId;
	  for(faceId = 0; faceId < m_calHardwareModel->getFaceCount(); faceId++)
	  {
		  pIndexBuffer[faceId*3+ m_calHardwareModel->getStartIndex()]+=m_calHardwareModel->getBaseVertexIndex();
		  pIndexBuffer[faceId*3+1+ m_calHardwareModel->getStartIndex()]+=m_calHardwareModel->getBaseVertexIndex();
		  pIndexBuffer[faceId*3+2+ m_calHardwareModel->getStartIndex()]+=m_calHardwareModel->getBaseVertexIndex();
	  }

  }

  // We use ARB_vertex_buffer_object extension,
  // it provides better performance

  glGenBuffersARB(6, m_bufferObject);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[0]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*3*sizeof(float),(const void*)pVertexBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[1]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*4*sizeof(float),(const void*)pWeightBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[2]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*3*sizeof(float),(const void*)pNormalBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[3]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*4*sizeof(float),(const void*)pMatrixIndexBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_bufferObject[4]);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalVertexCount()*2*sizeof(float),(const void*)pTexCoordBuffer, GL_STATIC_DRAW_ARB);

  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferObject[5]);

  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_calHardwareModel->getTotalFaceCount()*3*sizeof(CalIndex),(const void*)pIndexBuffer, GL_STATIC_DRAW_ARB);

  free(pVertexBuffer);
  free(pWeightBuffer);
  free(pNormalBuffer);
  free(pMatrixIndexBuffer);
  free(pIndexBuffer);

  return true;

}
