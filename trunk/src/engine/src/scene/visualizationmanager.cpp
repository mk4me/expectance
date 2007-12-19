/*
 * Copyright (c) 2007, Future Today. All rights reserved.
 * author: abak, mka
 */

#include "visualizationmanager.h"

using namespace ft;

VisualizationManager* VisualizationManager::m_instance = NULL;

VisualizationManager* VisualizationManager::getInstance()
{
    if (m_instance == NULL)
    {
        _dbg << "VisualizationManager::getInstace(): instance of VisualizationManager created " << endl;
        m_instance = new VisualizationManager();
		
		TextureManager::getInstance();  // enforced creation of singleton
		CameraManager::getInstance();			// enforced creation of singleton
    }

    return m_instance;
}

VisualizationManager* VisualizationManager::createInstanceFirstTime()
{
    if (m_instance == NULL)
    {
        m_instance = new VisualizationManager();
    }
    else
    {
        if (Debug::ERR)
            _dbg << "VisualizationManager::createInstanceFirstTime(): instance alraedy created!!!! " << endl;
    }
    return m_instance;
}

void VisualizationManager::DestroyInstance()
{
    if (m_instance != NULL)
    {
	    m_instance->m_SceneObjects.clear();

		std::map<std::string,SceneObject*>::iterator it = m_instance->m_DataObjects.begin();
		for( ; it != m_instance->m_DataObjects.end(); ++it ) {
			delete it->second;
	    }
	    m_instance->m_DataObjects.clear();

        delete m_instance;
    }
}

bool VisualizationManager::Init()
{
	if (!CameraManager::getInstance()->Init())
	{
		return false;
	}

	if (!OGLContext::getInstance()->Init())
	{
		return false;
	}

	if (!InitSceneObjects()) 
	{
		return false;
	}

	if (!MenuManager::getInstance()->Init(0,0))
	{
		return false;
	}
	
	if(Config::getInstance()->IsKey("camera_marker_visible"))
	{
		m_ActiveCameraMarker = Config::getInstance()->GetIntVal("camera_marker_visible");
	}
	else
		m_ActiveCameraMarker = 0; // no active camera marker visible

	return true;
}


bool VisualizationManager::InitSceneObjects()
{

	OGLContext::getInstance()->InitNormalFloorDL(20);
	if (!OGLContext::getInstance()->InitTexturedFloorDL(20)) return false;
	if (!OGLContext::getInstance()->InitLogoDL()) return false;

	return true;
}


void VisualizationManager::OnRender()
{
	
	// Scene ViewPort Render Pipeline SVPR
	OGLContext::getInstance()->setSceneViewport( CameraManager::getInstance()->IsZoom() ); 
	CameraManager::getInstance()->UpdateView(); //update current camera View 
	
	OGLContext::getInstance()->DrawSceneViewPortPrimitives(); 
	if (m_ActiveCameraMarker)
		CameraManager::getInstance()->RenderCurrentCamera(); //show avatar's camera position
	Render3DSceneObjects();
	Render2DSceneObjects();
	
	// DataViewPort Render Pipeline
	// place for DVPR
	if (OGLContext::getInstance()->DATA_VIEWPORT)
	{
		OGLContext::getInstance()->setDataViewport();
		OGLContext::getInstance()->DrawDataViewPortPrimitives(); 
		RenderDataObjects();
        DrawLegend();
	}
	// swap the front- and back-buffer
	glutSwapBuffers();
	// increase frame counter
	ft::UpdateManager::getInstance()->increraseFramesCounter();
}

void VisualizationManager::DrawLegend()
{
    glPushMatrix();
  	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    OGLContext* ctx = OGLContext::getInstance();
    
    float dvpHeight = ctx->getDataViewportHeight();
    float dvpWidth = ctx->getDataViewportWidth();

    int text_height = 15;
    int y = dvpHeight - text_height;
    int text_x = dvpWidth/2+4;

    string legend = "Legend:";
	glEnable(GL_LINE_SMOOTH);
	glColor4f(1,1,1,0.8f);
	OGLContext::getInstance()->OGLWriteBitmap(1, text_x, y, legend.c_str());
	glDisable(GL_LINE_SMOOTH);

    
    int line_width = 30;
    int line_start = text_x;
    text_x += line_width + 4;
    
    DataCollector* pObj;
  	std::map<std::string,SceneObject*>::iterator it=m_DataObjects.begin();
	for( ; it != m_DataObjects.end(); ++it ) {
		if ((pObj = dynamic_cast<DataCollector*>(it->second))!=NULL)
		{
			if (pObj->isVisible())
			{
                y -= text_height;

                CalVector vColor = pObj->getColor();
    		    glColor4f(vColor.x,vColor.y,vColor.z,1);
                glBegin(GL_LINES);
                glVertex2d(line_start,y); glVertex2d(line_start+line_width,y);
                glEnd();

                string curveDesc =pObj->getLegendLabel();
	            glEnable(GL_LINE_SMOOTH);
	            glColor4f(1,1,1,0.5f);
	            OGLContext::getInstance()->OGLWriteBitmap(1, text_x, y-3, curveDesc.c_str());
	            glDisable(GL_LINE_SMOOTH);
			}
		}
	}

    glPopMatrix();
}

