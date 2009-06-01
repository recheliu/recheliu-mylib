/*************************************************

example_multi_wins

This sample uses CTestWin, a sub class of CGlutWin, 
to create an application with three GLUT windows.

**************************************************/

#pragma comment (lib, "libgw.lib")      // link with the library libgw

#include "../include/TestWin.h"

CTestWin 
	cGreenWin(0.0, 1.0, 0.0, 1.0), 
	cRedWin(1.0, 0.0, 0.0, 1.0),
	cBlueWin(0.0, 0.0, 1.0, 1.0);

void _GlobalFunc(int iWid, unsigned int uiCbId, va_list vaArgs)
{
	CTestWin *pcWin;
	if( cRedWin.IGetId() == iWid )
		pcWin = &cRedWin;
	else
	if( cGreenWin.IGetId() == iWid )
		pcWin = &cGreenWin;
	else
	if( cBlueWin.IGetId() == iWid )
		pcWin = &cBlueWin;
	else
		return;

	pcWin->_PrintEvent(uiCbId, vaArgs);
}

int
main(int argn, char *argv[])
{
	CGlutWin::_Init(
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

	cRedWin.ICreate("RED", false, 100, 100, 256, 256);
	cGreenWin.ICreate("GREEN", false, 200, 100, 256, 256);
	cBlueWin.ICreate("BLUE", false, 300, 100, 256, 256);

	CGlutWin::_RegisterGlobalFunc(_GlobalFunc);

	glutMainLoop();
	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2009/01/16 14:37:00  leeten

[2009/01/16]
1. [FIRST TIME CHECKIN] This is an example to test how the global function can work in multiple window environment.


*/
