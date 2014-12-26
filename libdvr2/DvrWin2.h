#pragma once
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif	

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

							// these 2 functions can be used by the inheriting classes 
							// to set up the variables for customized volume rendering

							// NOTE: GL_TEXTURE0 is reserved by CDvrWin2 to generate
							// the texture coordinate and as the source for ping-pong rendering
	virtual void _BeginDisplay();	
	virtual void _EndDisplay();		
	virtual void 
		_RenderSlab(
			int iSlab, int iNrOfSlabs,
			double pdModelviewMatrix[], double pdProjectionMatrix[], int piViewport[],
			double dMinX, double dMaxX, 
			double dMinY, double dMaxY, 
			double dMinZ, double dMaxZ);

						// inhered interfaces
	void _DisplayFunc();
	void _ReshapeFunc(int, int);

	void _InitFunc();

public:
	CDvrWin2(void);
	virtual ~CDvrWin2(void);
};

