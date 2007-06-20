/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 * based on Rob Bateman examples code
 */


#pragma once

#ifndef _GEN_TEXTURE_MANAGER_H
#define _GEN_TEXTURE_MANAGER_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <map>
#include "../utility/debug.h"


namespace ft
{
	struct TexRef
	{
		/// the openGL texture object
		unsigned int idx;

		/// the reference count
		unsigned int ref;

		/// the size of the loaded texture
		unsigned int size;

		/// the alias 
		
		/// ctor
		TexRef(unsigned id_,unsigned sz) : idx(id_),ref(1),size(sz) {}
		/// ctor
		TexRef() : idx(0),ref(0),size(0) {}
		/// ctor
		TexRef(const TexRef& tr) : idx(tr.idx),ref(tr.ref),size(tr.size) {}
	};

    class TextureManager
    {
    public:
        TextureManager(void) { /*empty*/}
	    virtual ~TextureManager(void) { /*empty*/}
        
        static TextureManager* getInstance();
        static void DestroyInstance();
		
		unsigned int LoadTexture(const std::string filename,bool compressed=true);
		void ReleaseTexture(unsigned int);
		unsigned int GetTextureSize(unsigned int);
		unsigned int GetTotalTextureSize();
	private:
        static TextureManager* m_instance;
		std::map<std::string,TexRef> m_Textures;

		unsigned int MakeGlTexture(GLenum Format,const unsigned char *pixels,unsigned int w,unsigned int h,bool compressed);
		int LoadTgaImage(const std::string filename,unsigned char** pixels,unsigned int *w,unsigned int *h,unsigned int* bpp);
		int LoadPcxImage( const std::string filename, unsigned char** pixels,  unsigned int* w,  unsigned int* h,  unsigned int* bpp);
		int LoadBmpImage( const std::string filename,  unsigned char** pixels,  unsigned int* w,  unsigned int* h,  unsigned int* bpp);
		char *ExtractFileName(const char* path);	
    };

	/// this structure is used to reference count OpenGL textures

};

#endif //_GEN_TEXTURE_MANAGER_H
