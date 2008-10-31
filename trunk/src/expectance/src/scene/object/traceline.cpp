#include "traceline.h"

#include <osg/Hint>
#include <osg/PositionAttitudeTransform>
#include <osg/BlendFunc>
#include <iostream>

using namespace ft;
using namespace std;


TraceLine::TraceLine(void): 
m_bufferSize(100)
{
	m_color = osg::Vec4( 1, 1, 1, 1 );
    m_tracerVertices = new osg::Vec3Array;
    m_tracerColors = new osg::Vec4Array;
    m_tracerColors->push_back(m_color);
	//m_tracerVertices->push_back( osg::Vec3( 0.0, 0.0, 0.0 ));
	//m_tracerVertices->push_back( osg::Vec3( 100.0, 100.0, 100.0 ));
    m_tracerGeometry = new osg::Geometry;
	m_tracerDrawArrays = new osg::DrawArrays;
	m_tracerGeode = new osg::Geode;
	m_tracerVertexBufferObject = new osg::VertexBufferObject;

	m_tracerGeometry.get()->setVertexArray(m_tracerVertices.get());
	m_tracerGeometry.get()->setColorBinding(osg::Geometry::BIND_OVERALL);
	m_tracerGeometry.get()->setColorArray(m_tracerColors.get());
	m_tracerDrawArrays.get()->setMode(osg::PrimitiveSet::LINE_STRIP);
	m_tracerGeometry.get ()->addPrimitiveSet(m_tracerDrawArrays.get());

	m_tracerDrawArrays.get()->setCount( m_tracerVertices.get()->size() );

	m_tracerVertices.get ()->setVertexBufferObject(m_tracerVertexBufferObject.get());

	m_tracerGeometry.get()->setUseDisplayList(false);

	m_tracerVertexBufferObject->dirty();

	m_tracerGeometry.get()->dirtyDisplayList();
	m_tracerGeometry.get()->dirtyBound();
	
	m_tracerGeode.get()->addDrawable(m_tracerGeometry.get());
	m_tracerStateSet = new osg::StateSet();
	osg::ref_ptr<osg::Hint> antialiasHint = new	osg::Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	// Additionally but not necessary Turn on blending
	osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
	//osg::Hint(GL_LINE_WIDTH, 4.0f);

    m_tracerStateSet->setAttributeAndModes(blendFunc.get(), osg::StateAttribute::ON); 
    m_tracerStateSet->setAttributeAndModes(antialiasHint.get(), osg::StateAttribute::ON); 
	m_tracerStateSet->setMode( GL_LIGHTING, osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF );
	m_tracerGeode.get()->setStateSet(m_tracerStateSet.get());
	m_tracerGeometry->setUpdateCallback( new LineChange(this) );
	this->addChild(m_tracerGeode.get());
	WorldManager::getInstance()->AddToScene(this);
}

void TraceLine::Update(void)
{
	// update data for geometry primitive (you can do it as well in AddPoint but here seems to work more effectively)
	m_tracerGeometry->setVertexArray(m_tracerVertices.get());
	m_tracerDrawArrays->setCount( m_tracerVertices.get()->size() );	
	m_tracerGeometry->dirtyDisplayList();
	m_tracerGeometry->dirtyBound();
}

void TraceLine::setLineColor(const osg::Vec4 color)
{
	m_color = color;
	(*m_tracerColors)[0] = m_color;
}
void TraceLine::AddPoint(const osg::Vec3d &point)
{
	if (point.ptr()!= NULL)
	{
		m_tracerVertices.get()->push_back(point);
		unsigned long _size = m_tracerVertices.get()->size();

		//remove points when buffer is too long
		if ((m_bufferSize != 0)&&(_size > m_bufferSize))
			m_tracerVertices.get()->erase(m_tracerVertices.get()->begin());
	}
}