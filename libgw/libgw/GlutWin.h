#ifndef		__GLUT_WINDOW_H__
#define		__GLUT_WINDOW_H__

#if				WITH_OPENCV	
#include "GlutWin_config.h"	
#endif	// #if	WITH_OPENCV	

#if 1    // TEST-ADD
#include <algorithm>
#endif
#include <vector>
#include <stack>
using namespace std;

#if				WITH_OPENCV	
	#include "cxcore.h"
	#include "highgui.h"

	#ifndef OPENCV_VER
	#pragma message( "Warning: Remember to specify the preprocessor OPENCV before linking LIBGW!"  )
	#else
		#if	OPENCV_VER >= 200

	#if WITH_STATIC_OPENCV	
		#if defined(_DEBUG)
			#pragma comment (lib, "libjasperd.lib")
			#pragma comment (lib, "libjpegd.lib")
			#pragma comment (lib, "libpngd.lib")
			#pragma comment (lib, "libtiffd.lib")
			#pragma comment (lib, "zlibd.lib")
		#else	// #if defined(_DEBUG)
			#pragma comment (lib, "libjasper.lib")
			#pragma comment (lib, "libjpeg.lib")
			#pragma comment (lib, "libpng.lib")
			#pragma comment (lib, "libtiff.lib")
			#pragma comment (lib, "zlib.lib")
		#endif	// #if defined(_DEBUG)
	#endif	// #if WITH_STATIC_LINK

			#pragma message( OPENCV_MESSAGE  )
			
			#if defined( WITH_STATIC_OPENCV	) && defined( _DEBUG )
				#pragma comment (lib, OPENCV_CORE_DEBUG_LIB )      
				#pragma comment (lib, OPENCV_HIGHGUI_DEBUG_LIB )   
			#else	// #if defined( WITH_STATIC_LINK) && !defined( _DEBUG )

			#pragma comment (lib, OPENCV_CORE_LIB )      
			#pragma comment (lib, OPENCV_HIGHGUI_LIB )   
			#endif	// #if defined( WITH_STATIC_LINK) && !defined( _DEBUG )	
		#elif OPENCV_VER >= 100
			#pragma message( OPENCV_MESSAGE  )
			#pragma comment (lib, "cv.lib")      
			#pragma comment (lib, "cxcore.lib")  
			#pragma comment (lib, "highgui.lib") 
			#pragma comment (lib, "cvaux.lib")   
		#else
			#pragma error( "Unsupported OpenCV version" )
		#endif
	#endif
#else	// #if	WITH_OPENCV	
	#include "lodepng.h"
	#if defined(_DEBUG)
	#pragma comment (lib, "lodepngd.lib")
	#else	// #if defined(_DEBUG)
	#pragma comment (lib, "lodepng.lib")
	#endif	// #if defined(_DEBUG)
#endif	// #if	WITH_OPENCV	

	#pragma comment (lib, "winmm.lib")      /* link with Windows MultiMedia lib */
	#ifdef _DEBUG
		#pragma comment (lib, "libfps_d.lib")      // link with my own library libfps
	#else
		#pragma comment (lib, "libfps_r.lib")      // link with my own library libfps
	#endif


	#pragma comment (lib, "opengl32.lib")      /* link with Windows MultiMedia lib */
	#pragma comment (lib, "glu32.lib")      /* link with Windows MultiMedia lib */
	#ifdef USE_FREEGLUT
		#define GLUI_FREEGLUT

	#if	defined(WITH_STATIC_LINK)
		#pragma comment (lib, "freeglut_static.lib")
	#else	// #if	defined(WITH_STATIC_LINK)	
		#pragma comment (lib, "freeglut.lib")      
	#endif	// #if	defined(WITH_STATIC_LINKP)	
	#else
		#pragma comment (lib, "glut32.lib")      /* link with Windows MultiMedia lib */
	#endif

	#if	!defined(WITH_STATIC_LINK)		
	#define GLUIDLL 
	#endif	// #if	!defined(WITH_STATIC_LINK)	

	// combine w/ GLUI
	#ifdef USE_FREEGLUT
		#pragma message( "Use GLUI w/ FREEGLUT" )

		#if	defined(WITH_STATIC_LINK)
			#if	defined(_DEBUG)
				#pragma comment (lib, "glui32d_freeglut.lib")
			#else	// #if	defined(_DEBUG)
				#pragma comment (lib, "glui32_freeglut.lib")
			#endif	// #if	defined(_DEBUG)
		#else	// #if	defined(WITH_STATIC_LINK)	
		#pragma comment (lib, "glui32dll_freeglut.lib")

		#endif	// #if	defined(WITH_STATIC_LINK)	
	#else
		#pragma message( "Use GLUI w/o FREEGLUT" )
		#pragma comment (lib, "glui32dll.lib")
	#endif
	#define GLUI_NO_LIB_PRAGMA

	#include <GL/glui.h>

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <memory.h>
	#define _USE_MATH_DEFINES
	#include <math.h>

	#include <stdarg.h>	

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

#include "libopengl.h"

#include "libfps.h"

class CGlutWin {

	// a function pointer to the function that handle all windows events (e.g. window display and resize/ and GLUI events)
	// the meaning of the paramters: 
	//	iGlutWid:	the GLUT window id
	//	uiCbId:		the event or callback ID
protected:
	static void (*_GlobalFunc)(int iWid, unsigned int uiCbId, va_list vaArgs);
public:
	static void _RegisterGlobalFunc(void (*_MyGlobalFunc)(int iWid, unsigned int uiCbId, va_list vaArgs));
public:
	static void _GlobalCB(int iWid, unsigned int uiCbId, ...);
public:
	enum {
		CB_DISPLAY,
		CB_RESHAPE,
		CB_KEYBOARD,
		CB_SPECIAL,
		CB_MOTION,
		CB_PASSIVE_MOTION,
		CB_MOUSE,
		CB_TIMER,
		CB_GLUI,	// used to indicate GLUI control event
		CB_MANUAL,	// user-defined events
	};

protected:

