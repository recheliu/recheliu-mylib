#ifndef		__GLUT_WINDOW_H__
#define		__GLUT_WINDOW_H__


#pragma comment (lib, "winmm.lib")      /* link with Windows MultiMedia lib */

#if USING_FREEGLUT
	#include <GL/freeglut.h>
	#pragma comment (lib, "freeglut.lib")
#else
	#include <GL/glut.h>
	#pragma comment (lib, "glut32.lib")   
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#ifdef NDEBUG
	#undef assert
	#define assert(expr) \
	{\
		if( !(expr) ) \
		{	\
		fprintf(stderr, "%s in %s (%d):", #expr, __FILE__, __LINE__);\
			perror("");	\
			exit(-1);	\
		}\
	}\
		
#else
	#include <assert.h>
#endif

#define M_PI		(3.14159)
#define M_PI_2		(M_PI / 2.0)

#include "libopengl.h"

class CGlutWindow {
	typedef GLdouble TMatrix[16];

	// the window id
	int iId;

	// title
	char szTitle[1024+1];
	char szReshape[1024+1];
	char szDisplay[1024+1];
	char szInit[1024+1];

	// the viewport
	int piViewport[4];

	// the model, view, and projection matrices
	TMatrix tViewMatrix, tInitViewMatrix;
	TMatrix tModelMatrix, tInitModelMatrix;
	TMatrix tProjectionMatrix;

	// the different between the origin of the model and the cursor the on screen.
	// it is used to adjust the shift of the model
	double dXDiffOnScreen, dYDiffOnScreen;	

	// GLUT Callbacks 

	// the callback to release resource
	void (*QuitFunc)();

	///////////////////////////////////////////////////////////////
	// The callbacks, functions and vairable for the mouse interfaces.
	// The reference point of every kind of motion is the point where the mouse is just clicked.
	int iBeginX, iBeginY; // the 2D coordinate of the cursor
	int iCursorX, iCursorY; // the 2D coordinate of the cursor
	GLenum eMouseButton;
	GLenum eModifier;
	bool bMoving;

	double pdOldCoord[3], pdNewCoord[3];
	double pdCurPos[3], pdBeginPos[3];
	double dX, dY, dZ;

	void _AlignPan(double pdNewCoord[3], double pdOldCoord[3]);
	void _AlignRotate(double pdAxis[3]);

	void _RotateCamera();
	void _PanCamera();
	void _ZoomCamera();

	void _PanModel();
	void _RotateModel();
	void _ZoomModel();

	// interfaces
protected:
	virtual void _DisplayFunc() = 0;
	virtual void _ReshapeFunc(int, int) = 0;
	virtual void _KeyboardFunc(unsigned char, int, int) = 0;
	virtual void _SpecialFunc(int, int, int) = 0;
	virtual void _IdleFunc() = 0;

	virtual void _InitFunc() = 0;
	virtual void _ExitFunc() = 0;

public:
	void _DisplayCB();
	void _ReshapeCB(int w, int h);
	void _KeyboardCB(unsigned char key, int x, int y);
	void _SpecialCB(int skey, int x, int y);
	void _MotionCB(int x, int y);
	void _MouseCB(int button, int state, int x, int y);
	void _IdleCB();

	// return the window ID
	int IGetId();

	int ICreate(
		char *szTitle, 
		int iX, int iY, int iW, int iH);

	CGlutWindow();
	~CGlutWindow();
};

void _GWAddWindow(CGlutWindow *win, char *szTitle, int x, int y, int w, int h);

#endif	//	__GLUT_WINDOW_H__

/*

$Log: not supported by cvs2svn $

*/
