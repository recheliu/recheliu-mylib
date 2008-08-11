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

} // namespace CGlutWin_static

using namespace CGlutWin_static;
/////////////////////////////////////////////////////////////////////////////
// static member method of class CGlutWin

vector<CGlutWin*> CGlutWin::vcWins;

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
	glutSetWindow(pcActiveWin->iId);
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

	vcWins.push_back(win);
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2008/08/10 05:00:28  leeten

[2008/08/10]
1. First time checkin. Now we use the static member methods to add windows.


*/
