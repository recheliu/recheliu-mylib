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
// ADD-BY-LEETEN 2008/08/16-BEGIN
	#pragma comment (lib, "glew32.lib")      
	#pragma comment (lib, "libfbo.lib")      
	#pragma comment (lib, "libshader.lib")      
// ADD-BY-LEETEN 2008/08/16-END

#pragma once
#include "GlutWin.h"

class CDvrWin :
	public CGlutWin
{
	// ADD-BY-TLEE 2008/08/17-BEGIN
	enum {
		WINDOW_SIZE	= 0x10
	};
	// ADD-BY-TLEE 2008/08/17-END

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

	// ADD-BY-TLEE 2008/08/17-BEGIN
	void _GluiFunc(unsigned short usValue);
							// variable for setting the window resolution
	GLUI_Spinner *pcWidth_spinner;
	GLUI_Spinner *pcHeight_spinner;
	int iWindowWidth;	
	int iWindowHeight;	
	// ADD-BY-TLEE 2008/08/17-END

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

	// MOD-BY-TLEE 2008/08/20-FROM:
		// void _SetVolume(const void *pVol, GLenum eType, GLenum eFormat, int iXDim, int iYDim, int iZDim);
	// TO:
	void _SetVolume(GLenum eInternalFormat, const void *pVol, GLenum eType, GLenum eFormat, int iXDim, int iYDim, int iZDim);
	// MOD-BY-TLEE 2008/08/20-END
	void _SetTransferFunc(const void *pTf, GLenum eType, GLenum eFormat, int iNrOfTfEntries);

public:
	CDvrWin(void);
public:
	~CDvrWin(void);
};

/*

$Log: not supported by cvs2svn $
Revision 1.4  2008/08/17 23:48:31  leeten

[2008/08/17]
1. [ADD] Add 2 spinners to control the window size. To handle this event, the method _Gluifunc is thus overloaded, and a new event index WINDOW_SIZE is added.

Revision 1.3  2008/08/16 21:32:23  leeten

[2008/08/16]
1. [Add] implictly link the following libs: glew32.lib, libfbo.lib and libshader.lib.

Revision 1.2  2008/08/15 14:31:19  leeten

[2008/08/15]
1. [DEL] Remove the pragma to implictly link the libraries in mylib since it can cause VC not to check the dependency for those library.

Revision 1.1.1.1  2008/08/14 22:54:48  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN] This is a library to create a GLUTWIN window for direct volume rendering (DVR).

Revision 1.1.1.1  2008/08/12 16:58:49  leeten

[2008/08/12]
1. First time checkin. This is my DVR renderer for 3D regular volume. For testing, the dataset HIPIP is checkin too.


*/

