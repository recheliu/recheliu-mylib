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
	// MOD-BY-LEETEN 08/05/2010-FROM:
		// #pragma comment (lib, "libfbo.lib")      // link with my own library libfps
	// TO:
	#ifdef _DEBUG
		#pragma comment (lib, "libfbo_d.lib")      // link with my own library libfps
	#else
		#pragma comment (lib, "libfbo_r.lib")      // link with my own library libfps
	#endif
	// MOD-BY-LEETEN 08/05/2010-END


	#include <GL/glew.h>

	#include "GlutWin.h"

	// ADD-BY-LEETEN 2009/11/05-BEGIN
	#include "fbo.h"
	// ADD-BY-LEETEN 2009/11/05-END

class CFboWin :
	public CGlutWin
{
protected:
				// id to the FBO
	GLuint fid;	

	#if	0	// DEL-BY-LEETEN 2009/11/05-BEGIN
		// MOD-BY-LEETEN 2009/08/24-FROM:
			// template<GLenum eDefaultInternalFormat, GLenum eDefaultFormat>
		// TO:
		template<GLenum eDefaultInternalFormat, GLenum eDefaultFormat, GLenum eTexTarget = GL_TEXTURE_2D>
		// MOD-BY-LEETEN 2009/08/24-END

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
	#endif	// DEL-BY-LEETEN 2009/11/05-END

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
			#if	0	// MOD-BY-LEETEN 2009/08/24-FROM:
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
			#else	// MOD-BY-LEETEN 2009/08/24-TO:
			cColor._Create();
			cDepth._Create();
			#endif	// MOD-BY-LEETEN 2009/08/24-END

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
			// ADD-BY-LEETEN 2009/08/24-BEGIN
			glPushAttrib(GL_ENABLE_BIT);
			// ADD-BY-LEETEN 2009/08/24-END

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			#if	0	// MOD-BY-LEETEN 2009/08/24-FROM:
				glBindTexture(GL_TEXTURE_2D, cColor.t2d);
				glEnable(GL_TEXTURE_2D);
			#else	// MOD-BY-LEETEN 2009/08/24-TO:
			glBindTexture(cColor.eTarget, cColor.t2d);
			glEnable(cColor.eTarget);
			#endif	// MOD-BY-LEETEN 2009/08/24-END

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
			// MOD-BY-LEETEN 2009/08/24-FROM:
				//	glDisable(GL_TEXTURE_2D);
			// TO:
			glPopAttrib();	// glPushAttrib(GL_ENABLE_BIT);
			// MOD-BY-LEETEN 2009/08/24-END
		} // if 
	}

	void _ReshpaeFbo(int w, int h)
	{
		if( w && h )
		{
			if( ibIsFboEnabled )
			{
				#if	0	// MOD-BY-LEETEN 2009/08/24-FROM:
					glBindTexture(GL_TEXTURE_2D, cColor.t2d);
					glTexImage2D(GL_TEXTURE_2D, 0, cColor.eInternalFormat,
						w, h, 0, cColor.eFormat, GL_FLOAT, NULL);	

					glBindTexture(GL_TEXTURE_2D, cDepth.t2d);
					glTexImage2D(GL_TEXTURE_2D, 0, cDepth.eInternalFormat,
						w, h, 0, cDepth.eFormat, GL_FLOAT, NULL);	
				#else	// MOD-BY-LEETEN 2009/08/24-TO:
				cColor._SetResolution(w, h);
				cDepth._SetResolution(w, h);
				#endif	// MOD-BY-LEETEN 2009/08/24-END

				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fid);
				#if	0	// MOD-BY-LEETEN 2009/08/24-FROM:
					glFramebufferTexture2DEXT(
						GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
						GL_TEXTURE_2D, cColor.t2d, 0);

					glFramebufferTexture2DEXT(
						GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
						GL_TEXTURE_2D, cDepth.t2d, 0);
				#else	// MOD-BY-LEETEN 2009/08/24-TO:
				glFramebufferTexture2DEXT(
					GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
					cColor.eTarget, cColor.t2d, 0);

				glFramebufferTexture2DEXT(
					GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
					cColor.eTarget, cDepth.t2d, 0);
				#endif	// MOD-BY-LEETEN 2009/08/24-END

				// MOD-BY-LEETEN 08/05/2010-FROM:
					// assert( GL_FRAMEBUFFER_COMPLETE_EXT == EGetFboStatus(true) );
				// TO:
				unsigned int iFboStatus = EGetFboStatus(true);
				assert( GL_FRAMEBUFFER_COMPLETE_EXT == iFboStatus );
				// MOD-BY-LEETEN 08/05/2010-END

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
Revision 1.3  2009/11/09 20:22:12  leeten

[2009/11/09]
1. [DEL] Move the structure CFrameBufferTexture to fbo.h.
2. [ADD] Incldue the header fbo.h.

Revision 1.2  2009/11/04 20:49:59  leeten

[2009/11/04]
1. [ADD] Define new methods _SetResolution() to specify the resolution of the FBO.
2. [ADD] Define new methods _Create() to allocate the FBO.

Revision 1.1  2009/03/09 19:28:12  leeten

[2009/03/09]
1. [1ST] This project define a subclass CFobWin inheriting from CGlutWin, which defines a GLUT window with FBO support.


*/
