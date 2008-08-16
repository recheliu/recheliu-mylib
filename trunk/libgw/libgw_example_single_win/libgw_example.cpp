#include "GlutWin.h"

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
	CGlutWin::_Init(
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

	// MOD-BY-TLEE 2008/08/16-FROM:
		//	CGlutWin::_AddWin(&cRedWin,		"RED", false, 100, 100, 256, 256);
	// TO:
	cRedWin.ICreate("RED", false, 100, 100, 256, 256);
	// MOD-BY-TLEE 2008/08/16-END
		// ADD-BY-LEETEN 08/12/2008-BEGIN
		cRedWin._DisplayFpsOn();
		cRedWin._KeepUpdateOn();
		// ADD-BY-LEETEN 08/11/2008-END

	cGreenWin._AddGluiWin();
	// MOD-BY-TLEE 2008/08/16-FROM:
		//	CGlutWin::_AddWin(&cGreenWin,	"GREEN", false, 200, 100, 256, 256);
	// TO:
	cGreenWin.ICreate("GREEN", false, 200, 100, 256, 256);
	// MOD-BY-TLEE 2008/08/16-END
	cGreenWin.PCGetGluiWin()->add_spinner("G:", GLUI_SPINNER_FLOAT, &cGreenWin.g);

	cBlueWin._AddGluiSubwin(GLUI_SUBWINDOW_RIGHT);
	// MOD-BY-TLEE 2008/08/16-FROM:
		//	CGlutWin::_AddWin(&cBlueWin,	"BLUE", false, 300, 100, 256, 256);
	// TO:
	cBlueWin.ICreate("BLUE", false, 300, 100, 256, 256);
	// MOD-BY-TLEE 2008/08/16-END
		cBlueWin.PCGetGluiSubwin()->add_spinner("B:", GLUI_SPINNER_FLOAT, &cBlueWin.b);

	cWhiteWin._AddGluiWin();
	cWhiteWin._AddGluiSubwin(GLUI_SUBWINDOW_LEFT);
	// MOD-BY-TLEE 2008/08/16-FROM:
		//	CGlutWin::_AddWin(&cWhiteWin,	"WHITE", false, 400, 100, 256, 256);
	// TO:
	cWhiteWin.ICreate("WHITE", false, 400, 100, 256, 256);
	// MOD-BY-TLEE 2008/08/16-END
		cWhiteWin.PCGetGluiSubwin()->add_spinner("R:", GLUI_SPINNER_FLOAT, &cWhiteWin.r);
		cWhiteWin.PCGetGluiSubwin()->add_spinner("G:", GLUI_SPINNER_FLOAT, &cWhiteWin.g);
		cWhiteWin.PCGetGluiWin()->add_spinner("B:", GLUI_SPINNER_FLOAT, &cWhiteWin.b);

	glutMainLoop();
	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.4  2008/08/13 21:03:48  leeten

[2008/08/13]
1. [CHANGE] Use the new static method CGlutWin::_Init other than glutInit to initialize OpenGL/GLUT. Since the swap of buffer will be implictly done in the member callback _DisplayCB(), it is removed.
2. [ADD] Make the RED window can display FPS and keep update.

Revision 1.3  2008/08/11 04:35:06  leeten

[2008/08/11]
1. [CHANGE] Declare the color variables as public.
2. [ADD] Add a new window to draw a 3D white box. This window is associated w/ a GLUI win and a GLUI subwin to control the RGB channels.
3. [ADD] Add a GLUI window to the GREEN window to change the green channel.
4. [ADD] Add a GLUI sub-window to the BLUE window to change the blue channel.

Revision 1.2  2008/08/10 05:03:56  leeten

[2008/08/10]
1. Include GlutWin.h to replace gw.h
2. Create three windows.
3. Use the static member CGlutWin::_AddWin to create windows.

Revision 1.1.1.1  2008/08/09 13:50:10  leeten

[2008/08/09]
1. First time checkin. LIBGW is a OO-based library to create GLUT window. In each window, there is a 3D coordinate for the object and mouse countrol to manipulate the coordinate and viewpoint.


*/
