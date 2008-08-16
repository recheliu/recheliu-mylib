#ifndef		__GLUT_WINDOW_H__
#define		__GLUT_WINDOW_H__

// ADD-BY-LEETEN 08/09/2008-BEGIN
#include <vector>
#include <stack>
using namespace std;
// ADD-BY-LEETEN 08/09/2008-END

#pragma comment (lib, "winmm.lib")      /* link with Windows MultiMedia lib */

#pragma comment (lib, "libfps.lib")      // link with my own library libfps

#if		defined(_WIN32)
	#define GLUT_BUILDING_LIB	
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

	// ADD-BY-LEETEN 08/12/2008-BEGIN
	#include <stdarg.h>	
	// ADD-BY-LEETEN 08/12/2008-END

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

// ADD-BY-LEETEN 08/12/2008-BEGIN
#include "libfps.h"
// DEL-BY-LEETEN 2008/08/14-BEGIN
	// #pragma comment (lib, "libfps.lib")      
// DEL-BY-LEETEN 2008/08/14-END
// ADD-BY-LEETEN 08/12/2008-END

class CGlutWin {

	// ADD-BY-LEETEN 08/12/2008-BEGIN
				// variables and method for displaying FPS
protected:
	FPS_COUNTER cFps;
	bool bDisplayFps;
public:
	void _DisplayFpsOn();
	void _DisplayFpsOff();
	float fGetFps();

				// variables and method for output string
public:
	char *SZSprintf(const char *szFormat, ...);
	void _DrawString(char *szString);		// draw a string on the screen
	void _AddToLog(char *szString, FILE* fpOutput = stderr);			// print out a string on the console as the log

				// variables and method for deciding if the screen should be keep updating
protected:
						
	bool bKeepUpdate;	// if bKeepUpdate is true, in the _IdleCB, a glutPostRedisplay() will be automatically called
public:
	void _KeepUpdateOn();
	void _KeepUpdateOff();

	// ADD-BY-LEETEN 08/12/2008-END

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

	// ADD-BY-LEETEN 08/13/2008-BEGIN
	void _UpdateWinCoord(int *px, int *py, bool bFlipY = true);
	// ADD-BY-LEETEN 08/13/2008-END
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

	// ADD-BY-LEETEN 08/11/2008-BEGIN
	virtual void _TimerFunc(int value);
	// ADD-BY-LEETEN 08/11/2008-END

	// ADD-BY-LEETEN 08/16/2008-BEGIN
							// because the window can be created via the method ICreate,
							// this purposes of _AddWin are only to register callbacks
							// and add the window to the list, which can be hidden from 
							// the caller. Hence it is declared as protected.
	static void _AddWin(
		CGlutWin *win); 
	// ADD-BY-LEETEN 08/16/2008-END

public:
	virtual void _DisplayCB();
	virtual void _ReshapeCB(int w, int h);
	virtual void _KeyboardCB(unsigned char key, int x, int y);
	virtual void _SpecialCB(int skey, int x, int y);
	virtual void _MotionCB(int x, int y);
	virtual void _MouseCB(int button, int state, int x, int y);
	virtual void _IdleCB();

	// ADD-BY-LEETEN 08/11/2008-BEGIN
	virtual void _TimerCB(int value);
	// ADD-BY-LEETEN 08/11/2008-END

	// ADD-BY-LEETEN 08/11/2008-BEGIN
	void _AddTimer(unsigned int msecs, short value = 0);	// add a timer event
	void _Redisplay();										// trigger a display event
	// ADD-BY-LEETEN 08/11/2008-END

	int IGetId();			// return the window ID	

	int ICreate(			// create the window.
		char *szTitle, 
		bool bUseDefault = true,
		int iX = 0, int iY = 0, int iW = 128, int iH = 128);

	CGlutWin();
	virtual ~CGlutWin();

	// ADD-BY-LEETEN 08/12/2008-BEGIN
	void _Push();			// push current window
	void _Pop();			// pop this window

							// when implement new method that involve OpenGL status, 
							// it is strongly recommended to implement in such a way:
							/*
							CGlutWin::_GlMethod()
							{
								_Begin();

								...

								_End();
							}
							*/

							// variables to maintain the active window
	stack<int> cWid_stack;	// a stack to hold the previous window
	void _PushWid();		// push current window ID
	void _PopWid();			// pop current window ID
	void _Begin();			// push current window and set
	void _End();			// pop the window
	// ADD-BY-LEETEN 08/12/2008-BEGIN

	// ADD-BY-LEETEN 08/09/2008-BEGIN
	void _Set();			// force the window to be the active window

	// ADD-BY-LEETEN 08/15/2008-BEGIN
	void _Reshape(int w, int h)
	{
		_Set();
		glutReshapeWindow(w, h);
	}
	// ADD-BY-LEETEN 08/15/2008-END

	//////////////////////////////////////////////////////////////////////////////
										// static variables/methods
	static vector<CGlutWin*> vcWins;
	static int IGetActiveWin();
	static int IGetNrOfWins();
	static void _SetActiveWin(int iWin);
	static CGlutWin *PCGetActiveWin();

	// DEL-BY-TLEE 2008/08/16-BEGIN
		static void _AddWin(
			CGlutWin *win, 
			char *szTitle, 
			bool bUseDefault = true, int x = 0, int y = 0, int w = 128, int h = 128);
	// DEL-BY-TLEE 2008/08/16-END

	// ADD-BY-LEETEN 08/09/2008-END

