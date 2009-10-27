#pragma once
#include <GL/gl.h>

	#include "FboWin.h"

class CDvrWin2 :
	public CFboWin
{
protected:
	enum {
		MAX_NR_OF_SLICES = 1024,
	};
	int iNrOfSlices;	// #slices

	struct CSlab {
		CFrameBufferTexture<
			GL_RGBA32F_ARB,
			GL_RGBA> cBuffer;
		GLuint fid;
	};

	CSlab pcSlabs[2];

							// these 2 functions can be used by the inheriting classes 
							// to set up the variables for customized volume rendering

							// NOTE: GL_TEXTURE0 is reserved by CDvrWin2 to generate
							// the texture coordinate and as the source for ping-pong rendering
	virtual void _BeginDisplay();	
	virtual void _EndDisplay();		

						// inhered interfaces
	void _DisplayFunc();
	void _ReshapeFunc(int, int);

	void _InitFunc();

public:
	CDvrWin2(void);
	virtual ~CDvrWin2(void);
};

/*

$Log: not supported by cvs2svn $

*/
