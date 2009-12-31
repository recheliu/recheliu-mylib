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
	// ADD-BY-LEETEN 12/30/2009-BEGIN
	virtual void 
		CDvrWin2::_RenderSlab(
			int iSlab, int iNrOfSlabs,
			double pdModelviewMatrix[], double pdProjectionMatrix[], int piViewport[],
			double dMinX, double dMaxX, 
			double dMinY, double dMaxY, 
			double dMinZ, double dMaxZ);
	// ADD-BY-LEETEN 12/30/2009-END

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
Revision 1.1  2009/10/27 14:04:07  leeten

[2009/10/27]
1. [1ST] First time checkin. This library defined DvrWin2, a new class for volume rendering. This class already supports buildit-in ping-pong rendering.


*/
