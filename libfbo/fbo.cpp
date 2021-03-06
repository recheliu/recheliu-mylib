#include <GL/glew.h>
#include <GL/glut.h>

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
		}
	}
	return eStatus;
}

