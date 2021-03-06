/*************************************************

example_multi_wins

This sample uses CTestWin, a sub class of CGlutWin, 
to create an application with three GLUT windows.

**************************************************/

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

