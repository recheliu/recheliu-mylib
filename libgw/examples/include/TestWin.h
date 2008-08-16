/***************************************************

TestWin:

This header define a simeple subclass of CGlutWin. It 
is mainly used to illustrate the example in my manual.

***************************************************/

#include "GlutWin.h"

class CTestWin:public CGlutWin {
	void _DisplayFunc()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glColor4f(r, g, b, a);
		glutWireCube(1.0f);
	}

public:
	// the colors
	float r, g, b, a;

	CTestWin()	{};
	CTestWin(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};


/*

$Log: not supported by cvs2svn $

*/