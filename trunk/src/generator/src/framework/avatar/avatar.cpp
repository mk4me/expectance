/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: abak, mka
 */

#include "avatar.h"
#include "../utility/debug.h"

using namespace ft;

Avatar::Avatar(CalModel* calModel, CalCoreModel* calCoreModel, const std::string modelName):m_renderMethod(0)
{
    m_calModel = calModel;
    m_calCoreModel = calCoreModel;
	setID(modelName);
}

Avatar::Avatar(CalModel* calModel, CalCoreModel* calCoreModel)
{
    m_calModel = calModel;
    m_calCoreModel = calCoreModel;
}

void Avatar::SetCalModel(CalModel* calModel)
{
    m_calModel = calModel;
}

CalModel* Avatar::GetCalModel()
{
    return m_calModel;
}

void Avatar::SetCalCoreModel(CalCoreModel* calCoreModel)
{
    m_calCoreModel = calCoreModel;
}

CalCoreModel* Avatar::GetCalCoreModel()
{
    return m_calCoreModel;
}

void Avatar::OnMessage(Message& msg)
{
    DBG("Avatar::OnMessage: received");
}

void Avatar::OnUpdate(float elapsedTime)
{

}

bool Avatar::Render()
{
	glPushMatrix();
	glRotatef(-90,1.0f,0.0f,0.0f); //must rotate because of 3dmax model representation ZYX to OGL XYZ
	// set global OpenGL states
	if (!m_shadow)
	{
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	m_calModel->getSkeleton()->calculateBoundingBoxes();

	if (m_renderMethod == 0) 
		RenderModelMesh(m_shadow);
	else if (m_renderMethod == 1) 
		RenderModelSkeleton(m_shadow);
	else if (m_renderMethod == 2)
		RenderModelBoundingBox(m_shadow);

	// clear light
	if (!m_shadow)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
	m_shadow= !m_shadow; // temporary hack
	return true;
}

void Avatar::setRenderMethod(const int renderMethod)
{
	m_renderMethod = renderMethod;
}




void Avatar::RenderModelMesh(bool shadow)
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
void Avatar::RenderModelSkeleton(bool shadow)
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
void Avatar::RenderModelBoundingBox(bool shadow)
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