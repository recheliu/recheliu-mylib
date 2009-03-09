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
	#pragma comment (lib, "glew32.lib")      // link with my own library libfps
	#pragma comment (lib, "libfbo.lib")      // link with my own library libfps

	#include <GL/glew.h>

	#include "GlutWin.h"

class CFboWin :
	public CGlutWin
{
protected:
				// id to the FBO
	GLuint fid;	

	template<GLenum eDefaultInternalFormat, GLenum eDefaultFormat>
	struct CFrameBufferTexture{
		GLuint t2d;		// id to the texture
		GLenum eInternalFormat;	// the internal format of the texture
		GLenum eFormat;			// the external format of the texture

		CFrameBufferTexture()
		{
			t2d = 0;
			eInternalFormat = eDefaultInternalFormat;
			eFormat = eDefaultFormat;
		}
	};

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
			CREATE_2D_TEXTURE(
				GL_TEXTURE_2D, cColor.t2d, 
				GL_NEAREST, cColor.eInternalFormat, 
				128, 128, // the real resolution will be determined later
				cColor.eFormat, GL_FLOAT, NULL);

			CREATE_2D_TEXTURE(
				GL_TEXTURE_2D, cDepth.t2d, 
				GL_NEAREST, cDepth.eInternalFormat,
				128, 128, // the real resolution will be determined later
				cDepth.eFormat, GL_FLOAT, NULL); 

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
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, cColor.t2d);
			glEnable(GL_TEXTURE_2D);

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
			glDisable(GL_TEXTURE_2D);
		} // if 
	}

	void _ReshpaeFbo(int w, int h)
	{
		if( w && h )
		{
			if( ibIsFboEnabled )
			{
				glBindTexture(GL_TEXTURE_2D, cColor.t2d);
				glTexImage2D(GL_TEXTURE_2D, 0, cColor.eInternalFormat,
					w, h, 0, cColor.eFormat, GL_FLOAT, NULL);	

				glBindTexture(GL_TEXTURE_2D, cDepth.t2d);
				glTexImage2D(GL_TEXTURE_2D, 0, cDepth.eInternalFormat,
					w, h, 0, cDepth.eFormat, GL_FLOAT, NULL);	

				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fid);
				glFramebufferTexture2DEXT(
					GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
					GL_TEXTURE_2D, cColor.t2d, 0);

				glFramebufferTexture2DEXT(
					GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
					GL_TEXTURE_2D, cDepth.t2d, 0);

				assert( GL_FRAMEBUFFER_COMPLETE_EXT == EGetFboStatus(true) );
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

/*

$Log: not supported by cvs2svn $

*/
