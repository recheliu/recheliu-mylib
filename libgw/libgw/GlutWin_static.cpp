#include "GlutWin.h"

namespace CGlutWin_static {

void 
_DisplayCB()
{
	CGlutWin::PCGetActiveWin()->_DisplayCB();
	// ADD-BY-LEETEN 2009/01/15-BEGIN
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_DISPLAY);
	// ADD-BY-LEETEN 2009/01/15-END
}

static 
void 
_ReshapeCB(int w, int h)
{
	CGlutWin::PCGetActiveWin()->_ReshapeCB(w, h);
	// ADD-BY-LEETEN 2009/01/15-BEGIN
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_RESHAPE, w, h);
	// ADD-BY-LEETEN 2009/01/15-END
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
		// ADD-BY-LEETEN 2009/01/15-BEGIN
		CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_KEYBOARD, key, x, y);
		// ADD-BY-LEETEN 2009/01/15-END
	}
}

static 
void 
_SpecialCB(int skey, int x, int y)
{
	CGlutWin::PCGetActiveWin()->_SpecialCB(skey, x, y);
	// ADD-BY-LEETEN 2009/01/15-BEGIN
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_SPECIAL, skey, x, y);
	// ADD-BY-LEETEN 2009/01/15-END
}

static 
void
 _IdleCB()
{
	CGlutWin::_IdleCB_static();
}

// ADD-BY-LEETEN 2009/03/04-BEGIN
static 
void 
_PassiveMotionCB(int x, int y)
{
	CGlutWin::PCGetActiveWin()->_PassiveMotionCB(x, y);
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_PASSIVE_MOTION, x, y);
}
// ADD-BY-LEETEN 2009/03/04-END

static 
void 
_MotionCB(int x, int y)
{
	CGlutWin::PCGetActiveWin()->_MotionCB(x, y);
	// ADD-BY-LEETEN 2009/01/15-BEGIN
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_MOTION, x, y);
	// ADD-BY-LEETEN 2009/01/15-END
}

static 
void 
_MouseCB(int button, int state, int x, int y)
{
	CGlutWin::PCGetActiveWin()->_MouseCB(button, state, x, y);
	// ADD-BY-LEETEN 2009/01/15-BEGIN
	CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_MOUSE, button, state, x, y);
	// ADD-BY-LEETEN 2009/01/15-END
}

// ADD-BY-LEETEN 08/11/2008-BEGIN
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

		// ADD-BY-LEETEN 2009/01/15-BEGIN
		CGlutWin::_GlobalCB(CGlutWin::PCGetActiveWin()->IGetId(), CGlutWin::CB_TIMER, iValue);
		// ADD-BY-LEETEN 2009/01/15-END
	}
}
// ADD-BY-LEETEN 08/11/2008-END

} // namespace CGlutWin_static

using namespace CGlutWin_static;
/////////////////////////////////////////////////////////////////////////////
// static member method of class CGlutWin

vector<CGlutWin*> CGlutWin::vcWins;

// ADD-BY-LEETEN 08/14/2008-BEGIN
void 
CGlutWin::_IdleCB_static()
{
	for(vector<CGlutWin*>::iterator 
			vipcWin = vcWins.begin();
		vipcWin != vcWins.end();
		vipcWin++)	
		(*vipcWin)->_IdleCB();
}
// ADD-BY-LEETEN 08/14/2008-END

// ADD-BY-LEETEN 08/11/2008-BEGIN

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

// ADD-BY-LEETEN 08/11/2008-END

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

	// ADD-BY-LEETEN 08/11/2008-BEGIN
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
	// ADD-BY-LEETEN 08/11/2008-END

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

	// ADD-BY-LEETEN 2009/03/04-BEGIN
	glutPassiveMotionFunc(	CGlutWin_static::_PassiveMotionCB);
	// ADD-BY-LEETEN 2009/03/04-END

	GLUI_Master.set_glutIdleFunc(		CGlutWin_static::_IdleCB);

	vcWins.push_back(win);
}

