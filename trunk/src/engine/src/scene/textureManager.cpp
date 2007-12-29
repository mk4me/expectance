/*
 * Copyright (c) 2007, FutureToday. All rights reserved.
 * author: mka
 * based on Rob Bateman examples code
 */

#include "TextureManager.h"
#include <assert.h>


// openGL extensions if not present, define them
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
	#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT			0x83F0
	#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT		0x83F1
	#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT		0x83F2
	#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT		0x83F3
#endif

#ifndef GL_TEXTURE_COMPRESSED_IMAGE_SIZE
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#endif

using namespace ft;

TextureManager* TextureManager::m_instance = NULL;

TextureManager* TextureManager::getInstance()
{
    if (m_instance == NULL)
    {
        _dbg << "TextureManager::getInstace(): instance of TextureManager created " << std::endl;
        m_instance = new TextureManager();
    }

    return m_instance;
}

void TextureManager::DestroyInstance()
{
	if (m_instance != NULL)
	{
		std::map<std::string,TexRef>::iterator it=m_instance->m_Textures.begin();
		for( ; it != m_instance->m_Textures.end(); ++it ) {
			glDeleteTextures(1,&it->second.idx);
		}
		m_instance->m_Textures.clear();

		//m_instance->DeleteAllTextures();
        delete m_instance;
	}
}



//--------------------------------------------------------------------------
/// \brief	This function takes raw image data and converts it to an openGL
///			texture. 
/// \param	Format		-	GL_RGB,GL_RGBA,GL_ALPHA
/// \param	pixels		-	the pixel data
/// \param	w			-	the image width
/// \param	h			-	the image height
///	\param	compressed	-	true for DXT compression
///

