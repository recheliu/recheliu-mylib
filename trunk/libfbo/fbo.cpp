#if	0	// MOD-BY-LEETEN-2009/06/01-FROM:
	#include <gl/glew.h>
	#include <gl/glut.h>
#else		// MOD-BY-LEETEN-2009/06/01-TO:
#include <GL/glew.h>
#include <GL/glut.h>
#endif		// MOD-BY-LEETEN-2009/06/01-END

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLenum
EGetFboStatus(bool bPrint)
{
	GLenum eStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if( !bPrint ) 
		return eStatus;
		
	switch(eStatus) {
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		fprintf(stderr, "GL_FRAMEBUFFER_COMPLETE_EXT\n");
		break;
	default:
		switch(eStatus) {
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n");
			break;
		// DEL-BY-LEETY 09/05/2006-BEGIN
		/*
		case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_DUPLICATE_ATTACHEMENT_EXT\n");
			break;
		*/
		// DEL-BY-LEETY 09/05/2006-END
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_DIMENSION_EXT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_FORMATS_EXT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_DRAW_BUFFER_EXT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n");
			break;
		// DEL-BY-LEETY 09/05/2006-BEGIN
		/*
		case GL_FRAMEBUFFER_STATUS_ERROR_EXT:
			fprintf(stderr, "GL_FRAMEBUFFER_STATUS_ERROR\n");
			break;
		*/
		// DEL-BY-LEETY 09/05/2006-END
		}
	}
	return eStatus;
}

/*

  $Log: not supported by cvs2svn $
  Revision 1.1.1.1  2006/11/09 19:22:31  leeten

  [11/09/2006]
  1. First time checkin.

  Revision 1.1.1.1  2006/09/13 19:49:26  leeten

  [09/13/2006]
  1. First time checkin. It is a new version renderere which simulate 4 slices at the same time.


*/
