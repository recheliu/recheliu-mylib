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
	iNrOfEntries = 0;
	pcTransFunc = NULL;
}

CTfWin::~CTfWin(void)
{
}

// inhered interfaces
void 
CTfWin::_DisplayFunc()
{
	assert(iNrOfEntries);
	glClear(GL_COLOR_BUFFER_BIT);

	pcTransFunc->_ExportColorMap(&pfColorMap[0], iNrOfEntries);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

	if( cHistogram.pfBins.BIsAllocated() )
	{
		// normalize the coordinate
		glTranslatef(-1.0f, -1.0f, -1.0f);
		glScalef(2.0f, 2.0f, 2.0f);

		float fDomainMin  = pcTransFunc->cDomainMin.FGetValue();
		float fDomainMax  = pcTransFunc->cDomainMax.FGetValue();
		float fHistogramMin = cHistogram.cMin.FGetValue();
		float fHistogramMax = cHistogram.cMax.FGetValue();

		glPushMatrix();
		glScalef(1.0f / (float)(fHistogramMax - fHistogramMin), 1.0f, 1.0f);
		glTranslatef(-fHistogramMin, 0.0f, 0.0f);
		
		glTranslatef(fDomainMin, 0.0f, 0.0f);
		glScalef((fDomainMax - fDomainMin)/(float)iNrOfEntries, 1.0f, 1.0f);

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

		glColor4f(1.0, 1.0, 1.0, 1.0);
		_DrawString3D(SZSprintf("%.2e", fDomainMin), 0.0f, 0.5f, 0.0f);
		_DrawString3D(SZSprintf("%.2e", fDomainMax), (float)iNrOfEntries, 0.5f, 0.0f);

		glPopMatrix();

		/////////////////////////////////////////////////////
		glPushMatrix();
		glScalef(1.0f / (float)(float)cHistogram.pfBins.USize(), 1.0f, 1.0f);

		glPushAttrib(GL_LINE_BIT);
		glLineWidth(4.0);

		glBegin(GL_LINE_STRIP);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		for(int i = 0; i < (int)cHistogram.pfBins.USize(); i++)
		{
			glVertex2f((float)i, cHistogram.pfBins[i]);
			glVertex2f((float)i + 1.0f, cHistogram.pfBins[i]);
		}
		glEnd();
		glPopAttrib();	// glPushAttrib(GL_LINE_BIT);
		glPopMatrix();

		_DrawString(SZSprintf("%.2e", fHistogramMin), 0, 2, false);
		_DrawString(SZSprintf("%.2e", fHistogramMax), -1, 2, true);
		_DrawString(SZSprintf("%.2e", cHistogram.fMaxCount), 0, -16, false);
	}
	else
	{

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

		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		_DrawString(SZSprintf("%.2e", pcTransFunc->cDomainMin.FGetValue()), 0, 0, false);
		_DrawString(SZSprintf("%.2e", pcTransFunc->cDomainMax.FGetValue()), -1, 0, true);

	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void 
CTfWin::_InitFunc()
{
	assert( pcTransFunc );

	_Reshape(512, 128);	

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

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

