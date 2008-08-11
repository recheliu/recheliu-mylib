// MOD-BY-LEETEN 08/09/2008-FROM:
	// #include "gw.h"
// TO:
#include "GlutWin.h"
// MOD-BY-LEETEN 08/09/2008-END

class CTestWin:public CGlutWin {
	// the colors

// ADD-BY-LEETEN 08/11/2008-BEGIN
// declare them as public methods and variables
public:
// ADD-BY-LEETEN 08/11/2008-END

	float r, g, b, a;

	void _DisplayFunc()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glColor4f(r, g, b, a);
		glutWireCube(1.0f);

		glutSwapBuffers();
	}

public:
	CTestWin()	{};
	CTestWin(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

CTestWin 
	cGreenWin(0.0, 1.0, 0.0, 1.0), 
	cRedWin(1.0, 0.0, 0.0, 1.0),
	cBlueWin(0.0, 0.0, 1.0, 1.0);

// ADD-BY-LEETEN 08/11/2008-BEGIN
CTestWin cWhiteWin(1.0, 1.0, 1.0, 1.0);
// ADD-BY-LEETEN 08/11/2008-END

int
main(int argn, char *argv[])
{
	glutInit(&argn, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL );

	#if 0	// MOD-BY-LEETEN 08/09/2008-FROM:
		_GWAddWin(&cRedWin,		"RED");
		_GWAddWin(&cGreenWin,	"GREEN");
		_GWAddWin(&cBlueWin,	"BLUE");
	#else	// MOD-BY-LEETEN 08/09/2008-TO:

	#if	0	// MOD-BY-LEETEN 08/11/2008-FROM:
		CGlutWin::_AddWin(&cRedWin,		"RED");
		CGlutWin::_AddWin(&cGreenWin,	"GREEN");
		CGlutWin::_AddWin(&cBlueWin,	"BLUE");
	#else	// MOD-BY-LEETEN 08/11/2008-TO:

	CGlutWin::_AddWin(&cRedWin,		"RED", false, 100, 100, 256, 256);

	cGreenWin._AddGluiWin();
	CGlutWin::_AddWin(&cGreenWin,	"GREEN", false, 200, 100, 256, 256);
	cGreenWin.PCGetGluiWin()->add_spinner("G:", GLUI_SPINNER_FLOAT, &cGreenWin.g);

	cBlueWin._AddGluiSubwin(GLUI_SUBWINDOW_RIGHT);
	CGlutWin::_AddWin(&cBlueWin,	"BLUE", false, 300, 100, 256, 256);
		cBlueWin.PCGetGluiSubwin()->add_spinner("B:", GLUI_SPINNER_FLOAT, &cBlueWin.b);

	cWhiteWin._AddGluiWin();
	cWhiteWin._AddGluiSubwin(GLUI_SUBWINDOW_LEFT);
	CGlutWin::_AddWin(&cWhiteWin,	"WHITE", false, 400, 100, 256, 256);
		cWhiteWin.PCGetGluiSubwin()->add_spinner("R:", GLUI_SPINNER_FLOAT, &cWhiteWin.r);
		cWhiteWin.PCGetGluiSubwin()->add_spinner("G:", GLUI_SPINNER_FLOAT, &cWhiteWin.g);
		cWhiteWin.PCGetGluiWin()->add_spinner("B:", GLUI_SPINNER_FLOAT, &cWhiteWin.b);

	#endif	// MOD-BY-LEETEN 08/11/2008-END
	#endif	// MOD-BY-LEETEN 08/09/2008-END

	glutMainLoop();
	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.2  2008/08/10 05:03:56  leeten

[2008/08/10]
1. Include GlutWin.h to replace gw.h
2. Create three windows.
3. Use the static member CGlutWin::_AddWin to create windows.

Revision 1.1.1.1  2008/08/09 13:50:10  leeten

[2008/08/09]
1. First time checkin. LIBGW is a OO-based library to create GLUT window. In each window, there is a 3D coordinate for the object and mouse countrol to manipulate the coordinate and viewpoint.


*/
