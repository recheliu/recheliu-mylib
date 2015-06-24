#ifndef __LIB_OPENGL_LIB_H_
#define __LIB_OPENGL_LIB_H_

#if	defined(USE_FREEGLUT)
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

	#include "libbuf.h"

						// incldue the CUDA's vector types

	template<typename VT, typename CT, typename TT>
	struct CVectorArray {
		TBuffer<VT> pvCoords;
		TBuffer<CT> pvColors;
		TBuffer<TT> pvTexCoords;
	};

// Definition of macros
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
			(WIDTH), (HEIGHT), (DEPTH), 0, (FORMAT), (TYPE), (PTR));	\
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
				fprintf(stderr, "%s@%s(%d): %s\n", __FUNCTION__, __FILE__, __LINE__, gluErrorString(iError));	\
				if( terminate )	exit(-iError);	\
			}\
		}

namespace GlHelper {
	inline 
	void 
	_DrawString3D(const char *szString, float fX, float fY, float fZ)		
	{
		glPushMatrix();

		// normalize the coordinate to be window coordinate
		glTranslatef(fX, fY, fZ);

		glRasterPos3f(0.0, 0.0, 0.0);
		for(const char *pc = szString; *pc; pc++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *pc);

		glPopMatrix();
	}
};

#endif // __LIB_OPENGL_LIB_H_

