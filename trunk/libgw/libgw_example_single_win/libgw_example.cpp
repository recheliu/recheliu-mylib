// MOD-BY-LEETEN 08/09/2008-FROM:
	// #include "gw.h"
// TO:
#include "GlutWin.h"
// MOD-BY-LEETEN 08/09/2008-END

class CTestWin:public CGlutWin {
	// the colors
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
	CGlutWin::_AddWin(&cRedWin,		"RED");
	CGlutWin::_AddWin(&cGreenWin,	"GREEN");
	CGlutWin::_AddWin(&cBlueWin,	"BLUE");
	#endif	// MOD-BY-LEETEN 08/09/2008-END

	glutMainLoop();
	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/09 13:50:10  leeten

[2008/08/09]
1. First time checkin. LIBGW is a OO-based library to create GLUT window. In each window, there is a 3D coordinate for the object and mouse countrol to manipulate the coordinate and viewpoint.


*/
