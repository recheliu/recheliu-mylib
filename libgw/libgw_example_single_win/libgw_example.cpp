#include "gw.h"

class CTestWin:public CGlutWin {
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

CTestWin cGreenWin(0.0, 1.0, 0.0, 1.0), cRedWin(1.0, 0.0, 0.0, 1.0);

int
main(int argn, char *argv[])
{
	glutInit(&argn, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL );

	_GWAddWin(&cGreenWin,	"GREEN");
	_GWAddWin(&cRedWin,		"RED");

	glutMainLoop();
	return 0;
}

/*

$Log: not supported by cvs2svn $

*/
