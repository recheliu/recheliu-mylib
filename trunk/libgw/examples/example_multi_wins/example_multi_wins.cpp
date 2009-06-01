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

int
main(int argn, char *argv[])
{
	CGlutWin::_Init(
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

	cRedWin.ICreate("RED", false, 100, 100, 256, 256);
	cGreenWin.ICreate("GREEN", false, 200, 100, 256, 256);
	cBlueWin.ICreate("BLUE", false, 300, 100, 256, 256);

	glutMainLoop();
	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/16 16:25:22  leeten

[2008/08/16]
1. [FIRST TIME CHECKIN].


*/