unsigned int TextureManager::MakeGlTexture(GLenum Format,const unsigned char *pixels,unsigned int w,unsigned int h,bool compressed)
{
	unsigned int texObject;

	// generate a texture object which will be used to reference the
	// data on the graphics card once loaded.
	glEnable(GL_TEXTURE);
	glGenTextures(1,&texObject);

	// specify byte alignment
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	// bind the texture so we can edit it
	glBindTexture (GL_TEXTURE_2D, texObject);

	// make the texture repeat in the u and v texture directions
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// linearly filter the texture when it needs to be magnified
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// linearly filter textures into the distance
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//	glTexImage2D copies the texture data from system memory onto the graphics
	//	card. If we wish to use DXT compression then we simply change the internal
	//	format parameter to glTexImage2D.
	//
	if(compressed) {
		switch(Format) {
		//
		case GL_RGB:
  			glTexImage2D (GL_TEXTURE_2D,0,GL_COMPRESSED_RGB_S3TC_DXT1_EXT,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
			break;

		//
		case GL_RGBA:
  			glTexImage2D (GL_TEXTURE_2D,0,GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
			break;

		//
		case GL_ALPHA:
			glTexImage2D (GL_TEXTURE_2D,0,GL_ALPHA,w,h,0,GL_ALPHA,GL_UNSIGNED_BYTE,pixels);
			break;

		default: ;
		}
	}
	else {
		glTexImage2D (GL_TEXTURE_2D,0,Format,w,h,0,Format,GL_UNSIGNED_BYTE,pixels);
	}

	return texObject;

}



//--------------------------------------------------------------------------
/// \brief	This function loads the specified image file and returns the 
///			openGL texture object for it. Only tga, bmp and pcx images are
///			supported.
/// \param	filename	-	the image file to use for the texture
/// \return	compressed	-	if true DXT compression will be used.
///
unsigned int TextureManager::LoadTexture(const std::string filename,bool compressed) {

	// check to see if file is already loaded.
	std::map<std::string,TexRef>::iterator it = m_Textures.find(filename);
	if ( it!=m_Textures.end()) {
		it->second.ref++;
		return it->second.idx;
	}

	// if not, try to load the file.
	unsigned int w=0,h=0,bpp=0;
	unsigned char* pixels=0;

	int len = filename.length(); 

	// load a bmp
	if( (filename[len-3] == 'b' || filename[len-3] == 'B') &&
		(filename[len-2] == 'm' || filename[len-2] == 'M') &&
		(filename[len-1] == 'p' || filename[len-1] == 'P') ) {
		if(!LoadBmpImage(filename,&pixels,&w,&h,&bpp))
			return 0;
	}
	else
	// load a pcx
	if( (filename[len-3] == 'p' || filename[len-3] == 'P') &&
		(filename[len-2] == 'c' || filename[len-2] == 'C') &&
		(filename[len-1] == 'x' || filename[len-1] == 'X') ) {
		if(!LoadPcxImage(filename,&pixels,&w,&h,&bpp))
			return 0;
	}
	else
	// load a tga 
	if( (filename[len-3] == 't' || filename[len-3] == 'T') &&
		(filename[len-2] == 'g' || filename[len-2] == 'G') &&
		(filename[len-1] == 'a' || filename[len-1] == 'A') ) {
		if(!LoadTgaImage(filename,&pixels,&w,&h,&bpp))
			return 0;
	}
	else {
		_dbg << "Unsupported image format\n";
		return 0;
	}

	// generat the correct texture type for the image
	unsigned int tex_object;
	switch(bpp) {
	case 1:
		tex_object = MakeGlTexture(GL_ALPHA,pixels,w,h,compressed);
		break;
	case 2:
		break;
	case 3:
		tex_object = MakeGlTexture(GL_RGB,pixels,w,h,compressed);
		break;
	case 4:
		tex_object = MakeGlTexture(GL_RGBA,pixels,w,h,compressed);
		break;
	default:
		break;
	}

	int data_size=0;
	if(compressed) {
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&data_size);
	}
	else {
		data_size = bpp*w*h;
	}


	// delete the pixel data because we no longer need it
	free(pixels);
	
	// insert the texture into a map to keep track of it
	m_Textures.insert( std::make_pair( std::string(filename), TexRef(tex_object,data_size) ) );

	// return GL texture object
	return tex_object;

}

//--------------------------------------------------------------------------
/// \brief	This function will release the specified texture (as long as 
///			it's not used elsewhere).
/// \param	the OpenGL texture object
///
void TextureManager::ReleaseTexture(unsigned int idx) {

	// iterate through the textures to find the requested texture
	std::map<std::string,TexRef>::iterator it=m_Textures.begin();
	for( ; it != m_Textures.end(); ++it ) {

		// when found...
		if(idx == it->second.idx) {

			// if ref count is zero ...
			if(--it->second.ref == 0) {

				// delete texture
				glDeleteTextures(1,&(it->second.idx));
				m_Textures.erase(it);
			}
			return;
		}
	}
}


//--------------------------------------------------------------------------
/// \brief	This function returns the amount of memory used to store the 
///			specified openGL texture. 
/// \param	idx	-	the openGL texture object
/// \return	the amount of memory used (in bytes)
///
unsigned int TextureManager::GetTextureSize(unsigned int idx) {

	// iterate through the textures to find the requested texture
	std::map<std::string,TexRef>::iterator it=m_Textures.begin();
	for( ; it != m_Textures.end(); ++it ) {

		// when found...
		if(idx == it->second.idx) {

			return it->second.size;
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
/// \brief	This returns the amount of memory used to store all textures 
///			currently loaded. The size returned is in bytes
/// \return	the total amount of memory used (in bytes)
///
unsigned int TextureManager::GetTotalTextureSize() {
	unsigned int sz=0;
	// iterate through the textures to find the requested texture
	std::map<std::string,TexRef>::iterator it=m_Textures.begin();
	for( ; it != m_Textures.end(); ++it ) {
		sz += it->second.size;		
	}
	return sz;
}


/*----- Extract filename -----*/
//char *TextureManager::ExtractFileName(const char *path)
//{
//  char buf[256],*p,*q;
//  int n;
//  strcpy(buf,path);
//  /*Delete extension*/
//  n=strlen(buf);
//  p=buf+n;
//  while (*--p!='.' && p>buf);
//  if (*p=='.')
//   {
//	*p='\0';
//	printf("Filename without extension: %s\n",buf);
//   }
//  else
//   {
//	printf("Filename doesn't have an extension\n");
//	p=buf+n;
//   }
//  /*Compute filename length*/
//  q=p;
//  while (*--q!='\\');
//  ++q;
//  //Filename length: p-q
//  //Filename without path: q
//  return q;
//}

/******************************************************** TGA *****************************************************/
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif



/*
 *	most compilers tend to align structure data size so that its size is 4byte
 *	aligned (ie, divisable by 4). This happens due to compiler optimisations that we have
 *	to manually prevent. This can be done with the #pragma pack() directive. Without it,
 *	the struct would probably end up looking like :
 *
 *		typedef struct
 *		{
 *			// bytes 0 to 3
 *			unsigned char  m_ImageIdent;
 *			unsigned char  m_ColourMapType;
 *			unsigned char  m_ImageType;
 *
 *			unsigned char  PADDING1;
 *
 *			// bytes 4 to 7
 *			unsigned short m_ColourMapOrigin;
 *			unsigned short m_ColourMapSize;
 *
 *			// bytes 8 to 11
 *			unsigned char  m_ColourMapESize;
 *
 *			unsigned char  PADDING2;
 *
 *			unsigned short m_Xorigin;
 *
 *			// bytes 12 to 15
 *			unsigned short m_Yorigin;
 *			unsigned short m_Width;
 *
 *			// bytes 16 to 19
 *			unsigned short m_Height;
 *			unsigned char  m_PixelIndexSize;
 *			unsigned char  m_ImageDescByte;
 *		}
 *		tga_header;
 *
 *	This struct is 20 bytes in size, which is going to cause us some big problems when
 *	trying to read an 18 byte chunk from the file. We will end up with rubbish data.
 *	This is why we tell compilers to align data structures to one byte.
 */
#pragma pack(push,1)

/*
*	This structure is used to hold the header of the tga image file format
*/
typedef struct
{
	unsigned char  m_ImageIdent;		/* size of image identification field */
	unsigned char  m_ColourMapType;		/* 0 - No colour map, 1 - has colour map */
	unsigned char  m_ImageType;			/* 0 - No Image, 1 - 8bit, 2 - uncompress RGB ..... */
	unsigned short m_ColourMapOrigin;	/* Index of first colour map entry */
	unsigned short m_ColourMapSize;		/* Number of colour map entries */
	unsigned char  m_ColourMapESize;	/* Number of bits for each entry (24) */
	unsigned short m_Xorigin;			/* x origin of image */
	unsigned short m_Yorigin;			/* y origin of image */
	unsigned short m_Width;				/* width of image in pixels */
	unsigned short m_Height;			/* height of image in pixels */
	unsigned char  m_PixelIndexSize;	/* number of bits stored in each pixel index */
	unsigned char  m_ImageDescByte;		/* should always be zero.... */
}
tga_header;


/*
 *	This structure is used purely for reading the RLE files.
 */
typedef struct
{
	unsigned char Count : 7;
	/* if these two bits are 1, then we have a start of a byte run */
	unsigned char Flag  : 1;
}
TgaByte;

/*
*	reset structure alignment back to the default
*/
#pragma pack(pop)


/*
 *	This union is a total of 2 bytes in size. s and b both share the
 *	same memory so the 2byte variable can be easily split into its constituant bytes.
 *	By assigning 1 to the 2byte variable, we can expect the array b to hold
 *	b[] = { 1, 0 } on a PC (LSB) and b[] = { 0, 1 } on an o2 (MSB). It also makes
 *	it nice and easy to swap the variables bytes.
 */
union charToShort
{
	/* 1 x 2 byte variable */
	unsigned short s;
	/* 2 x 1 byte variables */
	unsigned char  b[2];
};



/*
 *	This function is used to determine if the system has LSB or MSB byte ordering.
 *	By using a union, both the 2byte variable(short) and the array of 2 individual
 *	bytes (chars) will be stored in the same memory location. ie, the charToShort
 *	union data type is only two bytes in size and allows you to split apart a 2byte
 *	variable to check the ordering.
 *
 *	The two byte variables in the tga header will have to have their byte orderings
 *	switched on irix and mac, hence the use of this (sneaky) function to see if we need
 *	to swap the data bytes.
 */
int CheckByteOrder()
{
	/* Create a 2byte check variable */
	union charToShort cs;

	/* assign 1 to the 2byte variable */
	cs.s = 1;

	/* check the first byte */
	if(cs.b[0] == 0)
		return 1;

	return 0;
}



/*
 *	This function switches the byte orderings of two byte variables. It accepts a
 *	pointer to the memory location you wish to swap.
 */
void Swap2Bytes(IN void *ptr)
{
	/* assign a pointer to the 2byte variable */
	unsigned char *pcs = (unsigned char*)ptr;

	/* swap the bytes around */
	unsigned char temp = pcs[0];
	pcs[0] = pcs[1];
	pcs[1] = temp;
}

/*
 *	\brief	This function flips pixel colours from RGB (or RGBA) to BGR/BGRA
 *	\param	data	-	the pixel data to flip
 *	\param	num_px	-	the number of pixels to flip
 *	\param	bpp		-	the bytes per pixel, ie 1,3,4
 */
void BGR_TO_RGB(unsigned char* data,unsigned int num_px,unsigned int bpp)
{
	/* get pointer to data end */
	unsigned char *end = data + (bpp * num_px);

	/* loop till end of data, move on by number of bytes in pixel each time */
	for( ; data != end ; data += bpp )
	{
		unsigned char temp = *data;
		*data = data[2];
		data[2] = temp;
	}
}

/*
 *	\brief	This function flips an image upside down...
 *	\param	data	-	the pixel data to flip
 *	\param	rowsize	-	the data size of each scanline
 *	\param	height 	-	the number of scanlines in the image
 */
void FlipDaImage( unsigned char* data, unsigned int rowsize,unsigned int height)
{
	unsigned int mid = height/2,i,j;
	unsigned char *temprow = (unsigned char*) malloc ( rowsize );
	assert( temprow );

	for( i=0, j=(height-1); i<mid; ++i,--j )
	{
		/* copy low row to temp */
		memcpy( temprow           , &data[ i*rowsize ], rowsize );

		/* copy high row to low row */
		memcpy( &data[ i*rowsize ], &data[ j*rowsize ], rowsize );

		/* copy temp to high row */
		memcpy( &data[ j*rowsize ], temprow           , rowsize );
	}

	/* delete allocated data */
	free( temprow );
}

/*
 *	A function to load a tga file and return the OpenGL texture object
 */
int TextureManager::LoadTgaImage(const std::string filename,unsigned char** ppData,unsigned int* w,unsigned int* h,unsigned int* bpp)
{

	/*
	 *	Create an instance of the struct to hold the tga file header
	 */
	tga_header header;


	/*
	 *	Create a file pointer called fp and attempt to open the file
	 */
	FILE *fp = fopen(filename.c_str(),"rb");

	/*
	 *	if the file pointer is equal to NULL, the file failed to open
	 */
	if(fp)
	{
		/*
		 *	A pointer to hold the memory location of allocated memory that
		 *	we will require in order to store the pixel data
		 */
		unsigned int i,j,k;

		/*
		 *	Read the file header
		 */
		fread(&header,1,sizeof(tga_header),fp);

		/*
		 *	Check the byte ordering of the computer to see if we have to swap any bytes .
		 *	It's that pesky MSB/LSB thing again....
		 */
		if(CheckByteOrder())
		{
			/*
			 *	Swap the 2byte variables from the tga file header so that they make
			 *	sense on a unix box...
			 */
			Swap2Bytes( (void*)&header.m_ColourMapOrigin );
			Swap2Bytes( (void*)&header.m_ColourMapSize   );
			Swap2Bytes( (void*)&header.m_Xorigin );
			Swap2Bytes( (void*)&header.m_Yorigin );
			Swap2Bytes( (void*)&header.m_Width  );
			Swap2Bytes( (void*)&header.m_Height );
		}

		*w = header.m_Width;
		*h = header.m_Height;

		/* skip the image identification field if present */
		for( i=0; i!=header.m_ImageIdent; ++i )
		{
			fgetc(fp);
		}

		/* store bytes per pixel */
		*bpp = header.m_PixelIndexSize/8;

		/*
		 *	Determine the size (in bytes) of the pixel data size contained
		 *	in the file. width * height * 3 bytes per colour (RGB).
		 *
		 *	Use malloc to allocate the required memory size
		 */
		*ppData = (unsigned char*)malloc( (*bpp) * header.m_Width * header.m_Height );

		/*
		 *	Use assert to ensure that the allocation succeeded
		 */
		assert( *ppData != NULL);


		/* an 8bit colour mapped image */
		if( header.m_ImageType  == 1 )
		{
			/* This pointer holds reference to the actual 8bit data */
			unsigned char* tempptr = *ppData,*pd=0,*pData=0;

			/* number of bytes per row */
			unsigned int rowsize = (header.m_ColourMapESize/8) * header.m_Width;

			/* allocate and then read in the colour map.... */
			unsigned char* colourMap = (unsigned char*) malloc ( header.m_ColourMapSize * (header.m_ColourMapESize/8) );

			/* check allocation */
			assert( colourMap );

			/* store the new bytes per pixel (as given by the colour map entry size) */
			*bpp = ( header.m_ColourMapESize / 8 );

			/* read colour map data */
			fread( colourMap, header.m_ColourMapSize *(*bpp), 1, fp );

			/* flip colour map from BGR to RGB */
			BGR_TO_RGB( colourMap, header.m_ColourMapSize, *bpp );

			/* read the data in one go, we'll flip it upside down when expanding the colour map data.... */
			fread( tempptr, 1, header.m_Width * header.m_Height, fp );

			/* allocate memory for the new expanded image data... */
			*ppData = (unsigned char*) malloc( header.m_Height * rowsize );
			assert( *ppData );

			/* this pointer is used as an iterator  */
			pData = (*ppData);


			/* now going to run backwards through 8bit image and write it upside down when expanding the colour map */
			for( i=0; i != header.m_Height; ++i )
			{
				/* get a pointer to the data row */
				pd = tempptr + ( (header.m_Height-i-1) * header.m_Width );

				/* loop till the end of the row */
				for( j=0; j != rowsize; ++pd )
				{
					/* get pointer to actual colour in colour map */
					unsigned char *col =  &colourMap[ 3*(*pd) ];

					/* get end of colour */
					unsigned char *colend = col + *bpp;

					/* copy the colour over (3 or 4bytes probably...) */
					for( ; col != colend; ++col, ++j, ++pData )
					{
						/* copy colour from colour map into correct place in expanded image */
						*pData = *col;
					}
				}
			}

			/* free up any data... */
			free( tempptr );
			free( colourMap );


		}
		else

		/* uncompressed grey/RGB/RGBA data */
		if( header.m_ImageType  == 2 )
		{
			/* number of bytes per row */
			unsigned int rowsize = (*bpp) * header.m_Width;

			/* pointer to last row in image data */
			unsigned char* pd = *ppData + (*bpp) * (header.m_Height-1) * header.m_Width;

			/*
			 *	read the pixel data from the file in rows cos tga files are upside down...
			 */
			for( i = 0; i < header.m_Height; ++i, pd -= rowsize )
			{
				fread( pd, 1, rowsize, fp );
			}

			/* don't bother flipping bytes if greyscale image */
			if( *bpp != 1 )
			{
				/*
				 *	the tga file stores pixel data as bgr values. OpenGL however requires
				 *	the data in RGB. Therefore we have to traverse the array and swap the
				 *	pixel data around abit.
				 */
				BGR_TO_RGB( (*ppData), header.m_Height * header.m_Width, *bpp );
			}
		}
		else

		/* RLE colour mapped images */
		if( header.m_ImageType  == 9 )
		{
			/* This pointer holds reference to the actual 8bit data */
			unsigned char* tempptr = *ppData;

			/* number of bytes per row */
			unsigned int rowsize = (header.m_ColourMapESize/8) * header.m_Width;

			/* allocate and then read in the colour map.... */
			unsigned char* colourMap = (unsigned char*) malloc ( header.m_ColourMapSize * (header.m_ColourMapESize/8) );

			/* temporary iterator pointers */
			unsigned char *pde,*pData;

			/* check allocation */
			assert( colourMap );

			/* store the new bytes per pixel (as given by the colour map entry size) */
			*bpp = ( header.m_ColourMapESize / 8 );

			/* read colour map data */
			fread( colourMap, header.m_ColourMapSize * (*bpp), 1, fp );

			/* flip colour map from BGR to RGB */
			BGR_TO_RGB( colourMap, header.m_ColourMapSize, *bpp );

			/* allocate memory for the uncompressed image... */
			*ppData = (unsigned char*) malloc ( header.m_Height * header.m_Width * (*bpp)  );

			/* get ptr to first element */
			pData = *ppData;

			/* get end pointer marker */
			pde = pData + header.m_Height * header.m_Width * (*bpp);

			/* now the reading process begins!! */
			while( pde != pData )
			{
				unsigned char byte;

				fread( &byte, 1, 1, fp );

				/*
				 *	if the flag is zero, then the count indicates a run of raw data
				 */
				if( (byte & 0x80) == 0 )
				{
					byte++;

					for(j=0;j<byte; ++j)
					{
						unsigned char c = fgetc(fp);
						for( k = 0; k != *bpp; ++k, ++pData )
						{
							*pData = colourMap[ (*bpp)*c + k ];
						}
					}
				}

				/*
				 *	If the flag is one, then the count represents the amount of times that the next byte
				 *	is repeated. This is the RLE bit....
				 */
				else
				{
					unsigned char c = fgetc(fp);
					byte -= 0x7F;

					for(j=0;j<byte; ++j)
					{
						for( k = 0; k != *bpp; ++k, ++pData )
						{
							*pData = colourMap[ (*bpp)*c + k ];
						}
					}

				}
			}

			/* turn the image upside down .... */
			FlipDaImage( *ppData, rowsize, header.m_Height );
		}
		else

		/* RLE compressed grey/RGB/RGBA images */
		if( header.m_ImageType  == 10 )
		{
			/* This pointer holds reference to the actual 8bit data */
			unsigned char* tempptr = *ppData;

			/* number of bytes per row */
			unsigned int rowsize = (*bpp) * header.m_Width;

			/* temporary iterator pointers */
			unsigned char* pde,*pData;

			/* get ptr to first element */
			pData = *ppData;

			/* get end pointer marker */
			pde = pData + header.m_Height * header.m_Width * (*bpp);

			/* now the reading process begins!! */
			while( pData < pde )
			{
				unsigned char colour[10] = {0,0,0,0};
				unsigned char byte;

				fread( &byte, 1, 1, fp );

				/*
				 *	if the flag is zero, then the count indicates a run of raw data
				 */
				if( (byte & 0x80) == 0 )
				{
					byte++;

					for(j=0;j<byte; ++j)
					{
						fread( colour, 1, *bpp, fp );

						for( k = 0; k != *bpp; ++k, ++pData )
						{
							*pData = colour[ k ];
						}
					}
				}

				/*
				 *	If the flag is one, then the count represents the amount of times that the next byte
				 *	is repeated. This is the RLE bit....
				 */
				else
				{
					byte -= 0x7F;

					fread( colour, 1, *bpp, fp );

					for(j=0;j<byte; ++j)
					{
						for( k = 0; k != *bpp; ++k, ++pData )
						{
							*pData = colour[ k ];
						}
					}

				}
			}

			/* flip colour map from BGR to RGB */
			BGR_TO_RGB( *ppData, header.m_Width*header.m_Height, *bpp );

			/* turn the image upside down .... */
			FlipDaImage( *ppData, rowsize, header.m_Height );
		}

		/* unsupported image type... */
		else
		{
			switch( header.m_ImageType )
			{
			case 0:
				printf("[ERROR] The file %s contains no image data\n",filename);
				break;
			case 3:
				printf("[ERROR] The file %s contains black & white data which is unsupported\n",filename);
				break;
			case 11:
				printf("[ERROR] The file %s contains compressed black & white data which is unsupported\n",filename);
				break;
			case 32:
				printf("[ERROR] The file %s contains compressed color map data which is un-supported\n",filename);
				break;
			case 33:
				printf("[ERROR] The file %s contains compressed color map data which is un-supported (4pass quad tree process)\n",filename);
				break;
			default:
				printf("[ERROR] The file %s does not appear to be valid\n",filename);
				break;

			}
		}

		/*
		 *	close the file
		 */
        fclose(fp);

        return 1;
	}
	return 0;
}

/***************************************************** PCX **********************************************************/

#pragma pack(push,1)
/*
	The pcx Header structure is 128 bytes in size. The information in the structure is roughly
	detailed as the following (according to the pcx file format description on www.wotsit.org)


		Byte      Item          Size   Description/Comments
		 0         Manufacturer 1      Constant Flag, 10 = ZSoft .pcx
		 1         Version      1      Version information
									   0 = Version 2.5 of PC Paintbrush
									   2 = Version 2.8 w/palette information
									   3 = Version 2.8 w/o palette information
									   4 = PC Paintbrush for Windows(Plus for
										  Windows uses Ver 5)
									   5 = Version 3.0 and > of PC Paintbrush
										  and PC Paintbrush +, includes
										  Publisher's Paintbrush . Includes
										  24-bit .PCX files
		 2         Encoding      1     1 = .PCX run length encoding
		 3         BitsPerPixel  1     Number of bits to represent a pixel
										  (per Plane) - 1, 2, 4, or 8
		 4         Window        8     Image Dimensions: Xmin,Ymin,Xmax,Ymax
		12         HDpi          2     Horizontal Resolution of image in DPI*
		14         VDpi          2     Vertical Resolution of image in DPI*
		16         Colormap     48     Color palette setting, see text
		64         Reserved      1     Should be set to 0.
		65         NPlanes       1     Number of color planes
		66         BytesPerLine  2     Number of bytes to allocate for a scanline
										  plane.  MUST be an EVEN number.  Do NOT
										  calculate from Xmax-Xmin.
		68         PaletteInfo   2     How to interpret palette- 1 = Color/BW,
										  2 = Grayscale (ignored in PB IV/ IV +)
		70         HscreenSize   2     Horizontal screen size in pixels. New field
										  found only in PB IV/IV Plus
		72         VscreenSize   2     Vertical screen size in pixels. New field
										  found only in PB IV/IV Plus
		74         Filler       54     Blank to fill out 128 byte header.  Set all
										 bytes to 0


		This then starts to look somewhat like this :
*/
typedef struct
{
	unsigned char	Manufacturer;
	unsigned char	Version;
	unsigned char	Encoding;
	unsigned char	BitsPerPixel;

	struct
	{
		unsigned short Xmin;
		unsigned short Ymin;
		unsigned short Xmax;
		unsigned short Ymax;
	}
	Window;

	short 			HDpi;
	short 			VDpi;
	unsigned char	ColorMap[48];
	unsigned char	Reserved1;
	unsigned char	NPlanes;
	short 			BytesPerLine;
	short 			PaletteInfo;
	short 			HscreenSize;
	short 			VscreenSize;
	unsigned char	Filler[54];
}
PcxHeader;

/*!
 *	\brief	This structure is intended to hold a single byte of information from the pcx file. This is kinda important
 *			because the bytes hold information about whether a run or single byte is used. This is basically a bit
 *			annoying so instead it is easier to create something using bit-fields that can quickly identify the bit
 *			flags that are set rather than using horrible bit-wise operations. I just kinda prefer it cos it's more
 *			readable....
 */
typedef struct
{
	union
	{
		struct
		{
			unsigned char Count : 6;
			/* if these two bits are 1, then we have a start of a byte run */
			unsigned char Flag1 : 1;
			unsigned char Flag2 : 1;
		};
		unsigned char byte;
	};
}
PcxByte;

typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}
PcxColour;

#pragma pack(pop)



/*!
 *	\brief	This function loads a ZSoft PCX File image file.
 *	\param	filename	-	the name of the file to load
 *	\param	pixels		-	pointer to the pointer that will hold the pixel data (yuck)
 *	\param	w			-	the width of the image file
 *	\param	h			-	the height of the image file
 *	\param	bpp			-	the bytes per pixel of the image file.
 *	\return	1 if ok, 0 otherwise
 */
int TextureManager::LoadPcxImage( const std::string filename, unsigned char** pixels,  unsigned int* w, unsigned int* h, unsigned int* bpp)
{
	/* open the file */
	FILE* fp = fopen( filename.c_str(),"rb" );

	/* if opened OK */
	if(fp)
	{
		/* holds the 128 byte header */
		PcxHeader header;

		/* read the header in */
		fread(&header,128,1,fp);

		if( header.Manufacturer == 10 )
		{
			/* calculate the image width & height */
			unsigned int XSIZE 			= header.Window.Xmax   -  header.Window.Xmin  +  1;
			unsigned int YSIZE 			= header.Window.Ymax   -  header.Window.Ymin  +  1;

			/* The number of bytes per line */
			unsigned int BytesPerLine	= header.BytesPerLine  *  header.NPlanes;

			/* simple count vars */
			unsigned int i,j,k;

			/* a temporary buffer for a scanline */
			unsigned char *tempbuffer;

			/* another temp var */
			unsigned int iCount;



			/* check pcx version */
			if( header.Version != 0x05 )
			{
				printf("[ERROR] Only version 5 of the pcx file format is supported....\n");
				fclose(fp);
				return 0;
			}

			/* allocate enough space for the pixel data */
			tempbuffer = (unsigned char*) malloc ( BytesPerLine );
			assert( tempbuffer );


			/* allocate enough space for the pixel data */
			(*pixels) = (unsigned char*) malloc ( header.NPlanes * XSIZE * YSIZE );
			assert( *pixels );

			/* store info */
			*w    =  XSIZE;
			*h    =  YSIZE;
			*bpp  =  header.NPlanes;

			/* Loop through until whole image is loaded.... */
			for (i = 0; i < YSIZE; ++i )
			{
				j = 0;

				/*
				 *	because the scanlines are orderer RRRGGGBBB rather than RGBRGBRGB, I'm reading and decompressing
				 *	the file one scanline at a time.
				 */
				while( j < BytesPerLine )
				{
					PcxByte byte;

					/* assume that at the very least we have a count of 1 */
					iCount=1;

					/* read a byte */
					fread( &byte, 1, 1, fp );

					/*
					 *	If the top 2 bits of the byte are 1, then the lower 6 bits represent a data count.
					 *	The following byte will then be repeated 'count' number of times.
					 */
					if( byte.Flag1 && byte.Flag2 )
					{
						/* see how many byte repeats we have */
						iCount = (unsigned int)byte.Count;

						/* read the data byte */
						fread( &byte, 1, 1, fp );
					}

					/* expand the compressed data... */
					for ( k = 0; k < iCount; ++k )
					{
						tempbuffer[j++] = byte.byte;
					}
				}

				/* having read the scanline into the scanline buffer, now re-organise into RGB data in the pixel buffer */
				for( j=0; j < XSIZE; ++j )
				{
					unsigned char *scanline = ((*pixels) + (XSIZE * header.NPlanes * (YSIZE-i-1)));

					for( k =0; k < header.NPlanes; ++k )
					{
						scanline[  (j * header.NPlanes) +  k ] = tempbuffer[ (header.BytesPerLine * k) + j ];
					}
				}
			}

			/* make sure we get rid of the temporary buffer memory allocation.... */
			free( tempbuffer );

			/* if the bytes per pixel == 1, then there will be a colourmap at the end (hopefully) */
			if( header.NPlanes == 1 )
			{
				unsigned char testchar=0;

				/* move to 769 bytes from the end... */
				fseek( fp, -769, SEEK_END );

				/* read the next char */
				testchar=fgetc(fp);

				/* hopefully it will be 12, or 0x0A which indicates the presence of a colour map */
				if( testchar == 0x0C )
				{
					/* allocate enough memory for the colour map */
					PcxColour* Colourmap = (PcxColour*) malloc ( 768 );
					assert(Colourmap);

					/* read the colour map data */
					fread( Colourmap, sizeof(PcxColour), 256, fp );

					/* see notes below/... */
					testchar = 0x01;

					/*
					 *	this is a really crap check for greyscale images. As far as I can tell, a pcx file stores
					 *	greyscale images by storing the grey tones in the colour palette. I can't really see the point
					 *	of expanding this to 24bit because it will then work differently to the other image loaders.
					 *	as a result I check to see if all colours read in actually match their index in which case
					 *	I will just ignore the colour pallette info....
					 */
					for(i=0;i<256;++i)
					{
						if( Colourmap[i].r != i ||
						    Colourmap[i].g != i ||
						    Colourmap[i].b != i )
						{
							testchar = 0;
							break;
						}
					}

					/* if the test proved false, then I will expand the data out into a 24bit colour image */
					if( testchar == 0x00 )
					{
						unsigned char* px_o= (*pixels);
						unsigned char* end_o = px_o + (XSIZE*YSIZE);
						unsigned char* px;

						tempbuffer = (unsigned char*) malloc( XSIZE*YSIZE*3 );
						assert(tempbuffer);

						px = tempbuffer;

						/* Expand the old 8bit colourmapped data into 24bit... */
						while( px_o != end_o )
						{
							*px = Colourmap[ *px_o ].r;
							++px;

							*px = Colourmap[ *px_o ].g;
							++px;

							*px = Colourmap[ *px_o ].b;
							++px;

							++px_o;
						}

						/* delete the not needed 8bit data */
						free( *pixels );

						/* store pointer to new pixel data */
						*pixels = tempbuffer;

						/* update the bytes per pixel... */
						*bpp  =  3;
					}

					free( Colourmap );
				}
			}

		}
		else
		{
			printf("[ERROR] %s is not a pcx file!!\n" , filename );
			fclose(fp);
			return 0;
		}

		fclose(fp);

		return 1;
	}
	return 0;
}




/***************************************************** BMP **********************************************************/
/* force 2 byte alignment of structures */
#pragma pack(push,2)

/*!
 *	This structure comes first in the bitmap file. It's a pretty boring structure really....
 */
typedef struct
{
	/*!
	 *	The characters identifying the bitmap. The following entries are possible:
	 *
	 *	'BM' - Windows 3.1x, 95, NT, .
	 *	'BA' - OS/2 Bitmap Array
	 *	'CI' - OS/2 Color Icon
	 *	'CP' - OS/2 Color Pointer
	 *	'IC' - OS/2 Icon
	 *	'PT' - OS/2 Pointer
	 */
	unsigned char	Identifier[2];

	/*!
	 *	Complete file size in bytes
	 */
	unsigned long	FileSize;

	/*!
	 *	Reserved for later use
	 */
	unsigned long	Reserved;

	/*!
	 *	Offset from beginning of file to the beginning of the bitmap data
	 */
	unsigned long	BitmapDataOffset;
}
BitmapFileHeader;


/*!
 *	This structure immediately follows the main file header. This basically describes
 *	the information contained within
 */
typedef struct
{
	/*!
	 *	Length of the Bitmap Info Header used to describe the bitmap colors, compression,
	 *	The following sizes are possible:
	 *
	 *	28h - Windows 3.1x, 95, NT, .
	 *	0Ch - OS/2 1.x
	 *	F0h - OS/2 2.x
	 */
	unsigned long	BitmapHeaderSize;

	/*!
	 *	Horizontal width of bitmap in pixels.
	 */
	long			Width;

	/*!
	 *	Vertical height of bitmap in pixels.
	 */
	long			Height;

	/*!
	 *	Number of planes in this bitmap.
	 */
	unsigned short	Planes;

	/*!
	 *	Bits per pixel used to store palette entry information. This also identifies
	 *	in an indirect way the number of possible colors. Possible values are:
	 *
	 *	1 - Monochrome bitmap
	 *	4 - 16 color bitmap
	 *	8 - 256 color bitmap
	 *	16 - 16bit (high color) bitmap
	 *	24 - 24bit (true color) bitmap
	 *	32 - 32bit (true color) bitmap
	 */
	unsigned short 	BitsPerPixel;

	/*!
	 *	Compression specifications. The following values are possible:
	 *
	 *	0 - none (Also identified by BI_RGB)
	 *	1 - RLE 8-bit / pixel (Also identified by BI_RLE4)
	 *	2 - RLE 4-bit / pixel (Also identified by BI_RLE8)
	 *	3 - Bitfields  (Also identified by BI_BITFIELDS)
	 */
	unsigned long  	Compression;

	/*!
	 *	Size of the bitmap data in bytes. This number must be rounded to the next 4 byte boundary.
	 */
	unsigned long  	BitmapDataSize;

	/*!
	 *	Horizontal resolution expressed in pixel per meter.
	 */
	unsigned long   HResolution;

	/*!
	 *	Vertical resolution expressed in pixels per meter.
	 */
	unsigned long   VResolution;

	/*!
	 *	Number of colors used by this bitmap. For a 8-bit / pixel
	 *	bitmap this will be 100h or 256.
	 */
	unsigned long   ColorsUsed;

	/*!
	 *	Number of important colors. This number will be equal to the
	 *	number of colors when every color is important.
	 */
	unsigned long   ImportantColors;
}
BitmapInfoHeader;


/*!
 *	This structure is used to hold a colour map entry.
 */
typedef struct
{
	/*!	blue component	*/
	unsigned char b;

	/*!	green component */
	unsigned char g;

	/*!	red component */
	unsigned char r;

	/*!	some say alpha some say reserved.... */
	unsigned char a;
}
ColorMapEntry;

#pragma pack(pop)



/*!
 *	\brief	This function loads a bitmap image
 *	\param	filename	-
 */
int TextureManager::LoadBmpImage(const std::string filename, unsigned char** pixels, unsigned int*   w, unsigned int*   h, unsigned int*   bpp)
{
	BitmapFileHeader	FileHeader;
	BitmapInfoHeader 	ImageHeader;

	FILE *fp = fopen( filename.c_str(), "rb" );

	/* ensure file is a happy file... */
	if( fp == NULL )
	{
		return 0;
	}

	/* read the bitmap file */
	fread( &FileHeader, sizeof(BitmapFileHeader), 1, fp );

	/*
	 *	Make sure we have a valid bitmap file
	 */
	if( FileHeader.Identifier[0] != 'B' ||
		FileHeader.Identifier[1] != 'M' )
	{
		return 0;
	}

	/*
	 *	Read the image header.
	 */
	fread( &ImageHeader, sizeof(BitmapInfoHeader), 1, fp );

	if( ImageHeader.Compression != 0 )
	{
		printf("[ERROR] This bitmap loader does not support compressed files (yet)\n");
		return 0;
	}

	/* store some useful info */
	*w   = ImageHeader.Width;
	*h   = ImageHeader.Height;


	/*----------------------------------------------------------------------------	8 Bit Colour mapped images	*/

	/*
	 *	It appears as though all 8bit bmp images will have a colour map. This is
	 *	a tad bit annoying as a grey scale image would have also been nice!
	 */
	if( ImageHeader.BitsPerPixel == 8 )
	{
		/* pointer for colour map data */
		ColorMapEntry* map;

		/* hopefully if we have a colour map it will be a sensible 8bit one... */
		if( ImageHeader.BitsPerPixel != 8 )
		{
			fprintf(stderr,"[ERROR] The bitmap file you are trying to load has a really odd colour map definition!!\n");
		}

		/* allocate space for colour map */
		map = (ColorMapEntry*) malloc( sizeof(ColorMapEntry) * 256 );
		assert( map );

		/* read colour map */
		fread( map, sizeof(ColorMapEntry), 256, fp );
		
		{
			unsigned int i;
			for(i=0;i<256;++i)
			{
				/* if not a grey colour ignore greyscale reading */
				if( i != map[i].r &&
					i != map[i].g &&
					i != map[i].b ) {
					goto hacky_hacky_moo;
				}
			}		
			
			/* ignore colour map */
			*bpp = 1;
			free( map );

			/* allocate memory for the pixel data */
			*pixels = (unsigned char*) malloc( (*w) * (*h) );
			assert(*pixels);
			fread( *pixels, (*w) * (*h), 1, fp );
			return 1;
		}
		
		hacky_hacky_moo: ;

		/*
		 *	Bitmap files are rather irritatingly aligned to 4 bytes on a line....
		 */
		{
			unsigned int i;
			unsigned int RowSize = ImageHeader.Width;
			unsigned char *scanline;

			if( RowSize%4 != 0 )
			{
				RowSize += (4 - RowSize%4);
			}

			/* allocate enough memory for a scanline */
			scanline = (unsigned char*) malloc( RowSize );
			assert(scanline);

			/* we will expand the colour map to 24bit */
			*bpp = 3;

			/* allocate memory for the pixel data */
			*pixels = (unsigned char*) malloc( (*w) * (*h) * (*bpp) );
			assert(*pixels);

			/* loop through each scanline */
			for(i=0;i<(*h); ++i)
			{
				/* get a pointer to the start of the scanline in expanded data */
				unsigned char* pline = ( (*pixels) + ( 3 * (*w) * i ) );
				unsigned int j;

				/* read the scanline including padding */
				fread( scanline, RowSize, 1, fp );

				/* go through each pixel in line */
				for( j=0; j<(*w); ++j )
				{
					/* copy red from colour map to pixel data array */
					(*pline) = map[ scanline[j] ].r;
					++pline;

					/* copy green from colour map to pixel data array */
					(*pline) = map[ scanline[j] ].g;
					++pline;

					/* copy blue from colour map to pixel data array */
					(*pline) = map[ scanline[j] ].b;
					++pline;
				}
			}

			/* delete the dynamic data */
			free( scanline );
			free( map );
		}
		fclose(fp);
		return 1;
	}


	/*----------------------------------------------------------------------------	24/32 Bit uncompressed images	*/

	{
		unsigned int bdepth;
		unsigned int i;
		unsigned int RowSize;
		unsigned int padding = 0;

		/* determine the bitdepth of the actual bitmap image */
		switch( ImageHeader.BitsPerPixel )
		{
		case 24:
			bdepth=3;
			break;

		case 32:
			bdepth=4;
			break;

		default:
			printf("[ERROR] bitmap loader does not support this bit depth\n");
			fclose(fp);
			return 0;
		}
		*bpp = bdepth;

		/* we will need this value a fair amount, so pre-calculate it... */
		RowSize = bdepth * ImageHeader.Width;

		/* determine the padding at the end of the scanline */
		if( (RowSize % 4) != 0 )
		{
			padding = 4 - (RowSize % 4);
		}



		/* allocate memory for the pixel data */
		*pixels = (unsigned char*) malloc( (*h) * RowSize );
		assert(*pixels);

		for( i=0; i != *h; ++i )
		{
			unsigned char* end;
			/* get a pointer to the start of the scanline in expanded data */
			unsigned char* pline = ( (*pixels) + ( RowSize * i ) );
			unsigned int j;

			/* read the scanline including padding */
			fread( pline, RowSize, 1, fp );

			end   = pline + RowSize;

			/* flip scanline from BGR to RGB */
			for( ; pline != end ; pline += bdepth )
			{
				unsigned char temp  = *pline;
				pline[0] = pline[2];
				pline[2] = temp;
			}

			/* now read past padding */
			for( j=0; j!= padding; ++j )
			{
				fgetc(fp);
			}
		}
	}

	fclose(fp);

	return 1;
}
