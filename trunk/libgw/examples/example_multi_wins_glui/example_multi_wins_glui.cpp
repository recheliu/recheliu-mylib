/*************************************************

example_multi_wins_glui

This sample uses CTestWin, a sub class of CGlutWin, 
to create an application with four GLUT windows, RED, 
GREEN, BLUE and WHITE. The main purpose of this sample 
is to illustrate the usage of GLUI. The RED window has 
neither GLUI window nor GLUI subwindow. The GREEN window 
has a GLUI window with a spinner. The BLUE window has a 
GLUI subwindow with a spinner. The WHITE window has both a 
GLUI window containing one spinner and a GLUI subwindow 
containing two spinners.

**************************************************/

#include "..\include\TestWin.h"

CTestWin 
	cGreenWin(0.0, 1.0, 0.0, 1.0), 
	cRedWin(1.0, 0.0, 0.0, 1.0),
	cBlueWin(0.0, 0.0, 1.0, 1.0),
	cWhiteWin(1.0, 1.0, 1.0, 1.0);

int
main(int argn, char *argv[])
{
	CGlutWin::_Init(
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

	cRedWin.ICreate("RED", false, 100, 100, 256, 256);
		cRedWin._DisplayFpsOn();
		cRedWin._KeepUpdateOn();

	cGreenWin._AddGluiWin();
	cGreenWin.ICreate("GREEN", false, 200, 100, 256, 256);
	cGreenWin.PCGetGluiWin()->add_spinner("G:", GLUI_SPINNER_FLOAT, &cGreenWin.g);

	cBlueWin._AddGluiSubwin(GLUI_SUBWINDOW_RIGHT);
	cBlueWin.ICreate("BLUE", false, 300, 100, 256, 256);
		cBlueWin.PCGetGluiSubwin()->add_spinner("B:", GLUI_SPINNER_FLOAT, &cBlueWin.b);

	cWhiteWin._AddGluiWin();
	cWhiteWin._AddGluiSubwin(GLUI_SUBWINDOW_LEFT);
	cWhiteWin.ICreate("WHITE", false, 400, 100, 256, 256);
		cWhiteWin.PCGetGluiSubwin()->add_spinner("R:", GLUI_SPINNER_FLOAT, &cWhiteWin.r);
		cWhiteWin.PCGetGluiSubwin()->add_spinner("G:", GLUI_SPINNER_FLOAT, &cWhiteWin.g);
		cWhiteWin.PCGetGluiWin()->add_spinner("B:", GLUI_SPINNER_FLOAT, &cWhiteWin.b);

	glutMainLoop();
	return 0;
}

/*

$Log: not supported by cvs2svn $

*/
