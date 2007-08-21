/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "avatar.h"
#include "../utility/debug.h"

using namespace ft;

/**
 * \brief Constructor for avatar
 *
 * \param CalModel * calModel - model in Cal3d which is referenced by this avatar
 * \param CalCoreModel * calCoreModel - type defined in Cal3d that has been used to create CalModel for this avatar
 * \param const std::string modelName - name defined in ft::BaseObject which is the base class for Avatar
 **/
Avatar::Avatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName):
m_renderMethod(0), 
m_shadow(true)
{
    m_calModel = calModel;
    m_calCoreModel = calCoreModel;
	setName(modelName);
	if (OGLContext::getInstance()->HardwareAcceleration == 1) //try to check Vertex Shader in the hardware
	{
		m_hardwareRendering = CheckHardwareAcceleration();
	}
	else
		m_hardwareRendering = false;
}

/**
 * \brief Destructor
 *
 **/
Avatar::~Avatar()
{
	delete m_calHardwareModel;
	delete m_calModel;
	delete m_calCoreModel;
	
	if (m_hardwareRendering)
	{
		glDeleteProgramsARB(1, &m_vertexProgramId);
		glDeleteBuffersARB(6, m_bufferObject);	
	}
}

/**
 * \brief Tries to reserve memory for avatar data in graphics accelerator memory (if graphic accelerator supports vertex shader)
 *
 * \return bool - true if graphics accelerator suports vertex shader otherwise returns false
 **/
bool Avatar::CheckHardwareAcceleration()
{

	if(!loadBufferObject())
	{
	  std::cerr << "Error loading vertex buffer object." << std::endl;
	  return false;
	}

	if(!loadVertexProgram())
	{
	  std::cerr << "Error loading vertex program." << std::endl;
	  return false;
	}

	return true;
}

/**
 * \brief Sets model in Cal3d which is referenced by this avatar
 *
 * \param CalModel * calModel - Cal3d object
 **/
void Avatar::SetCalModel(CalModel* calModel)
{
    m_calModel = calModel;
}

/**
 * \brief Returns model from Cal3d which is referenced by this avatar
 *
 * \return CalModel * Cal3d model
 **/
CalModel* Avatar::GetCalModel()
{
    return m_calModel;
}

/**
 * \brief Sets object representing type defined in Cal3d that is used to create CalModel for this avatar
 *
 * \param CalCoreModel * calCoreModel - object representing type of CalModel
 **/
void Avatar::SetCalCoreModel(CalCoreModel* calCoreModel)
{
    m_calCoreModel = calCoreModel;
}

/**
 * \brief Returns object representing type defined in Cal3d that is used to create CalModel for this avatar
 *
 * \return CalCoreModel * - object representing type of CalModel
 **/
CalCoreModel* Avatar::GetCalCoreModel()
{
    return m_calCoreModel;
}

/**
 * \brief This method is called when message is sent to this object from ft::UpdateManager
 *
 * \param ft::Message * msg - message from ft::UpdateManager
 **/
void Avatar::OnMessage(Message* msg)
{
    if (DEBUG_MESSAGES)
        std::cout << toString() << " received message: " << Message::_GET_MSG_NAME(msg->getType()) << std::endl;

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
 * \brief Main rendering routine for Avatar
 **/
bool Avatar::Render()
{
	if (m_shadow) //draw the shadow
	{	
		glPushMatrix();
			OGLContext::getInstance()->GlShadowProjection();
			glColor4f(0.0f,0.0f,0.0f,0.0f);
			//glRotatef(-90,1.0f,0.0f,0.0f); //unnecessary while XYZ order in cal files
			RenderAvatar(m_renderMethod, m_shadow); 	
		glPopMatrix();
	}
	
	// draw the object that casts the shadow

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//
	glPushMatrix();
		//glRotatef(-90,1.0f,0.0f,0.0f); //unnecessary while XYZ order in cal files
		RenderAvatar(m_renderMethod, false);
	glPopMatrix();

	// clear light
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);


	return true;
}

/**
 * \brief Changes between three rendering methods: Mesh Skeleton and BoundingBox consecutivevely  
 *
 **/
void Avatar::ChangeRenderMethod()
{
    m_renderMethod = (m_renderMethod+1) % 3;
}

/**
 * \brief Sets between three rendering methods: Mesh Skeleton and BoundingBox
 *
 * \param const int renderMethod: 0 - Mesh, 1 - Skeleton, 2 - BoundingBox
 **/
void Avatar::setRenderMethod(const int renderMethod)
{
	m_renderMethod = renderMethod;
}

