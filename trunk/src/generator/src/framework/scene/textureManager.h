//----------------------------------------------------------------------------//
// textureManager.cpp                                                         //
// Copyright (C) 2007 Future Today                                            //
// author mka																  //
// based on Rob Bateman examples code                                         //
//----------------------------------------------------------------------------//

#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <map>

class TextureManager
{
public:
	TextureManager(void);
	virtual ~TextureManager(void);
	unsigned int LoadTexture(const char* filename,bool compressed=true);
	void ReleaseTexture(unsigned int);
	void DeleteAllTextures();
	unsigned int GetTextureSize(unsigned int);
	unsigned int GetTotalTextureSize();
protected:
	unsigned int MakeGlTexture(GLenum Format,const unsigned char *pixels,unsigned int w,unsigned int h,bool compressed);
	int LoadTgaImage(const char filename[],unsigned char** pixels,unsigned int *w,unsigned int *h,unsigned int* bpp);
	int LoadPcxImage( const char filename[], unsigned char** pixels,  unsigned int* w,  unsigned int* h,  unsigned int* bpp);
	int LoadBmpImage( const char filename[],  unsigned char** pixels,  unsigned int* w,  unsigned int* h,  unsigned int* bpp);
		
};

#endif