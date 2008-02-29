/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 * based on Rob Bateman examples code
 */


#pragma once

#ifndef _GEN_TEXTURE_MANAGER_H
#define _GEN_TEXTURE_MANAGER_H

#ifndef _CRT_SECURE_NO_WARNINGS    //against fopen ms warnings
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "../core/global.h"

namespace ft
{
	/// this structure is used to reference count OpenGL textures
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
	//! A TextureManager class
	/*!
	 *	This class is responsible for loading and releasing of textures for system. It creates textures on demand from files 
	 *  and places them in texture map. It's important that routines are OpenGL dependent. 
	 */
    class TextureManager
    {
    public:
        TextureManager(void) { /*empty*/}
	    virtual ~TextureManager(void) { /*empty*/}
        //! singleton - Returns the only instance of TextureManager
        static TextureManager* getInstance();
		//! destroy all resources owned by TextureManager
        static void DestroyInstance();

		unsigned int LoadTexture(const std::string filename,bool compressed=true);
		void ReleaseTexture(unsigned int);
		unsigned int GetTextureSize(unsigned int);
		unsigned int GetTotalTextureSize();
	private:
		//! take raw image data and convert it to an openGL
		unsigned int MakeGlTexture(GLenum Format,const unsigned char *pixels,unsigned int w,unsigned int h,bool compressed);
		static TextureManager* m_instance;
		std::map<std::string,TexRef> m_Textures;
		int LoadTgaImage(const std::string filename,unsigned char** pixels,unsigned int *w,unsigned int *h,unsigned int* bpp);
		int LoadPcxImage( const std::string filename, unsigned char** pixels,  unsigned int* w,  unsigned int* h,  unsigned int* bpp);
		int LoadBmpImage( const std::string filename,  unsigned char** pixels,  unsigned int* w,  unsigned int* h,  unsigned int* bpp);
		char *ExtractFileName(const char* path);	
    };
}

#endif //_GEN_TEXTURE_MANAGER_H
