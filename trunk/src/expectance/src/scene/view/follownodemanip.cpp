#include "follownodemanip.h"

followNodeMatrixManipulator::followNodeMatrixManipulator( transformAccumulator* ta)
{
   worldCoordinatesOfNode = ta;
   theMatrix = osg::Matrixd::identity();
}

void followNodeMatrixManipulator::setByMatrix(const osg::Matrixd& mat)
{
   theMatrix = mat;
}
void followNodeMatrixManipulator::setByInverseMatrix(const osg::Matrixd& mat)
{
   theMatrix = mat.inverse(mat);
}
void followNodeMatrixManipulator::updateTheMatrix()
{
   theMatrix = worldCoordinatesOfNode->getMatrix();
   
   //osg::Vec3f softTransform = theMatrix.getTrans();
   //softTransform.z() = -100.0;
   //theMatrix.setTrans(softTransform);
}
osg::Matrixd followNodeMatrixManipulator::getInverseMatrix() const
{
   osg::Matrixd m;
   m = theMatrix * osg::Matrixd::rotate(-3.14159265f/2.0, osg::Vec3(1,0,0) );
   return m;
}

//void followNodeMatrixManipulator::setTransformAccumulator(transformAccumulator* ta)
//{
//   worldCoordinatesOfNode = ta;
//   theMatrix = osg::Matrixd::identity();
//}

osg::Matrixd followNodeMatrixManipulator::getMatrix() const
{
   return theMatrix;
}

bool followNodeMatrixManipulator::handle(const osgGA::GUIEventAdapter&ea, osgGA::GUIActionAdapter&aa)
{
   switch(ea.getEventType())
   {
   case (osgGA::GUIEventAdapter::FRAME):
      {
         updateTheMatrix();
         return false;
      }
   }
   return false;
}
