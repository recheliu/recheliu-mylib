/***********************************************

CTfWin: 

This file defines a class for displaying transfer functions. 
It is a subclass of CGlutWin, a class for creating GLUT/GLUI-based
display window. 

The procedure to use this class is as below:

1. declare a object of class CTfWin.
2. Set up the editing transfunc function of class CTransFunc via method _SetTransFunc. 
3. Invoke function CGlutWin::_AddWin() to create this window. If the transfer function is not
	set yet, this application will be terminated.
4. (Optional) Setup the #entries via method _SetNrOfEntries. 
5. (Optional) if this transfer function is shared by other objects such as CTfUi, it is highly 
	RECOMMENDED to call method _KeepUpdateOn() to keep the frame up to date.

************************************************/

#include "TfWin.h"

CTfWin::CTfWin(void)
{
	// MOD-BY-TLEE 08/14/2008-FROM:
		// iLid = 0;
	// TO:
	iNrOfEntries = 0;
	pcTransFunc = NULL;
	// MOD-BY-TLEE 08/14/2008-END
}

CTfWin::~CTfWin(void)
{
}

// inhered interfaces
void 
CTfWin::_DisplayFunc()
{
	#if	0		// MOD-BY-TLEE 08/14/2008-FROM:
		assert(iLid);
		glClear(GL_COLOR_BUFFER_BIT);

		glCallList(iLid);
	#else		// MOD-BY-TLEE 08/14/2008-TO:

	assert(iNrOfEntries);
	glClear(GL_COLOR_BUFFER_BIT);

	pcTransFunc->_ExportColorMap(&pfColorMap[0], iNrOfEntries);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		// normalize the coordinate
		glTranslatef(-1.0f, -1.0f, -1.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		glScalef(1.0f/(float)iNrOfEntries, 1.0f, 1.0f);

		// plot the transfer func. as bar chart
		// each entry in the TR is a bar
		// the height of each bar represents the corresponding alpha
		// the color  of each bar represents the corresponding color
		glBegin(GL_QUADS);
		for(int i = 0; i < iNrOfEntries; i++)
		{
			float l, r, b, t;
			l = (float)i;
			r = (float)i + 1;
			b = 0.0f;
			t = pfColorMap[i*4 + 3];	// use the alpha channel as the height
			glColor3fv(&pfColorMap[i*4]);	// setup the color

			glVertex2f(l, b);
			glVertex2f(r, b);
			glVertex2f(r, t);
			glVertex2f(l, t);
		}
		glEnd();

		// plot the transfer func. as lines
		for(int c = 0; c < 3; c++)	// only plot the RGB channels
		{
			switch(c)
			{
			case 0:	glColor4f(1.0, 0.0f, 0.0f, 1.0f);	break;
			case 1:	glColor4f(0.0, 1.0f, 0.0f, 1.0f);	break;
			case 2:	glColor4f(0.0, 0.0f, 1.0f, 1.0f);	break;
			}

			glBegin(GL_LINE_STRIP);
				for(int i = 0; i < iNrOfEntries; i++)
					glVertex2f((float)i, pfColorMap[i*4 + c]);
			glEnd();
		}

		// ADD-BY-TLEE 2008/08/21-BEGIN
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		_DrawString(SZSprintf("%.2e", pcTransFunc->fDomainMin), 0, 0, false);
		_DrawString(SZSprintf("%.2e", pcTransFunc->fDomainMax), -1, 0, true);
		// ADD-BY-TLEE 2008/08/21-END
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	#endif		// MOD-BY-TLEE 08/14/2008-END

	// DEL-BY-LEETEN 08/12/2008-BEGIN
		// glutSwapBuffers();
	// DEL-BY-LEETEN 08/12/2008-END
}

void 
CTfWin::_InitFunc()
{
	// ADD-BY-TLEE 08/14/2008-BEGIN
	assert( pcTransFunc );
	// ADD-BY-TLEE 08/14/2008-END

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// ADD-BY-TLEE 08/14/2008-BEGIN
void 
CTfWin::_SetTransFunc(CTransFunc* pcTransFunc)
{
	this->pcTransFunc = pcTransFunc;
}

void 
CTfWin::_SetNrOfEntries(int iNrOfEntries)
{
	assert( iNrOfEntries );
	this->iNrOfEntries = iNrOfEntries;
	pfColorMap.alloc(iNrOfEntries * CTransFunc::NR_OF_COLORS);
}
// ADD-BY-TLEE 08/14/2008-END

#if	0	// DEL-BY-TLEE 08/14/2008-BEGIN
void 
CTfWin::_SetTransferFunc(const float *pfTf, int iNrOfTfEntries)
{
_Begin();

	// the list will be created in the first time to call this method.
	if( 0 == iLid )
		iLid = glGenLists(1);

	glNewList(iLid, GL_COMPILE);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();

			// normalize the coordinate
			glTranslatef(-1.0f, -1.0f, -1.0f);
			glScalef(2.0f, 2.0f, 2.0f);
			glScalef(1.0f/(float)iNrOfTfEntries, 1.0f, 1.0f);

			// plot the transfer func. as bar chart
			// each entry in the TR is a bar
			// the height of each bar represents the corresponding alpha
			// the color  of each bar represents the corresponding color
			glBegin(GL_QUADS);
			for(int i = 0; i < iNrOfTfEntries; i++)
			{
				float l, r, b, t;
				l = (float)i;
				r = (float)i + 1;
				b = 0.0f;
				t = pfTf[i*4 + 3];	// use the alpha channel as the height
				glColor3fv(&pfTf[i*4]);	// setup the color

				glVertex2f(l, b);
				glVertex2f(r, b);
				glVertex2f(r, t);
				glVertex2f(l, t);
			}
			glEnd();

			// plot the transfer func. as lines
			for(int c = 0; c < 3; c++)	// only plot the RGB channels
			{
				switch(c)
				{
				case 0:	glColor4f(1.0, 0.0f, 0.0f, 1.0f);	break;
				case 1:	glColor4f(0.0, 1.0f, 0.0f, 1.0f);	break;
				case 2:	glColor4f(0.0, 0.0f, 1.0f, 1.0f);	break;
				}

				glBegin(GL_LINE_STRIP);
					for(int i = 0; i < iNrOfTfEntries; i++)
						glVertex2f((float)i, pfTf[i*4 + c]);
				glEnd();
			}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	glEndList();

_End();
}
#endif	// DEL-BY-TLEE 08/14/2008-END

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/14 14:44:02  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN]. This library defines classes for trasnfer functions, including editing and displaying.

Revision 1.2  2008/08/13 21:16:37  leeten

[2008/08/13]
1. [DEL] Remove glutSwapBuffers();

Revision 1.1.1.1  2008/08/12 16:58:49  leeten

[2008/08/12]
1. First time checkin. This is my DVR renderer for 3D regular volume. For testing, the dataset HIPIP is checkin too.


*/

