/**************************************************

CDvrWin:

This file defines a class for apply direct volume rendering (DVR) to a 
regular 3D volume. 

It is a subclass of CGlutWin, a class for creating GLUT/GLUI-based
display window. 

The proecduer to use this class is as below:

1. declare a object of class CDvrWin.
2. Set up a volume and a transfer function before entering the GLinUT main loop. 
(NOTE: both the volume and transfer function can be alternated later)

**************************************************/

										// remove the libraries
	#ifdef WITH_STATIC_LINK
	#pragma comment (lib, "glew32s.lib")      
	#else	// #ifdef WITH_STATIC_LINK
	#pragma comment (lib, "glew32.lib")      
	#endif	// #ifdef WITH_STATIC_LINK		

	#ifdef _DEBUG
		#pragma comment (lib, "libfbo_d.lib")
		#pragma comment (lib, "libshader_d.lib")
	#else
		#pragma comment (lib, "libfbo_r.lib")
		#pragma comment (lib, "libshader_r.lib")
	#endif

#pragma once
#include "GlutWin.h"

class CDvrWin :
	public CGlutWin
{
protected:
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

protected:

	enum {
		WINDOW_SIZE	= 0x10
	};

	// inhered interfaces
	void _DisplayFunc();
	void _ReshapeFunc(int, int);
	void _KeyboardFunc(unsigned char, int, int);
	void _SpecialFunc(int, int, int);

	void _InitFunc();

	// new methods
	void _DisplayVolume();

	void _GluiFunc(unsigned short usValue);
							// variable for setting the window resolution
	GLUI_Spinner *pcWidth_spinner;
	GLUI_Spinner *pcHeight_spinner;
	int iWindowWidth;	
	int iWindowHeight;	

public:
	// texture handle
	GLuint t3dVol;
	GLuint t1dTf;
	GLuint t2dScreenColor;
	GLuint t2dScreenDepth;

	// shader object handles
	GLuint uPidTf1d;	// the handle to the shader that implement 1D transfer function classification

	// FBO handle
	GLuint uFidScreen;

	// #slices
	int iNrOfSlices;

	// #max slices
	int iMaxNrOfSlices;

	// dim. of the volume
	int iXDim, iYDim, iZDim;

	// the gain of the thickness
	float fThicknessRatio;

	// a flag to indicate if FBO is used
	int ibRenderToFbo;

	void _SetVolume(GLenum eInternalFormat, const void *pVol, GLenum eType, GLenum eFormat, int iXDim, int iYDim, int iZDim);
	void _SetTransferFunc(const void *pTf, GLenum eType, GLenum eFormat, int iNrOfTfEntries);

public:
	CDvrWin(void);
public:
	~CDvrWin(void);
};