/**
 * \brief Switches shadow on when it is off or switches shadow off when it is on.
 *
 **/
void Avatar::ChangeShadow()
{
    m_shadow = !m_shadow;
}

/**
 * \brief Switches the shadow on or off.
 *
 * \param const bool - defines if shadow should be swithced on or off
 **/
void Avatar::setShadow(const bool shadow)
{
	m_shadow = shadow;
}

/**
 * \brief Renders Avatar taking into consideration specific parameters 
 *
 * \param const int renderMethod: 0 - Mesh, 1 - Skeleton, 2 - BoundingBox
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Avatar::RenderAvatar(const int renderMethod, const bool shadow)
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
 * \brief Renders Avatar Mesh using software rendering method
 *
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Avatar::SoftwareRenderModelMesh(const bool shadow)
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
 * \brief Renders Avatar Mesh using hardware rendering method
 *
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Avatar::HardwareRenderModelMesh(const bool shadow)
{
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_vertexProgramId );

	glEnableVertexAttribArrayARB(0);
	glEnableVertexAttribArrayARB(1);
    glEnableVertexAttribArrayARB(2);
	glEnableVertexAttribArrayARB(3);
    glEnableVertexAttribArrayARB(8);

	glEnable(GL_VERTEX_PROGRAM_ARB);	

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

	glDisable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, 0 );
}

/**
 * \brief Renders Avatar Skeleton using software rendering method
 *
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Avatar::RenderModelSkeleton(const bool shadow)
{
	glEnable(GL_COLOR_MATERIAL);

	// draw the bone lines
	float lines[1024][2][3];
	int nrLines = m_calModel->getSkeleton()->getBoneLines(&lines[0][0][0]);

	glLineWidth(3.0f);
	if (!shadow)
		glColor3f(1.0f, 1.0f, 0.0f);
	else 
		glColor3f(0.0f, 0.0f, 0.0f);

	glBegin(GL_LINES);
	for(int currLine = 0; currLine < nrLines; currLine++)
	{
		glVertex3f(lines[currLine][0][0], lines[currLine][0][1], lines[currLine][0][2]);
		glVertex3f(lines[currLine][1][0], lines[currLine][1][1], lines[currLine][1][2]);
	}
	glEnd();
	glLineWidth(1.0f);

	// draw the bone points
	float points[1024][3];
	int nrPoints = m_calModel->getSkeleton()->getBonePoints(&points[0][0]);

	glPointSize(4.0f);
	glBegin(GL_POINTS);
	if (!shadow)
		glColor3f(0.0f, 0.0f, 1.0f);
	else
		glColor3f(0.0f, 0.0f, 0.0f);
	for(int currPoint = 0; currPoint < nrPoints; currPoint++)
	{
		glVertex3f(points[currPoint][0], points[currPoint][1], points[currPoint][2]);
	}
	glEnd();
	glPointSize(1.0f);

	glDisable(GL_COLOR_MATERIAL);

}

/**
 * \brief Renders Avatar BoundingBox using software rendering method
 *
 * \param const bool shadow: true - render with shadow, otherwise without shadow
 **/
void Avatar::RenderModelBoundingBox(const bool shadow)
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
 * \brief Sets level of details for rendering
 *
 * \param float level - level of details
 **/
void Avatar::SetLodLevel(float level)
{
    if (m_calModel != NULL)
    {
        std::cout << "Avatar<" << getID() <<"> SetLodLevel " << level << std::endl;
        m_calModel->setLodLevel(level);
    }
}


