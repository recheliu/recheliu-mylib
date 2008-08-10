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
	return vcWins[IGetActiveWin()];
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

	glutDisplayFunc(	CGlutWin_static::_DisplayCB);
	glutReshapeFunc(	CGlutWin_static::_ReshapeCB);
	glutKeyboardFunc(	CGlutWin_static::_KeyboardCB);
	glutSpecialFunc(	CGlutWin_static::_SpecialCB);
	glutMotionFunc(		CGlutWin_static::_MotionCB);
	glutMouseFunc(		CGlutWin_static::_MouseCB);
	glutIdleFunc(		CGlutWin_static::_IdleCB);

	vcWins.push_back(win);
}

/*

$Log: not supported by cvs2svn $

*/
