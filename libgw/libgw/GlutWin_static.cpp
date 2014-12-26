#include "GlutWin.h"

namespace CGlutWin_static {

void 
_DisplayCB()
{
	CGlutWin::PCGetActiveWin()->_DisplayCB();
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_DISPLAY);
}

static 
void 
_ReshapeCB(int w, int h)
{
	CGlutWin::PCGetActiveWin()->_ReshapeCB(w, h);
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_RESHAPE, w, h);
}

static 
void 
_KeyboardCB(unsigned char key, int x, int y)
{
	switch(key) 
	{
	case 27:	// ESC
		// terminate this application
		exit(0);
		break;

	default:
		CGlutWin::PCGetActiveWin()->_KeyboardCB(key, x, y);
		CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_KEYBOARD, key, x, y);
	}
}

static 
void 
_SpecialCB(int skey, int x, int y)
{
	CGlutWin::PCGetActiveWin()->_SpecialCB(skey, x, y);
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_SPECIAL, skey, x, y);
}

static 
void
 _IdleCB()
{
	CGlutWin::_IdleCB_static();
}

static 
void 
_PassiveMotionCB(int x, int y)
{
	CGlutWin::PCGetActiveWin()->_PassiveMotionCB(x, y);
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_PASSIVE_MOTION, x, y);
}

static 
void 
_MotionCB(int x, int y)
{
	CGlutWin::PCGetActiveWin()->_MotionCB(x, y);
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_MOTION, x, y);
}

static 
void 
_MouseCB(int button, int state, int x, int y)
{
	CGlutWin::PCGetActiveWin()->_MouseCB(button, state, x, y);
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_MOUSE, button, state, x, y);
}

static 
void 
_TimerCB(int iIdValue)
{
	int iWid =		iIdValue	/ 0x010000;
	int iValue =	iIdValue	% 0x010000;

	int iWin = CGlutWin::IGetWin(iWid);
	if( iWin >= 0 )
	{
		glutSetWindow(iWid);
		CGlutWin::PCGetActiveWin()->_TimerCB((unsigned short)iValue);

		CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_TIMER, iValue);
	}
}

} // namespace CGlutWin_static

using namespace CGlutWin_static;
/////////////////////////////////////////////////////////////////////////////
// static member method of class CGlutWin

vector<CGlutWin*> CGlutWin::vcWins;

void 
CGlutWin::_IdleCB_static()
{
	for(vector<CGlutWin*>::iterator 
			vipcWin = vcWins.begin();
		vipcWin != vcWins.end();
		vipcWin++)	
		(*vipcWin)->_IdleCB();
}

// a static member method to add a timer event to the fiven window
// it is decalred as a static method in order to call the _TimerCB in the namespace GLutWin_static
void 
CGlutWin::_AddTimer(CGlutWin *win, unsigned int msecs, unsigned short value)
{
					// because the timer events from all windows are passed throught the same callback, 
					// the windows id is added so later the callback can correctly bypass the timer event
	int iIdValue = win->IAddWid(value);

					// register a timer event
	GLUI_Master.set_glutTimerFunc(msecs, CGlutWin_static::_TimerCB, iIdValue);
}


int
CGlutWin::IGetNrOfWins()
{
	return (int)vcWins.size();
}

void 
CGlutWin::_SetActiveWin(int iWin)
{
	assert( iWin < (int)vcWins.size() );
	glutSetWindow(vcWins[iWin]->iId);
	glutShowWindow();
}

CGlutWin *
CGlutWin::PCGetActiveWin()
{
	// Since the GLUI windows/subwindows might change the active window, it is manually reset to the current window

	CGlutWin *pcActiveWin = vcWins[IGetActiveWin()];
		pcActiveWin->_Set();
	return pcActiveWin;
}

