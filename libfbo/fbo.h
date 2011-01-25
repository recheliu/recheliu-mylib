#ifndef __FBO__H__
#define __FBO__H__

// ADD-BY-LEETEN 01/24/2011-BEGIN
#include "libopengl.h"
// ADD-BY-LEETEN 01/24/2011-END

// ADD-BY-LEETEN 2009/11/05-BEGIN

template<GLenum eDefaultInternalFormat, GLenum eDefaultFormat, GLenum eTexTarget = GL_TEXTURE_2D>
struct CFrameBufferTexture{
	// ADD-BY-LEETEN 2009/08/24-BEGIN
	GLenum eTarget;
	// ADD-BY-LEETEN 2009/08/24-END
	GLuint t2d;		// id to the texture
	GLenum eInternalFormat;	// the internal format of the texture
	GLenum eFormat;			// the external format of the texture

	CFrameBufferTexture()
	{
		// ADD-BY-LEETEN 2009/08/24-BEGIN
		eTarget = eTexTarget;
		// ADD-BY-LEETEN 2009/08/24-END
		t2d = 0;
		eInternalFormat = eDefaultInternalFormat;
		eFormat = eDefaultFormat;
	}

	// ADD-BY-LEETEN 2009/08/24-BEGIN
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
	// ADD-BY-LEETEN 2009/08/24-END
};

// ADD-BY-LEETEN 2009/11/05-END

GLenum EGetFboStatus(bool bPrint=false);

#endif // __FBO__H__

/*

  $Log: not supported by cvs2svn $
  Revision 1.2  2009/11/09 20:21:14  leeten

  [2009/11/09]
  1. [ADD] Add the new structure CFrameBufferTexture.

  Revision 1.1.1.1  2006/11/09 19:22:31  leeten

  [11/09/2006]
  1. First time checkin.

  Revision 1.1.1.1  2006/09/13 19:49:26  leeten

  [09/13/2006]
  1. First time checkin. It is a new version renderere which simulate 4 slices at the same time.


*/
