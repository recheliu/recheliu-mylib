#ifndef __LIB_OPENGL_LIB_H_
#define __LIB_OPENGL_LIB_H_

// ADD-BY-LEETEN 2009/01/18-BEGIN
	#include "libbuf.h"

						// incldue the CUDA's vector types
	#include <vector_types.h>

	template<typename VT, typename CT, typename TT>
	typedef struct CVectorArray {
		TBuffer<VT> pvCoords;
		TBuffer<CT> pvColors;
		TBuffer<TT> pvTexCoords;
	} CVectorArray ;

// ADD-BY-LEETEN 2009/01/18-END

// Definition of macros
// ADD-BY-LEETEN 2009/01/18-BEGIN
#define CREATE_3D_TEXTURE(TARGET, TID, FILTER, INT_FORMAT, WIDTH, HEIGHT, DEPTH, FORMAT, TYPE, PTR)	\
		{\
		glGenTextures(1, &(TID));	\
		glBindTexture(TARGET, TID);	\
		glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP);	\
		glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_CLAMP);	\
		glTexParameteri(TARGET, GL_TEXTURE_WRAP_R, GL_CLAMP);	\
		glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, FILTER);	\
		glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, FILTER);	\
		glTexImage3D(TARGET, 0, INT_FORMAT,	\
			(WIDTH), (HEIGHT), (DEPTH) 0, (FORMAT), (TYPE), (PTR));	\
		}

#define CREATE_1D_TEXTURE(TARGET, TID, FILTER, INT_FORMAT, WIDTH, FORMAT, TYPE, PTR)	\
		{\
		glGenTextures(1, &(TID));	\
		glBindTexture(TARGET, TID);	\
		glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP);	\
		glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, FILTER);	\
		glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, FILTER);	\
		glTexImage1D(TARGET, 0, INT_FORMAT,	\
			(WIDTH), 0, (FORMAT), (TYPE), (PTR));	\
		}
// ADD-BY-LEETEN 2009/01/18-END

#define CREATE_2D_TEXTURE(TARGET, TID, FILTER, INT_FORMAT, WIDTH, HEIGHT, FORMAT, TYPE, PTR)	\
		{\
		glGenTextures(1, &(TID));	\
		glBindTexture(TARGET, TID);	\
		glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_CLAMP);	\
		glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_CLAMP);	\
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
Revision 1.3  2008/08/15 15:08:25  leeten

[2008/08/15]
1. [CHANGE] In CREATE_2D_TEXTURE, use GL_CLAMP instead GL_CLAMP_TO_EDGE since GL_CLAMP_TO_EDGE is only defined in GLEW.

Revision 1.2  2008/02/23 03:02:33  leeten

[02/22/2008]
1. Chnage the clamp method of 2D texture from GL_CLAMP to GL_CLAMP_TO_EDGE.

Revision 1.1  2006/11/26 00:08:55  leeten

[11/25/2006]
1. First time checkin.


*/
