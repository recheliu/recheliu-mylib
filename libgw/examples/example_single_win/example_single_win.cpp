/*************************************************

example_single_win

This sample uses CTestWin, a sub class of CGlutWin, 
to create an application with a GLUT windows

**************************************************/

#include "../include/TestWin.h"

CTestWin 
	cRedWin(1.0, 0.0, 0.0, 1.0); 

int
main(int argn, char *argv[])
{
	CGlutWin::_Init(
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

	cRedWin.ICreate("RED", false, 100, 100, 256, 256);

	glutMainLoop();
	return 0;
}

