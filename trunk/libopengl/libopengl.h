#ifndef __LIB_OPENGL_LIB_H_
#define __LIB_OPENGL_LIB_H_

// Definition of macros
#define CREATE_2D_TEXTURE(TARGET, TID, FILTER, INT_FORMAT, WIDTH, HEIGHT, FORMAT, TYPE, PTR)	\
		{\
		glGenTextures(1, &(TID));	\
		glBindTexture(TARGET, TID);	\
		glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	\
		glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	\
		glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, FILTER);	\
		glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, FILTER);	\
		glTexImage2D(TARGET, 0, INT_FORMAT,	\
			(WIDTH), (HEIGHT), 0, (FORMAT), (TYPE), (PTR));	\
		}

#define CHECK_OPENGL_ERROR(prefix, terminate)	\
		{\
			GLint iError = glGetError();	\
			if( iError )	\
			{\
				fprintf(stderr, "%s: %s\n", prefix, gluErrorString(iError));	\
				if( terminate )	exit(-iError);	\
			}\
		}

// Declaration of functions
GLenum EGetFboStatus(bool bPrint=false);

#endif // __LIB_OPENGL_LIB_H_

/*

$Log: not supported by cvs2svn $
Revision 1.1  2006/11/26 00:08:55  leeten

[11/25/2006]
1. First time checkin.


*/
