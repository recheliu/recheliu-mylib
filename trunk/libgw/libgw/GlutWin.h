#ifndef		__GLUT_WINDOW_H__
#define		__GLUT_WINDOW_H__

// ADD-BY-LEETEN 08/09/2008-BEGIN
#include <vector>
using namespace std;
// ADD-BY-LEETEN 08/09/2008-END

#pragma comment (lib, "winmm.lib")      /* link with Windows MultiMedia lib */

#if		defined(_WIN32)
	#define GLUT_BUILDING_LIB	
	// DEL-BY-LEETEN 08/09/2008-BEGIN
		// #define _CRT_SECURE_NO_DEPRECATE
	// DEL-BY-LEETEN 08/09/2008-END
#endif

#if		defined(USING_FREEGLUT)
	#include <GL/freeglut.h>
	#pragma comment (lib, "freeglut.lib")
#else
	#include <GL/glut.h>
	#pragma comment (lib, "glut32.lib")   
#endif

	// ADD-BY-LEETEN 08/11/2008-BEGIN
	// combine w/ GLUI
	#include <GL/glui.h>
	// ADD-BY-LEETEN 08/11/2008-END

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

class CGlutWin {

// ADD-BY-LEETEN 08/11/2008-BEGIN

protected:
	// store the GLUI option to add a GLUI win and/or a GLUI subwin
	int iGluiEnum;

	// pointer to the created GLUI window
	GLUI *pcGluiWin;

	// pointer to the created GLUI sub-window
	GLUI *pcGluiSubwin;

	// location of the sub-window
	int iSubwinPosistion;

public:
	// define the option of GLUI windows as enum
	typedef enum {
		GLUI_NONE	= 0x0000,
		GLUI_WIN	= 0x0001,
		GLUI_SUBWIN	= 0x0002
	} EGluiEnum;

	// add a GLUI window 
	void _AddGluiWin();

	// return the pointer to the GLUI window
	GLUI *PCGetGluiWin();

	// add a GLUI subwindow in the specified position
	void _AddGluiSubwin(int iPosition = 0);

	// return the pointer to the GLUI sub-window
	GLUI *PCGetGluiSubwin();
// ADD-BY-LEETEN 08/11/2008-END

protected:
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

	// methods to manipulate the object 
	void _AlignPan(double pdNewCoord[3], double pdOldCoord[3]);
	void _AlignRotate(double pdAxis[3]);

	void _RotateCamera();
	void _PanCamera();
	void _ZoomCamera();

	void _PanModel();
	void _RotateModel();
	void _ZoomModel();

	// interfaces
	virtual void _DisplayFunc();
	virtual void _ReshapeFunc(int, int);
	virtual void _KeyboardFunc(unsigned char, int, int);
	virtual void _SpecialFunc(int, int, int);
	virtual void _IdleFunc();

	virtual void _InitFunc();

public:
	virtual void _DisplayCB();
	virtual void _ReshapeCB(int w, int h);
	virtual void _KeyboardCB(unsigned char key, int x, int y);
	virtual void _SpecialCB(int skey, int x, int y);
	virtual void _MotionCB(int x, int y);
	virtual void _MouseCB(int button, int state, int x, int y);
	virtual void _IdleCB();

	// return the window ID
	int IGetId();

	int ICreate(
		char *szTitle, 
		bool bUseDefault = true,
		int iX = 0, int iY = 0, int iW = 128, int iH = 128);

	CGlutWin();
	virtual ~CGlutWin();

	// ADD-BY-LEETEN 08/09/2008-BEGIN
	void _Set();

	static vector<CGlutWin*> vcWins;
	static int IGetActiveWin();
	static int IGetNrOfWins();
	static void _SetActiveWin(int iWin);
	static CGlutWin *PCGetActiveWin();
	static void _AddWin(
		CGlutWin *win, 
		char *szTitle, 
		bool bUseDefault = true, int x = 0, int y = 0, int w = 128, int h = 128);
	// ADD-BY-LEETEN 08/09/2008-END
};

// DEL-BY-LEETEN 08/09/2008-BEGIN
	// void _GWAddWin(CGlutWin *win, char *szTitle, bool bUseDefault = true, int x = 0, int y = 0, int w = 128, int h = 128);
// DEL-BY-LEETEN 08/09/2008-END

#endif	//	__GLUT_WINDOW_H__

/*

$Log: not supported by cvs2svn $
Revision 1.1  2008/08/10 05:00:27  leeten

[2008/08/10]
1. First time checkin. Now we use the static member methods to add windows.

Revision 1.1.1.1  2008/08/09 13:50:10  leeten

[2008/08/09]
1. First time checkin. LIBGW is a OO-based library to create GLUT window. In each window, there is a 3D coordinate for the object and mouse countrol to manipulate the coordinate and viewpoint.


*/
