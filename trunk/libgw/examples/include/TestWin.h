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

	void 
	_PrintEvent(unsigned int uiCbId, va_list vaArgs)
	{
		switch( uiCbId )
		{
		case CGlutWin::CB_DISPLAY:
			_AddToLog("DISPLAY");
			break;
		case CGlutWin::CB_RESHAPE:
			{
				int w = va_arg(vaArgs, int);
				int h = va_arg(vaArgs, int);
				_AddToLog(SZSprintf("RESHAPE %d %d", w, h));
			}
			break;
		case CGlutWin::CB_KEYBOARD:
			{
				int ub_key = va_arg(vaArgs, int);
				int w = va_arg(vaArgs, int);
				int h = va_arg(vaArgs, int);
				_AddToLog(SZSprintf("KEYBOARD %c %d %d", ub_key, w, h));
			}
			break;
		case CGlutWin::CB_SPECIAL:
			{
				int skey = va_arg(vaArgs, int);
				int w = va_arg(vaArgs, int);
				int h = va_arg(vaArgs, int);
				_AddToLog(SZSprintf("SPECIAL %d %d %d", skey, w, h));
			}
			break;
		case CGlutWin::CB_MOTION:
			{
				int x = va_arg(vaArgs, int);
				int y = va_arg(vaArgs, int);
				_AddToLog(SZSprintf("MOTION %d %d", x, y));
			}
			break;
		case CGlutWin::CB_MOUSE:
			{
				int button = va_arg(vaArgs, int);
				int state = va_arg(vaArgs, int);
				int x = va_arg(vaArgs, int);
				int y = va_arg(vaArgs, int);
				_AddToLog(SZSprintf("MOUSE %d %d %d %d", button, state, x, y));
			}
			break;
		case CGlutWin::CB_TIMER:
			{
				int button = va_arg(vaArgs, int);
				int state = va_arg(vaArgs, int);
				int x = va_arg(vaArgs, int);
				int y = va_arg(vaArgs, int);
				_AddToLog(SZSprintf("MOUSE %d %d %d %d", button, state, x, y));
			}
			break;
		case CGlutWin::CB_GLUI:
			;
		}
	}
};