void VisualizationManager::CleanUp()
{
	TextureManager::getInstance()->DestroyInstance();	
}


bool VisualizationManager::IsObjectTraceableByCamera(SceneObject* pObj)
{
    return (dynamic_cast<SceneObject*>(pObj)!=NULL);
}

bool VisualizationManager::AddObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
    bool result = false;

	if (!_id.empty())
	{
		result = true; //assume it's new element
		std::list<SceneObject*>::iterator _iteratorRObj = m_SceneObjects.begin();
		while(_iteratorRObj != m_SceneObjects.end())
		{
			if((*_iteratorRObj) == pObj)
			{
				// found, so don't add it
				result = false;
				break;
			}
			_iteratorRObj++;
		}

		if (result) // for new object
		{
		    m_SceneObjects.push_front( pObj );
			//m_SceneObjects.sort(greater<PSO>());
			//std::sort(m_SceneObjects.begin(), m_SceneObjects.end(), CompareSceneObjects());   // sort ascending
			
			if (IsObjectTraceableByCamera(pObj))
			{
				CameraManager::getInstance()->AddCamera(pObj); // add camera by default to Avatar object only
				CameraManager::getInstance()->setCurrentSceneObjectID(_id);   // set current scene object ID to last added scene object
			}
		}

	}
	return result;
}  

bool VisualizationManager::AddDataObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
	if (!_id.empty())
	{
	 	std::map<std::string,SceneObject*>::iterator it = m_DataObjects.find(_id);
		if ( it!=m_DataObjects.end()) { 
			return false;
		}
	    m_DataObjects.insert( std::make_pair( std::string(_id), pObj ) );
	}
	return true;
}  

bool VisualizationManager::RemoveObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
    bool result = false;
	if (!_id.empty())
	{
		CameraManager::getInstance()->RemoveCamera(_id); // remove pointer to camera from erased object

		std::list<SceneObject*>::iterator _iteratorRObj = m_SceneObjects.begin();
		while(_iteratorRObj != m_SceneObjects.end())
		{
			if((*_iteratorRObj) == pObj)
			{
				// found, so remove list element
				m_SceneObjects.erase(_iteratorRObj);
				result = true;
				break;
			}
			_iteratorRObj++;
		}
	}
	return result;
}

bool VisualizationManager::RemoveDataObject(SceneObject* pObj)
{
	std::string _id = pObj->getID();
	bool result = false;	
	if (!_id.empty())
	{
		result = RemoveDataObject(_id);
	}
	return result;
}

bool VisualizationManager::RemoveDataObject(std::string id)
{
 	std::map<std::string,SceneObject*>::iterator it = m_DataObjects.find(id);
	if ( it!=m_DataObjects.end()) 
	{ 
		//delete it->second; //scene manager does it
		m_DataObjects.erase(it);
		return true;
	}
	return false;
}

void VisualizationManager::Render3DSceneObjects()
{
	SceneObject *pObj;
	// iterate through the objects and render shadows
	std::list<SceneObject*>::iterator it=m_SceneObjects.begin();
	for( ; it != m_SceneObjects.end(); ++it ) {
		if ((pObj = (*it))!=NULL)
		{
			if (pObj->isVisible())
			{
				pObj->RenderShadow();
			}
		}
	}
	

	// iterate through the ordered list of objects (tracers) and render
	it=m_SceneObjects.begin();
	for( ; it != m_SceneObjects.end(); ++it ) {
		if ((pObj = (*it))!=NULL)
		{
			if (pObj->isVisible())
					pObj->Render();
		}
	}
}

void VisualizationManager::Render2DSceneObjects()
{
	if ((OGLContext::getInstance()->IsLogoFTActive())||(MenuManager::getInstance()->IsMenuVisible()))
	{
		OGLContext::getInstance()->GLOrtho2DCorrection();

		//2D context for rendering
		MenuManager::getInstance()->Render();
		OGLContext::getInstance()->RenderLogo();
	}

}

void VisualizationManager::RenderDataObjects()
{
	DataCollector *pObj;
	float _min=0, _max=0;
	bool _one = false;
	// iterate through the objects and find max and min values
	std::map<std::string,SceneObject*>::iterator it=m_DataObjects.begin();
	for( ; it != m_DataObjects.end(); ++it ) {
		if ((pObj = dynamic_cast<DataCollector*>(it->second))!=NULL)
		{
			if (_one) 
			{
				if ( _max < pObj->getMax() ) _max = pObj->getMax();
				if ( _min > pObj->getMin() ) _min = pObj->getMin();
				_one = true;
			}
			else
			{
				_min = pObj->getMin();
				_max = pObj->getMax();
			}
		}
	}
	OGLContext::getInstance()->SetDataViewportLegend(_min, _max);

	
	it=m_DataObjects.begin();
	for( ; it != m_DataObjects.end(); ++it ) {
		if ((pObj = dynamic_cast<DataCollector*>(it->second))!=NULL)
		{
			if (pObj->isVisible())
			{
				pObj->Render();
			}
		}
	}

}