// ADD-BY-LEETEN 08/11/2008-BEGIN
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
// ADD-BY-LEETEN 08/11/2008-END

// ADD-BY-LEETEN 08/12/2008-BEGIN
bool CGlutWin::bSwapBuffer = false;

void 
CGlutWin::_Init(
	int *argcp, 
	char **argv, 
	unsigned int mode
)
{
	glutInit(argcp, argv);
	glutInitDisplayMode(mode);

	if( mode & GLUT_DOUBLE )
		bSwapBuffer = true;
}
// ADD-BY-LEETEN 08/12/2008-END

// ADD-BY-LEETEN 08/13/2008-END

										// define a static method as the GLUI callbacks
// ADD-BY-LEETEN 2008/08/15-BEGIN
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
// ADD-BY-LEETEN 2008/08/15-END

// ADD-BY-LEETEN 2009/01/15-BEGIN
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

// ADD-BY-LEETEN 2009/01/15-END

/*

$Log: not supported by cvs2svn $
Revision 1.11  2009/02/22 22:09:45  leeten

[2009/02/22]
1. [DEBUG] Change the offset for the windows from 8 bits to 16 bits.

Revision 1.10  2009/01/16 14:31:37  leeten

[2009/01/16]
1. [ADD] Pass the window event as default events to the registered global function.

Revision 1.9  2008/08/20 19:35:24  leeten

[2008/08/20]
1. [DEL] Remove the method _AddButton() and related methods

Revision 1.8  2008/08/16 21:21:33  leeten

[2008/08/16]
1. [CHANGE] Change the type of the value for timer event from int to unsigned short.

Revision 1.7  2008/08/16 16:03:33  leeten

[2008/08/16]
1. [DEL] Remove old deleted code.
2. [CHANGE] Redefine the static method _AddWin since its usage is reduced to add a window to the window list, other than the function to create the windows.
3. [CHANGE] Use  the method IAddWin to combine the passed value and the window ID.

Revision 1.6  2008/08/15 14:40:32  leeten

[2008/08/15]
1. [ADD] Add a new paramter to _AddButton to specify the panel.
2. [CHANGE] Define a new static method _GluiCB_static as the callback to handle GLUI callbacks.Thus the _GluiCB is removed from the namesapce GlutWin_static.

Revision 1.5  2008/08/14 14:46:46  leeten

[2008/08/14]
1. [ADD] Define a new static method _IdleCB_static to bypass the idle event to all windows.

Revision 1.4  2008/08/13 21:01:32  leeten

[2008/08/13]
1. [ADD] Define a new callback _GluiCB() to handle event from GLUI control.
2. [ADD] Declare a new static method _AddButton() to add a button to a given window.
3. [ADD] Add a new static method _Init to replace the glutInit. If GLUT_DOUBLE is specifed, a new static flag bSwapBuffer will be true.

Revision 1.3  2008/08/12 16:40:54  leeten

[2008/08/12]
1. [ADD] Define a function _TimerCB in the namespace CGlutWin_static to handle the global timer event.
2. [ADD] Define a static method _AddTimer to associate a new timer to a window. In this method, the passed value and the window's id will be combined into a long int s.t later the callback _TimerCB() can bypass the event to the corresponding window.
3. [ADD] Define a new method _GetWin() to conver a GLUT window ID to the order in the vector vcWins.

Revision 1.2  2008/08/11 04:30:42  leeten

[2008/08/11]
1. [ADD] In PCGetActiveWin, manually set the window as the current window.
2. [ADD] In IGetActiveWin, when checking of window id failes, since the id might represent a GLUI window, the GLUI win and subwin if possible.
3. [CHANGE] Register the GLUT callbacks via GLUI.

Revision 1.1  2008/08/10 05:00:28  leeten

[2008/08/10]
1. First time checkin. Now we use the static member methods to add windows.


*/
