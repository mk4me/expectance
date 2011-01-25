/*
 * Copyright (c) 2008, FutureToday. All rights reserved.
 * author: mka
 */
#ifndef _OTHER_SHAPES_H
#define _OTHER_SHAPES_H

#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/BlendFunc>
#include <osg/AlphaFunc>


namespace ft
{
	
	osg::Node* createFloor(const osg::Vec3& center,float radius)
	{

	   
		int numTilesX = 10;
		int numTilesY = 10;
	   
		float width = 2*radius;
		float height = 2*radius;
	   
		osg::Vec3 v000(center - osg::Vec3(width*0.5f,height*0.5f,0.0f));
		osg::Vec3 dx(osg::Vec3(width/((float)numTilesX),0.0,0.0f));
		osg::Vec3 dy(osg::Vec3(0.0f,height/((float)numTilesY),0.0f));
	   
		// fill in vertices for grid, note numTilesX+1 * numTilesY+1...
		osg::Vec3Array* coords = new osg::Vec3Array;
		int iy;
		for(iy=0;iy<=numTilesY;++iy)
		{
			for(int ix=0;ix<=numTilesX;++ix)
			{
				coords->push_back(v000+dx*(float)ix+dy*(float)iy);
			}
		}
	   
		//colors when textures doesn't work
		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1,1,1,1)); //(0.245f,0.245f,0.245f,1.0f)); // gray

	   
		int numIndicesPerRow=numTilesX+1;
		osg::UByteArray* coordIndices = new osg::UByteArray; // assumes we are using less than 256 points...
		osg::UByteArray* textureIndices = new osg::UByteArray;
		osg::ref_ptr<osg::Vec2Array> texCoords (new osg::Vec2Array());
		for(iy=0;iy<numTilesY;++iy)
		{
			for(int ix=0;ix<numTilesX;++ix)
			{
				// four vertices per quad.
				coordIndices->push_back(ix    +(iy+1)*numIndicesPerRow);
				coordIndices->push_back(ix    +iy*numIndicesPerRow);
				coordIndices->push_back((ix+1)+iy*numIndicesPerRow);
				coordIndices->push_back((ix+1)+(iy+1)*numIndicesPerRow);
	           
				// one texture per quad
				//textureIndices->push_back(ix+iy);
				
				texCoords->push_back (osg::Vec2 (0.0f, 0.0f));
				texCoords->push_back (osg::Vec2 (0.0f, 1.0f));
				texCoords->push_back (osg::Vec2 (1.0f, 1.0f));
				texCoords->push_back (osg::Vec2 (1.0f, 0.0f));
			}

		}
	   

		// set up a single normal
		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
	   

		osg::Geometry* geom = new osg::Geometry;
		geom->setVertexArray(coords);
		geom->setVertexIndices(coordIndices);
		geom->setTexCoordArray (0, texCoords.get());
		geom->setColorArray(colors);
		geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
	   
		geom->setNormalArray(normals);
		geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	   
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,coordIndices->size()));
	   
		osg::Geode* geode = new osg::Geode;
		geode->addDrawable(geom);
		
		// load an image by reading a file: 
		osg::ref_ptr<osg::Image> image = osgDB::readImageFile(FT_DATA_PATH + "textures/floorhex3.tga");
		if (!image)
		{
		   std::cout << " couldn't find texture, quitting." << std::endl;
		   exit (EXIT_FAILURE);
		}
		else
		{
			//_floorTexture->setBorderWidth(0.8f);
			// texture in stateset
			osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
			tex->setImage(image.get());
			// After creating the OpenGL texture object, release the internal ref_ptr<Image> (to delete the Image).
			tex->setUnRefImageDataAfterApply( true );
			osg::StateSet* state = new osg::StateSet();
			// Assign texture unit 0 of our new StateSet to the texture we just created and enable the texture.
			state->setTextureAttributeAndModes(0,tex.get());
			
			// Additionally but not necessary Turn on blending
			osg::BlendFunc* bf = new osg::BlendFunc(
				osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
			state->setAttributeAndModes( bf );

			// Turn on alpha testing
			osg::AlphaFunc* af = new osg::AlphaFunc(
				osg::AlphaFunc::GREATER, 0.05f );
			state->setAttributeAndModes( af );

			geode->setStateSet(state);
		}

		return geode;
	}

	osg::Node* createTracker()
	{
		//osg::ref_ptr<osg::BoundingBox> bb = new osg::BoundingBox();
		//bb->expandBy(bs);
		osg::Geode* tracerGeode = new osg::Geode();
		osg::Geometry* tracerGeometry = new osg::Geometry();
		tracerGeode->addDrawable(tracerGeometry); 
		// Specify the vertices:
		osg::Vec3Array* tracerVertices = new osg::Vec3Array;
		tracerVertices->push_back( osg::Vec3(0, -10, 130) ); // front left 
		tracerVertices->push_back( osg::Vec3(0, -10, 130) ); // front right 
		tracerVertices->push_back( osg::Vec3(10, 10, 130) ); // back right 
		tracerVertices->push_back( osg::Vec3(-10, 10, 130) ); // back left 
		tracerVertices->push_back( osg::Vec3(0, 0, 110) ); // bottom 
		tracerGeometry->setVertexArray( tracerVertices );

		osg::DrawElementsUInt* tracerBase = 
		  new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
		tracerBase->push_back(3);
		tracerBase->push_back(2);
		tracerBase->push_back(1);
		tracerBase->push_back(0);
		tracerGeometry->addPrimitiveSet(tracerBase);
	   
		osg::DrawElementsUInt* tracerFaceOne = 
		  new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	   tracerFaceOne->push_back(0);
	   tracerFaceOne->push_back(1);
	   tracerFaceOne->push_back(4);
	   tracerGeometry->addPrimitiveSet(tracerFaceOne);

	   osg::DrawElementsUInt* tracerFaceTwo = 
		  new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	   tracerFaceTwo->push_back(1);
	   tracerFaceTwo->push_back(2);
	   tracerFaceTwo->push_back(4);
	   tracerGeometry->addPrimitiveSet(tracerFaceTwo);

	   osg::DrawElementsUInt* tracerFaceThree = 
		  new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	   tracerFaceThree->push_back(2);
	   tracerFaceThree->push_back(3);
	   tracerFaceThree->push_back(4);
	   tracerGeometry->addPrimitiveSet(tracerFaceThree);

	   osg::DrawElementsUInt* tracerFaceFour = 
		  new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	   tracerFaceFour->push_back(3);
	   tracerFaceFour->push_back(0);
	   tracerFaceFour->push_back(4);
	   tracerGeometry->addPrimitiveSet(tracerFaceFour);

	   osg::Vec4Array* colors = new osg::Vec4Array;
	   colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) ); //index 0 red
	   colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f) ); //index 1 green
	   colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) ); //index 1 green
	   osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4> 
		  *colorIndexArray;colorIndexArray = new 
		  osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
	   colorIndexArray->push_back(0); // vertex 0 assigned color array element 0
	   colorIndexArray->push_back(1); // vertex 1 assigned color array element 1
	   colorIndexArray->push_back(2); // vertex 2 assigned color array element 2
	   colorIndexArray->push_back(0); // vertex 3 assigned color array element 3
	   colorIndexArray->push_back(1); // vertex 4 assigned color array element 3
	   tracerGeometry->setColorArray(colors);
	   tracerGeometry->setColorIndices(colorIndexArray);
	   tracerGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
		osg::StateSet* state = new osg::StateSet();
		// Additionally Turn on blending
		osg::BlendFunc* bf = new osg::BlendFunc(
			osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
		state->setAttributeAndModes( bf );
		tracerGeode->setStateSet(state);

		return tracerGeode;
	}
}

#endif //_OTHER_SHAPES_H