	int iSnapshotIndex;

	struct CViewFrustrum
	{
		float fAngle_degree;
		float fNear;
		float fFar;

		CViewFrustrum()
		{
			fAngle_degree = 30.0f;
			fNear	= 0.05f;
			fFar	= 1000.0f;
		}
	} cViewFrustrum;

	struct CMouseEvent {
		int iButton;
		int iState;
		int iX;
		int iY;
	};
	vector<CMouseEvent> vcMouseEvents;
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
	void _DrawString(char *szString, int iX = 0, int iY = 0, bool bAlignToRight = false);		// draw a string on the screen
	void _AddToLog(char *szString, FILE* fpOutput = stderr);			// print out a string on the console as the log

				// draw a string in the origin of current 3D coordinate
	void _DrawString3D(char *szString, float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);		

				// variables and method for deciding if the screen should be keep updating
protected:
						
	bool bKeepUpdate;	// if bKeepUpdate is true, in the _IdleCB, a glutPostRedisplay() will be automatically called
public:
	void _KeepUpdateOn();
	void _KeepUpdateOff();

	void _DisableVerticalSync();


protected:
						// store the GLUI option to add a GLUI win and/or a GLUI subwin
	int iGluiEnum;

						// pointer to the created GLUI window
	GLUI *pcGluiWin;

						// pointer to the created GLUI sub-window
	GLUI *pcGluiSubwin;

						// location of the sub-window
	int iSubwinPosistion;

	void _UpdateWinCoord(int *px, int *py, bool bFlipY = true);
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

protected:
	typedef GLdouble TMatrix[16];

						// the window id
	int iId;

	char szMatrixFilename[1024+1];

						// title
	char szTitle[1024+1];
	char szReshape[1024+1];
	char szDisplay[1024+1];
	char szInit[1024+1];

						// the viewport
	int piViewport[4];

	int iGlutWindowWidth;
	int iGlutWindowHeight;

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

	int iPassiveCursorX;
	int iPassiveCursorY;

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

	virtual void _MotionFunc(int x, int y);
	virtual void _MouseFunc(int button, int state, int x, int y);

	virtual void _PassiveMotionFunc(int x, int y);

	virtual void _InitFunc();

	virtual void _TimerFunc(unsigned short value);

							// because the window can be created via the method ICreate,
							// this purposes of _AddWin are only to register callbacks
							// and add the window to the list, which can be hidden from 
							// the caller. Hence it is declared as protected.
	static void _AddWin(
		CGlutWin *win); 

	#if				WITH_OPENCV	
	IplImage *pcSnapshot;
	#endif	//	#if	WITH_OPENCV	

public:

							// save the current snapshot to an file
	void _SaveSnapshot(char *szSnapshotFilename = NULL);

public:

	void _SaveMatrix(char *szMatrixFilename);
	void _OpenMatrix(char *szMatrixFilename);
	void _LoadSavedMatrix(char *szMatrixFilename = NULL);

	virtual void _DisplayCB();
	virtual void _ReshapeCB(int w, int h);
	virtual void _KeyboardCB(unsigned char key, int x, int y);
	virtual void _SpecialCB(int skey, int x, int y);
	virtual void _MotionCB(int x, int y);
	virtual void _MouseCB(int button, int state, int x, int y);
	virtual void _IdleCB();

	virtual void _PassiveMotionCB(int x, int y);

	virtual void _TimerCB(unsigned short value);

	void _AddTimer(unsigned int msecs, unsigned short value = 0);	// add a timer event
	void _Redisplay();										// trigger a display event

	int IGetId();			// return the window ID	

	int ICreate(			// create the window.
		char *szTitle, 
		bool bUseDefault = true,
		int iX = 0, int iY = 0, int iW = 128, int iH = 128);

	CGlutWin();
	virtual ~CGlutWin();

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

	void _Set();			// force the window to be the active window

	void _Reshape(int w, int h)
	{
		_Set();
		glutReshapeWindow(w, h);
	}

	//////////////////////////////////////////////////////////////////////////////
										// static variables/methods
	static vector<CGlutWin*> vcWins;
	static int IGetActiveWin();
	static int IGetNrOfWins();
	static void _SetActiveWin(int iWin);
	static CGlutWin *PCGetActiveWin();

										// given a GLUT window id, return its order in the vcWins;
	static int IGetWin(int iWid);

										// add a timer event in the given time period in msecs to the specified window with the given value
	static void _AddTimer(CGlutWin *win, unsigned int msecs, unsigned short value);

	static bool bSwapBuffer;

	static void _Init(int *argcp, char **argv, unsigned int mode);

	virtual void _GluiFunc(unsigned short usValue);
	virtual void _GluiCB(unsigned short usValue);

											// combine the window ID with the given value
											// the purpose of this method is to create an unique ID 
											// for GLUI controls
	int IAddWid(unsigned short usValue)
	{
		return IGetId() * 0x010000 + (int)usValue;
	}

	#if	defined(USE_FREEGLUT)
	static void WarningFunc( const char *fmt, va_list ap );
	#endif	// #if	defined(USE_FREEGLUT)

	static void _IdleCB_static();

											// declare a static method as the GLUI callbacks
	static void _GluiCB_static(int iIdValue);
};

#endif	//	__GLUT_WINDOW_H__

