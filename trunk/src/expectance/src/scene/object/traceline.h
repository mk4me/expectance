/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 */
#ifndef _TRACE_LINE_H
#define _TRACE_LINE_H

#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>


namespace ft
{
	class TraceLine: public osg::Group
	{
	public:
		TraceLine(void);
		void updateTrackingInformation(void);
	protected:
		~TraceLine(void){};

	private:
		osg::ref_ptr<osg::Geode> m_tracerGeode;
		osg::ref_ptr<osg::Geometry> m_tracerGeometry;
		osg::ref_ptr<osg::DrawArrays> m_tracerDrawArrays;
		osg::ref_ptr<osg::Vec3Array> m_tracerVertices;
		osg::ref_ptr<osg::Vec4Array> m_tracerColors;
		osg::ref_ptr<osg::VertexBufferObject> m_tracerVertexBufferObject;
		osg::ref_ptr<osg::StateSet> m_tracerStateSet;

		osg::Vec3d m_currentValue, m_deltaValue;
		unsigned int m_bufferSize;
	};

	class LineChange : public osg::Drawable::UpdateCallback
	{
    public:
        LineChange(TraceLine* pat) {m_pat = pat;}

        virtual void update(osg::NodeVisitor*nv, osg::Drawable* dbl) 
        {
			if(m_pat!=NULL)
			{
	            const osg::FrameStamp *fs = nv->getFrameStamp();
		        if( fs != 0L )
			    {
					m_pat->updateTrackingInformation();
				}
            }
        }
    private:
		TraceLine* m_pat;
	};

}
#endif //TRACE_LINE