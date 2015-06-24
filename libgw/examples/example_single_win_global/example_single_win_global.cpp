/*************************************************

example_single_win

This sample uses CTestWin, a sub class of CGlutWin, 
to create an application with a GLUT windows. The main 
purpose of thie program is to test the gobla function to 
bypass windows and GLUI events.

**************************************************/

#include "../include/TestWin.h"

CTestWin 
	cRedWin(1.0, 0.0, 0.0, 1.0); 

void 
_GlobalFunc(int iWid, unsigned int uiCbId, va_list vaArgs)
{
	if( cRedWin.IGetId() != iWid )
		return;

	cRedWin._PrintEvent(uiCbId, vaArgs);
}

int
main(int argn, char *argv[])
{
	CGlutWin::_Init(
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

	cRedWin.ICreate("RED", false, 100, 100, 256, 256);

	CGlutWin::_RegisterGlobalFunc(_GlobalFunc);

	glutMainLoop();
	return 0;
}

