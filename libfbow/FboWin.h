/*

FboWin: 

This is a subclass inheriting from CGlutWin. This 
class defines an OpenGL window that support FBO. 

The reason why this functionality is not defined in 
CGlutWin is because that FBO requires extra libraries 
such LIBFBO and glew, which might be not needed for some
existing projects.

*/

	#pragma once
	#ifdef			WITH_STATIC_LINK
	#pragma comment (lib, "glew32s.lib")      // link with my own library libfps
	#else	// #ifdef	WITH_STATIC_LINK
	#pragma comment (lib, "glew32.lib")      // link with my own library libfps
	#endif	//#ifdef	WITH_STATIC_LINK	
	#ifdef _DEBUG
		#pragma comment (lib, "libfbo_d.lib")      // link with my own library libfps
	#else
		#pragma comment (lib, "libfbo_r.lib")      // link with my own library libfps
	#endif


	#include <GL/glew.h>

	#include "GlutWin.h"

	#include "fbo.h"

class CFboWin :
	public CGlutWin
{
protected:
				// id to the FBO
	GLuint fid;	

	CFrameBufferTexture<
		GL_RGBA, 
		GL_RGBA> cColor;

	CFrameBufferTexture<
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT> cDepth;

public:
	int ibIsFboEnabled;

	void _SetInternalColorFormat(GLenum eFormat)
	{
		cColor.eInternalFormat = eFormat;
	}

	void _SetInternalDepthFormat(GLenum eFormat)
	{
		cDepth.eInternalFormat = eFormat;
	}

	void _InitFbo()
	{
		if( ibIsFboEnabled )
		{
			cColor._Create();
			cDepth._Create();

			glGenFramebuffersEXT(1, &fid);	// allocate framebuffer object
		}
	}

	void _EnableFbo()
	{
		if( ibIsFboEnabled )
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fid);
	}

	void _DisableFbo()
	{
		if( ibIsFboEnabled )
		{
			glPushAttrib(GL_ENABLE_BIT);

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(cColor.eTarget, cColor.t2d);
			glEnable(cColor.eTarget);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);	glVertex2f(-1.0f, -1.0f);
				glTexCoord2f(1.0f, 0.0f);	glVertex2f( 1.0f, -1.0f);
				glTexCoord2f(1.0f, 1.0f);	glVertex2f( 1.0f,  1.0f);
				glTexCoord2f(0.0f, 1.0f);	glVertex2f(-1.0f,  1.0f);
			glEnd();
			glPopAttrib();	// glPushAttrib(GL_ENABLE_BIT);
		} // if 
	}

	void _ReshpaeFbo(int w, int h)
	{
		if( w && h )
		{
			if( ibIsFboEnabled )
			{
				cColor._SetResolution(w, h);
				cDepth._SetResolution(w, h);

				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fid);
				glFramebufferTexture2DEXT(
					GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
					cColor.eTarget, cColor.t2d, 0);

				glFramebufferTexture2DEXT(
					GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
					cColor.eTarget, cDepth.t2d, 0);

				unsigned int iFboStatus = EGetFboStatus(true);
				assert( GL_FRAMEBUFFER_COMPLETE_EXT == iFboStatus );

				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			}
		}
	}

	void _ReshapeFunc(int w, int h)
	{
		_ReshpaeFbo(w, h);
	}

	CFboWin(void)
	{
		ibIsFboEnabled = (int)false;
		fid = 0;
	}

	virtual ~CFboWin(void)
	{
	}
};

