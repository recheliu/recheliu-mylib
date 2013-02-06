#pragma once
// ADD-BY-LEETEN 09/13/2011-BEGIN
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#else
// ADD-BY-LEETEN 09/13/2011-END
#include <GL/gl.h>
#endif	// ADD-BY-LEETEN 09/13/2011

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

// ADD-BY-LEETEN 04/06/2010-BEGIN
protected:
	float	fThicknessGain;
	GLuint t1dTf;
	float fTfDomainMin, fTfDomainMax;
	float fDataValueMin, fDataValueMax;
public:
	void _SetTfDomain(float fMin, float fMax)
	{
		fTfDomainMin = fMin;
		fTfDomainMax = fMax;
	}
	void _SetDataValue(float fMin, float fMax)
	{
		fDataValueMin = fMin;
		fDataValueMax = fMax;
	}
	void _SetTransferFunc(const void *pTf, GLenum eType, GLenum eFormat, int iNrOfTfEntries);

protected:
	GLuint t3dVol;
	int iXDim, iYDim, iZDim;
public:
	void _SetVolume(GLenum eInternalFormat, const void *pVol, GLenum eType, GLenum eFormat, int iXDim, int iYDim, int iZDim);
// ADD-BY-LEETEN 04/06/2010-END

							// these 2 functions can be used by the inheriting classes 
							// to set up the variables for customized volume rendering

							// NOTE: GL_TEXTURE0 is reserved by CDvrWin2 to generate
							// the texture coordinate and as the source for ping-pong rendering
	virtual void _BeginDisplay();	
	virtual void _EndDisplay();		
	// ADD-BY-LEETEN 12/30/2009-BEGIN
	virtual void 
		_RenderSlab(
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
Revision 1.5  2011-01-25 02:41:50  leeten

[01/24/2011]
1. [DEL] Solve compilation error.

Revision 1.4  2010/04/06 19:56:56  leeten

[04/06/2010]
1. [ADD] Defien new methods _SetTfDomain and _SetDataValue to specify the domain of the transfer function.

Revision 1.3  2010/04/06 19:54:02  leeten

[04/06/2010]
1. [ADD] Define a new method _SetVolume to upload the specified volume as a 3D texture.
2. [ADD] Define a new method _SetTransferFunc to specify the transfer function.

Revision 1.2  2009/12/31 01:40:08  leeten

[12/30/2009]
1. [ADD] Declare a new method _RenderSlab() to render a slab.

Revision 1.1  2009/10/27 14:04:07  leeten

[2009/10/27]
1. [1ST] First time checkin. This library defined DvrWin2, a new class for volume rendering. This class already supports buildit-in ping-pong rendering.


*/