	// ADD-BY-LEETEN 08/11/2008-BEGIN
										// given a GLUT window id, return its id in the vcWins;
	static int IGetWin(int iWid);

										// add a timer event in the given time period in msecs to the specified window with the given value
	static void _AddTimer(CGlutWin *win, unsigned int msecs, short value);
	// ADD-BY-LEETEN 08/11/2008-END

	// ADD-BY-LEETEN 08/12/2008-BEGIN
	static bool bSwapBuffer;
	static void _Init(int *argcp, char **argv, unsigned int mode);
	// ADD-BY-LEETEN 08/12/2008-END

	// ADD-BY-LEETEN 08/13/2008-BEGIN
	virtual void _GluiFunc(unsigned short usValue);
	virtual void _GluiCB(unsigned short usValue);

	// ADD-BY-LEETEN 2008/08/15-BEGIN
											// combine the window ID with the given value
											// the purpose of this method is to create an unique ID 
											// for GLUI controls
	int IAddWid(unsigned short usValue)
	{
		return IGetId() * 0x0100 + usValue;
	}
	// ADD-BY-LEETEN 2008/08/15-END

	#if	0		// MOD-BY-LEETEN 2008/08/15-FROM:
		void _AddButton(char *szName, unsigned short usValue);
		static void _AddButton(CGlutWin *win, char *szName, unsigned short usValue);
	#else		// MOD-BY-LEETEN 2008/08/15-TO:
											// add one parameter to decide the panel

	void _AddButton(char *szName, unsigned short usValue, GLUI_Panel *pcPanel = NULL);
	static void _AddButton(CGlutWin *win, char *szName, unsigned short usValue, GLUI_Panel *pcPanel = NULL);

	#endif		// MOD-BY-LEETEN 2008/08/15-END
	// ADD-BY-LEETEN 08/13/2008-END

	// ADD-BY-LEETEN 08/14/2008-BEGIN
	static void _IdleCB_static();
	// ADD-BY-LEETEN 08/14/2008-END

	// ADD-BY-LEETEN 2008/08/15-BEGIN
											// declare a static method as the GLUI callbacks
	static void _GluiCB_static(int iIdValue);
	// ADD-BY-LEETEN 2008/08/15-END
};

#endif	//	__GLUT_WINDOW_H__

/*

$Log: not supported by cvs2svn $
Revision 1.6  2008/08/15 14:39:13  leeten

[2008/08/15]
1. [DEL] Remove old deleted code.
2. [DEL] Remove the pragmas  to link libraries in mylib.
3. [ADD] Add a new method IAddWid to combien the window's ID with a unsigned short value.
4. [ADD] Add a new paramter to _AddButton to specify the panel.
5. [ADD] Declared a new static method _GluiCB_static as the callback to handle GLUI callbacks.

Revision 1.5  2008/08/14 14:46:31  leeten

[2008/08/18]
1. [ADD] Declare a new static method _IdleCB_static to bypass the idle event to all windows.

Revision 1.4  2008/08/13 21:00:24  leeten

[2008/08/13]
1. [ADD] Declare a new method SZSprint to format string. Hence stdarg.h is included
2. [ADD] Declare a new method _DrawString() to draw a string in the left bottom corner.
3. [ADD] Declare a new method _AddToLog to add a string to specifed file (by default STDERR). When printing this string, a prefix will be added to indicate which window display this string, and a new line will be automatically appended at the end.
4. [ADD] Declare methods _DisplayFpsOn()/_DisplayFpsOff(); Hence libfps.h is included
5. [ADD] Declare new methods _KeepUpdateOn()/_KeepUpdateOff()  to decide if the screen should be keeping up to date
6. [ADD] Declare a new callback _GluiCB() to handle event from GLUI control. It will call another method _GluiFunc().
7. [ADD] Declare a new method _AddButton() to add a button to the GLUI windows/subwindows.
8. [ADD] Declare a new static method _AddButton() to add a button to a given window.
9. [ADD] Add a new static method _Init to replace the glutInit. If GLUT_DOUBLE is specifed, a new static flag bSwapBuffer will be true.

Revision 1.3  2008/08/12 16:36:59  leeten

[2008/08/12]
1. [DEL] Remvoe the checking of FREEGLUT since it will be done in GLUI.
2. [ADD] Support the timer event. To trigger a timer event, the application can call a static method _AddTimer(). to process the timer event, a new callback _TimerCB() is defined. This callback will invoke another method _TimerFunc().
3. [ADD] Define methods to avoid the misorder of active windows since the timer event can shuffle the order among the GLUT windows.
4. [ADD] Define new methods to similar GLUT APIs: _Push(), _Pop() and _Redisplay().
5. [ADD] Define a new method _GetWin() to conver a GLUT window ID to the order in the vector vcWins.

Revision 1.2  2008/08/11 04:27:26  leeten

[2008/08/11]
1. [ADD] Combine with GLUI.
2. [ADD] Declare new routine to enable GLUI subwin/win and return the GLUI pointers.
3. [ADD] Add enums as GLUI options.

Revision 1.1  2008/08/10 05:00:27  leeten

[2008/08/10]
1. First time checkin. Now we use the static member methods to add windows.

Revision 1.1.1.1  2008/08/09 13:50:10  leeten

[2008/08/09]
1. First time checkin. LIBGW is a OO-based library to create GLUT window. In each window, there is a 3D coordinate for the object and mouse countrol to manipulate the coordinate and viewpoint.


*/
