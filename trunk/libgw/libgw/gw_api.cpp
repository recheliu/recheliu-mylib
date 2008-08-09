/*

GW_API.cpp: this file define the API used to launch the GLUT windows.

*/
#include <vector>

using namespace std;
#include "gw.h"

vector<CGlutWin*> vcWins;

static 
int 
IGetActiveWinId()
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

static 
void 
_DisplayCB()
{
	vcWins[IGetActiveWinId()]->_DisplayCB();
}

static 
void 
_ReshapeCB(int w, int h)
{
	vcWins[IGetActiveWinId()]->_ReshapeCB(w, h);
}

static 
void 
_KeyboardCB(unsigned char key, int x, int y)
{
	switch(key) 
	{
	case 27:
		exit(0);
		break;

	default:
		vcWins[IGetActiveWinId()]->_KeyboardCB(key, x, y);
	}
}

static 
void 
_SpecialCB(int skey, int x, int y)
{
	vcWins[IGetActiveWinId()]->_SpecialCB(skey, x, y);
}

static 
void
_IdleCB()
{
	vcWins[IGetActiveWinId()]->_IdleCB();
}

static 
void 
_MotionCB(int x, int y)
{
	vcWins[IGetActiveWinId()]->_MotionCB(x, y);
}

static 
void 
_MouseCB(int button, int state, int x, int y)
{
	vcWins[IGetActiveWinId()]->_MouseCB(button, state, x, y);
}

/////////////////////////////////////////////////////////////////
void
_GWAddWin(CGlutWin *win, char *szTitle, bool bUseDefault, int x, int y, int w, int h)
{
	win->ICreate(szTitle, bUseDefault, x, y, w, h);

	glutDisplayFunc(	_DisplayCB);
	glutReshapeFunc(	_ReshapeCB);
	glutKeyboardFunc(	_KeyboardCB);
	glutSpecialFunc(	_SpecialCB);
	glutMotionFunc(		_MotionCB);
	glutMouseFunc(		_MouseCB);
	glutIdleFunc(		_IdleCB);

	vcWins.push_back(win);
}


/*

$Log: not supported by cvs2svn $

*/