int 
CGlutWin::IGetActiveWin()
{
	int iId = glutGetWindow();
	
	int iWin = 0;
	for(vector<CGlutWin*>::iterator 
			ivcWin = vcWins.begin();
		ivcWin != vcWins.end() && (*ivcWin)->IGetId() != iId;
		ivcWin++, iWin++)
		;

	// since GLUI can create more windows, if the previous search fail, search the parent window if possible
	if( iWin >= (int)vcWins.size() )
	{
		iWin = 0;
		for(vector<CGlutWin*>::iterator 
				ivcWin = vcWins.begin();
				ivcWin != vcWins.end();
			ivcWin++, iWin++)
		{
			 if( (*ivcWin)->pcGluiSubwin && iId == (*ivcWin)->pcGluiSubwin->get_glut_window_id() )
				 break;

			 if( (*ivcWin)->pcGluiWin && iId == (*ivcWin)->pcGluiWin->get_glut_window_id() )
				 break;
		}
	}

	assert( iWin < (int)vcWins.size() );

	return iWin;
}

void 
CGlutWin::_AddWin(
	CGlutWin *win)
{
	GLUI_Master.set_glutDisplayFunc(	CGlutWin_static::_DisplayCB);
	GLUI_Master.set_glutReshapeFunc(	CGlutWin_static::_ReshapeCB);
	GLUI_Master.set_glutKeyboardFunc(	CGlutWin_static::_KeyboardCB);
	GLUI_Master.set_glutSpecialFunc(	CGlutWin_static::_SpecialCB);
	GLUI_Master.set_glutMouseFunc(		CGlutWin_static::_MouseCB);

	glutMotionFunc(		CGlutWin_static::_MotionCB);

	glutPassiveMotionFunc(	CGlutWin_static::_PassiveMotionCB);

	GLUI_Master.set_glutIdleFunc(		CGlutWin_static::_IdleCB);

	vcWins.push_back(win);
}

// given a GLUT window id, return its id in the vcWins;
int 
CGlutWin::IGetWin(int iWid)
{
	int iWin = 0;
	for(vector<CGlutWin*>::iterator 
			ivcWin = vcWins.begin();
		ivcWin != vcWins.end() && (*ivcWin)->IGetId() != iWid;
		ivcWin++, iWin++)
		;
	if( iWin == vcWins.size() )
		iWin = -1;

	return iWin;

}

bool CGlutWin::bSwapBuffer = false;

#if	defined(USE_FREEGLUT)
void CGlutWin::WarningFunc( const char *fmt, va_list ap )
{
	// Ignore warnings.
}
#endif	// #if	defined(USE_FREEGLUT)

void 
CGlutWin::_Init(
	int *argcp, 
	char **argv, 
	unsigned int mode
)
{
	glutInit(argcp, argv);
	#if	defined(USE_FREEGLUT)
	glutInitWarningFunc( CGlutWin::WarningFunc );
	#endif	// #if	defined(USE_FREEGLUT)
	glutInitDisplayMode(mode);

	if( mode & GLUT_DOUBLE )
		bSwapBuffer = true;
}

										// define a static method as the GLUI callbacks
void 
CGlutWin::_GluiCB_static(int iIdValue)
{
	int iWid =		iIdValue	/ 0x010000;
	int iValue =	iIdValue	% 0x010000;

	int iWin = CGlutWin::IGetWin(iWid);
	if( iWin >= 0 )
	{
		glutSetWindow(iWid);
		CGlutWin::PCGetActiveWin()->_GluiCB((unsigned short)iValue);
	}
}

void 
(*CGlutWin::_GlobalFunc)(int iWid, unsigned int uiCbId, va_list vaArgs) = NULL;

void 
CGlutWin::_GlobalCB
(
	int iWid, 
	unsigned int uiCbId, 
	...
)
{
	if( NULL == _GlobalFunc )
		return;

	va_list args;
	va_start(args, uiCbId);
	_GlobalFunc(iWid, uiCbId, args);
	va_end (args);
}

void 
CGlutWin::_RegisterGlobalFunc(
	void (*_MyGlobalFunc)(int iWid, unsigned int uiCbId, va_list vaArgs))
{
	_GlobalFunc = _MyGlobalFunc;
}

