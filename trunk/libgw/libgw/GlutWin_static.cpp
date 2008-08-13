#include "GlutWin.h"

namespace CGlutWin_static {

void 
_DisplayCB()
{
	CGlutWin::PCGetActiveWin()->_DisplayCB();
}

static 
void 
_ReshapeCB(int w, int h)
{
	CGlutWin::PCGetActiveWin()->_ReshapeCB(w, h);
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
	}
}

static 
void 
_SpecialCB(int skey, int x, int y)
{
	CGlutWin::PCGetActiveWin()->_SpecialCB(skey, x, y);
}

static 
void
 _IdleCB()
{
	CGlutWin::PCGetActiveWin()->_IdleCB();
}

static 
void 
_MotionCB(int x, int y)
{
	CGlutWin::PCGetActiveWin()->_MotionCB(x, y);
}

static 
void 
_MouseCB(int button, int state, int x, int y)
{
	CGlutWin::PCGetActiveWin()->_MouseCB(button, state, x, y);
}

// ADD-BY-LEETEN 08/11/2008-BEGIN
static 
void 
_TimerCB(int iIdValue)
{
	int iWid =		iIdValue	/ 0x0100;
	int iValue =	iIdValue	% 0x0100;

	int iWin = CGlutWin::IGetWin(iWid);
	if( iWin >= 0 )
	{
		glutSetWindow(iWid);
		CGlutWin::PCGetActiveWin()->_TimerCB(iValue);
	}
}
// ADD-BY-LEETEN 08/11/2008-END

// ADD-BY-LEETEN 08/13/2008-BEGIN
void
_GluiCB(int iIdValue)
{
	int iWid =		iIdValue	/ 0x0100;
	int iValue =	iIdValue	% 0x0100;

	int iWin = CGlutWin::IGetWin(iWid);
	if( iWin >= 0 )
	{
		glutSetWindow(iWid);
		CGlutWin::PCGetActiveWin()->_GluiCB((unsigned short)iValue);
	}
}
// ADD-BY-LEETEN 08/13/2008-END

} // namespace CGlutWin_static

using namespace CGlutWin_static;
/////////////////////////////////////////////////////////////////////////////
// static member method of class CGlutWin

vector<CGlutWin*> CGlutWin::vcWins;

// ADD-BY-LEETEN 08/11/2008-BEGIN

// a static member method to add a timer event to the fiven window
// it is decalred as a static method in order to call the _TimerCB in the namespace GLutWin_static
void 
CGlutWin::_AddTimer(CGlutWin *win, unsigned int msecs, short value)
{
	// because the timer events from all windows are passed throught the same callback, 
	// the windows id is added so later the callback can correctly bypass the timer event
	int iIdValue = win->iId * 0x0100 + value;

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
	// MOD-BY-LEETEN 08/11/2008-FROM:
		// return vcWins[IGetActiveWin()];
	// TO:
	// Since the GLUI windows/subwindows might change the active window, it is manually reset to the current window

	CGlutWin *pcActiveWin = vcWins[IGetActiveWin()];
	// MOD-By-TLEE 08/11/2008-FROM:
		// glutSetWindow(pcActiveWin->iId);
	// TO:
		pcActiveWin->_Set();
	// MOD-By-TLEE 08/11/2008-END
	return pcActiveWin;
	// MOD-BY-LEETEN 08/11/2008-END
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
	CGlutWin *win, 
	char *szTitle, 
	bool bUseDefault, 
	int x, int y, int w, int h)
{
	win->ICreate(szTitle, bUseDefault, x, y, w, h);

	#if	0	// MOD-BY-LEETEN 08/11/2008-FROM:


		glutDisplayFunc(	CGlutWin_static::_DisplayCB);
		glutReshapeFunc(	CGlutWin_static::_ReshapeCB);
		glutKeyboardFunc(	CGlutWin_static::_KeyboardCB);
		glutSpecialFunc(	CGlutWin_static::_SpecialCB);
		glutMotionFunc(		CGlutWin_static::_MotionCB);
		glutMouseFunc(		CGlutWin_static::_MouseCB);
		glutIdleFunc(		CGlutWin_static::_IdleCB);

	#else	// MOD-BY-LEETEN 08/11/2008-TO:

	GLUI_Master.set_glutDisplayFunc(	CGlutWin_static::_DisplayCB);
	GLUI_Master.set_glutReshapeFunc(	CGlutWin_static::_ReshapeCB);
	GLUI_Master.set_glutKeyboardFunc(	CGlutWin_static::_KeyboardCB);
	GLUI_Master.set_glutSpecialFunc(	CGlutWin_static::_SpecialCB);
	GLUI_Master.set_glutMouseFunc(		CGlutWin_static::_MouseCB);

	glutMotionFunc(		CGlutWin_static::_MotionCB);

	GLUI_Master.set_glutIdleFunc(		CGlutWin_static::_IdleCB);

	if( win->iGluiEnum & GLUI_SUBWIN )
	{
		win->pcGluiSubwin = GLUI_Master.create_glui_subwindow(win->iId, win->iSubwinPosistion);
		win->pcGluiSubwin->set_main_gfx_window( win->iId );
	}

	if( win->iGluiEnum & GLUI_WIN )
	{
		static char szGluiTitle[1024+1];
		sprintf(szGluiTitle, "%s-GLUI", szTitle);
		win->pcGluiWin = GLUI_Master.create_glui(szGluiTitle);
		win->pcGluiWin->set_main_gfx_window( win->iId );
	}

	#endif	// MOD-BY-LEETEN 08/11/2008-END

	// ADD-BY-TLEE 08/13/2008-BEGIN
				// call _InitFunc after the GLUI win/subwin have been created
	win->_InitFunc();
	// ADD-BY-TLEE 08/13/2008-END

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

// ADD-BY-LEETEN 08/13/2008-BEGIN
void 
CGlutWin::_AddButton(
	CGlutWin *win,
	char *szName,
	unsigned short usValue
	)
{
	int iWidValue = win->IGetId() * 0x0100 + usValue;
	win->PCGetGluiSubwin()->add_button(szName, iWidValue, CGlutWin_static::_GluiCB);
}

// ADD-BY-LEETEN 08/13/2008-END

/*

$Log: not supported by cvs2svn $
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
