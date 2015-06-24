#ifndef __FBO__H__
#define __FBO__H__

#include "libopengl.h"

template<GLenum eDefaultInternalFormat, GLenum eDefaultFormat, GLenum eTexTarget = GL_TEXTURE_2D>
struct CFrameBufferTexture{
	GLenum eTarget;
	GLuint t2d;		// id to the texture
	GLenum eInternalFormat;	// the internal format of the texture
	GLenum eFormat;			// the external format of the texture

	CFrameBufferTexture()
	{
		eTarget = eTexTarget;
		t2d = 0;
		eInternalFormat = eDefaultInternalFormat;
		eFormat = eDefaultFormat;
	}

	void _SetResolution(int w, int h)
	{
		glBindTexture(eTarget, t2d);
		glTexImage2D(eTarget, 0, eInternalFormat,
			w, h, 0, eFormat, GL_FLOAT, NULL);	
	}

	void _Create()
	{
		CREATE_2D_TEXTURE(
			eTarget, t2d, 
			GL_NEAREST, eInternalFormat, 
			128, 128, // the real resolution will be determined later
			eFormat, GL_FLOAT, NULL);
	}
};

GLenum EGetFboStatus(bool bPrint=false);

#endif // __FBO__H__

