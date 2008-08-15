/**************************************************

CDvrWin:

This file defines a class for apply direct volume rendering (DVR) to a 
regular 3D volume. 

It is a subclass of CGlutWin, a class for creating GLUT/GLUI-based
display window. 

The proecduer to use this class is as below:

1. declare a object of class CDvrWin.
2. Set up a volume and a transfer function before entering the GLUT main loop. 
(NOTE: both the volume and transfer function can be alternated later)

**************************************************/

										// remove the libraries
#if	0	// DEL-BY-LEETEN 2008/08/14-BEGIN
	#pragma comment (lib, "glew32.lib")      
	#pragma comment (lib, "libfbo.lib")      
	#pragma comment (lib, "libshader.lib")      
	#pragma comment (lib, "libdvr.lib")      
#endif	// DEL-BY-LEETEN 2008/08/14-END

#pragma once
#include "GlutWin.h"

class CDvrWin :
	public CGlutWin
{
	// inhered interfaces
	void _DisplayFunc();
	void _ReshapeFunc(int, int);
	void _KeyboardFunc(unsigned char, int, int);
	void _SpecialFunc(int, int, int);

	#if	0	// DEL-BY-LEETEN 08/14/2008-BEGIN
		void _IdleFunc();
	#endif	// DEL-BY-LEETEN 08/14/2008-END

	void _InitFunc();

	// new methods
	void _DisplayVolume();

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

	void _SetVolume(const void *pVol, GLenum eType, GLenum eFormat, int iXDim, int iYDim, int iZDim);
	void _SetTransferFunc(const void *pTf, GLenum eType, GLenum eFormat, int iNrOfTfEntries);

public:
	CDvrWin(void);
public:
	~CDvrWin(void);
};

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/14 22:54:48  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN] This is a library to create a GLUTWIN window for direct volume rendering (DVR).

Revision 1.1.1.1  2008/08/12 16:58:49  leeten

[2008/08/12]
1. First time checkin. This is my DVR renderer for 3D regular volume. For testing, the dataset HIPIP is checkin too.


*/