char vertexProgramStr[]= 
"!!ARBvp1.0\n"\
"PARAM constant = { 1, 3, 0, 0 };\n"\
"TEMP R0, R1, R2, R3, R4, R5;\n"\
"ADDRESS A0;\n"\
"ATTRIB texCoord = vertex.attrib[8];\n"\
"ATTRIB normal = vertex.attrib[2];\n"\
"ATTRIB index = vertex.attrib[3];\n"\
"ATTRIB weight = vertex.attrib[1];\n"\
"ATTRIB position = vertex.attrib[0];\n"\
"PARAM worldViewProjMatrix[4] = { state.matrix.mvp };\n"\
"PARAM diffuse = state.material.diffuse;\n"\
"PARAM ambient = state.material.ambient;\n"\
"PARAM lightDir = state.light[0].position;\n"\
"PARAM matrix[87] = { program.local[0..86] };\n"\
"\n"\
"MOV result.texcoord[0].xy, texCoord.xyxx;	\n"\
"\n"\
"MUL R4, index, constant.y;	\n"\
"\n"\
"ARL A0.x, R4.y;\n"\
"DP3 R0.x, matrix[A0.x].xyzx, normal.xyzx;\n"\
"DP3 R0.y, matrix[A0.x + 1].xyzx, normal.xyzx;\n"\
"DP3 R0.z, matrix[A0.x + 2].xyzx, normal.xyzx;\n"\
"MUL R1.yzw, R0.xxyz, weight.y;\n"\
"\n"\
"ARL A0.x, R4.x;\n"\
"DP3 R0.x, matrix[A0.x].xyzx, normal.xyzx;\n"\
"DP3 R0.y, matrix[A0.x + 1].xyzx, normal.xyzx;\n"\
"DP3 R0.z, matrix[A0.x + 2].xyzx, normal.xyzx;\n"\
"MAD R1.yzw, R0.xxyz, weight.x, R1.yyzw;\n"\
"\n"\
"DP3 R0.x, R1.yzwy, R1.yzwy;\n"\
"RSQ R0.x, R0.x;\n"\
"MUL R0.xyz, R0.x, R1.yzwy;\n"\
"DP3 R1.x, lightDir.xyzx, lightDir.xyzx;\n"\
"RSQ R1.x, R1.x;\n"\
"MUL R2.xyz, R1.x, lightDir.xyzx;\n"\
"DP3 R0.x, R0.xyzx, R2.xyzx;\n"\
"MAX R0.x, R0.x, constant.z;\n"\
"ADD R0, R0.x, ambient;\n"\
"MUL result.color.front.primary, R0, diffuse;\n"\
"\n"\
"ARL A0.x, R4.w;\n"\
"DPH R0.x, position.xyzx, matrix[A0.x];\n"\
"DPH R0.y, position.xyzx, matrix[A0.x + 1];\n"\
"DPH R0.z, position.xyzx, matrix[A0.x + 2];\n"\
"\n"\
"ARL A0.x, R4.z;\n"\
"DPH R3.x, position.xyzx, matrix[A0.x];\n"\
"DPH R3.y, position.xyzx, matrix[A0.x + 1];\n"\
"DPH R3.z, position.xyzx, matrix[A0.x + 2];\n"\
"\n"\
"ARL A0.x, R4.y;\n"\
"DPH R1.y, position.xyzx, matrix[A0.x];\n"\
"DPH R1.z, position.xyzx, matrix[A0.x + 1];\n"\
"DPH R1.w, position.xyzx, matrix[A0.x + 2];\n"\
"MUL R2.xyz, R1.yzwy, weight.y;\n"\
"\n"\
"ARL A0.x, R4.x;\n"\
"DPH R1.x, position.xyzx, matrix[A0.x];\n"\
"DPH R1.y, position.xyzx, matrix[A0.x + 1];\n"\
"DPH R1.z, position.xyzx, matrix[A0.x + 2];\n"\
"\n"\
"MAD R1.xyz, R1.xyzx, weight.x, R2.xyzx;\n"\
"MAD R1.xyz, R3.xyzx, weight.z, R1.xyzx;\n"\
"MAD R0.xyz, R0.xyzx, weight.w, R1.xyzx;\n"\
"\n"\
"DPH result.position.x, R0.xyzx, worldViewProjMatrix[0];\n"\
"DPH result.position.y, R0.xyzx, worldViewProjMatrix[1];\n"\
"DPH result.position.z, R0.xyzx, worldViewProjMatrix[2];\n"\
"DPH result.position.w, R0.xyzx, worldViewProjMatrix[3];\n"\
"END\n";

/**
 * \brief Allocates memory for vertex shader and load avatar data into accelerator memory
 *
 * \return bool - if allocation problems return false
 **/
bool Avatar::loadBufferObject()
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


/**
 * \brief Tries to prepare buffer in graphics accelerator for vertex rendering purpose
 *
 * \return bool - if not supported false
 **/
bool Avatar::loadVertexProgram()
{
	glGenProgramsARB( 1, &m_vertexProgramId );
	
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, m_vertexProgramId );
	
	glProgramStringARB( GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
		strlen(vertexProgramStr), vertexProgramStr );
	
	if ( GL_INVALID_OPERATION == glGetError() )
	{
		// Find the error position
		GLint errPos;
		glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB,
			&errPos );
		// Print implementation-dependent program
		// errors and warnings string.
		const unsigned char *errString = glGetString( GL_PROGRAM_ERROR_STRING_ARB);
		fprintf( stderr, "error at position: %d\n%s\n",
			errPos, errString );
		return false;
	}
	
	
	glBindProgramARB( GL_VERTEX_PROGRAM_ARB, 0 );

	return true;	
}
