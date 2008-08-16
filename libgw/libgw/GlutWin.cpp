#include "GlutWin.h"

void 
CGlutWin::_AlignPan(double pdNewCoord[3], double pdOldCoord[3])
{
	if( 0 == (GLUT_ACTIVE_ALT & eModifier) )
		return;

	size_t uMaxDir = 0;
	for(size_t d = 1; d<3; d++)
		if( fabs(pdNewCoord[d] - pdOldCoord[d]) > 
			fabs(pdNewCoord[uMaxDir] - pdOldCoord[uMaxDir]) )
			uMaxDir = d;

	for(size_t d = 0; d<3; d++)
		if( d != uMaxDir )
			pdNewCoord[d] = pdOldCoord[d];
}

void 
CGlutWin::_AlignRotate(double pdAxis[3])
{
	if( 0 == (GLUT_ACTIVE_ALT & eModifier) )
		return;

	size_t uMaxDir = 0;	// X
	for(size_t d = 1; d < 3; d++)
		if( fabs(pdAxis[d]) > fabs(pdAxis[uMaxDir]) )
			uMaxDir = d;

	for(size_t d = 0; d < 3; d++)
		pdAxis[d] = (uMaxDir == d)?((pdAxis[d]>0.0)?1.0:-1.0):0.0;
}

///////////////////////////////////////////////////////////////
// functions to manipulate the model and camera
void
trackball_ptov(int x, int y, int width, int height, double v[3])
{
    double d, a;
    /* project x,y onto a hemisphere centered within width, height , note z is up here*/
    v[0] = (double)(2*x - width) / (double)width;
    v[1] = (double)(2*y - height) / (double)height;    
    d = sqrt(v[0]*v[0] + v[1]*v[1]);
	v[2] = 0.0;
	if( d < 1.0 )
		v[2] = cos(M_PI_2 * d);
    a = 1.0f / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *=a;    v[1] *= a;    v[2] *= a;
}

void
CGlutWin::_RotateCamera()
{
	trackball_ptov(piViewport[2]/2, piViewport[3]/2, piViewport[2], piViewport[3], pdBeginPos); 
	trackball_ptov(piViewport[2]/2 + iCursorX - iBeginX, piViewport[3]/2 + iCursorY - iBeginY, piViewport[2], piViewport[3], pdCurPos);

	dX = pdCurPos[0] - pdBeginPos[0];
	dY = pdCurPos[1] - pdBeginPos[1];
	dZ = pdCurPos[2] - pdBeginPos[2];

	if (dX || dY || dZ) 
	{
		/* compute theta and cross product */
		double pdAxis[3];
		double dAngle = 0.1 * 90.0 * sqrt(dX*dX + dY*dY + dZ*dZ);
		pdAxis[0] = pdBeginPos[1] * pdCurPos[2] - pdBeginPos[2] * pdCurPos[1];
		pdAxis[1] = pdBeginPos[2] * pdCurPos[0] - pdBeginPos[0] * pdCurPos[2];
		pdAxis[2] = pdBeginPos[0] * pdCurPos[1] - pdBeginPos[1] * pdCurPos[0];

		// ADD-BY-LEETEN 03/31/2008-BEGIN
		_AlignRotate(pdAxis);
		// ADD-BY-LEETEN 03/31/2008-END

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotated(dAngle, pdAxis[0], pdAxis[1], pdAxis[2]);
		glMultMatrixd(tViewMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, tViewMatrix);

		glutPostRedisplay();
	}
}

void 
CGlutWin::_PanCamera()
{
	gluUnProject(
		(double)iBeginX, (double)iBeginY, 0.0, 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdOldCoord[0], &pdOldCoord[1], &pdOldCoord[2]);

	gluUnProject(
		(double)iCursorX, (double)iCursorY, 0.0, 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdNewCoord[0], &pdNewCoord[1], &pdNewCoord[2]);

	_AlignPan(pdNewCoord, pdOldCoord);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(tViewMatrix);
	glTranslated(pdOldCoord[0] - pdNewCoord[0], (pdOldCoord[1] - pdNewCoord[1]), pdOldCoord[2] - pdNewCoord[2]);
	glGetDoublev(GL_MODELVIEW_MATRIX, tViewMatrix);
	glutPostRedisplay();
}

void 
CGlutWin::_ZoomCamera()
{
	gluUnProject(
		(double)piViewport[2]/2.0, (double)piViewport[3]/2.0, ((double)(iBeginY)/(double)piViewport[3]) * 2.0 - 1.0, 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdOldCoord[0], &pdOldCoord[1], &pdOldCoord[2]);

	gluUnProject(
		(double)piViewport[2]/2.0, (double)piViewport[3]/2.0, ((double)(iCursorY)/(double)piViewport[3]) * 2.0 - 1.0, 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdNewCoord[0], &pdNewCoord[1], &pdNewCoord[2]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(tViewMatrix);
	glTranslated(pdOldCoord[0] - pdNewCoord[0], (pdOldCoord[1] - pdNewCoord[1]), pdOldCoord[2] - pdNewCoord[2]);
	glGetDoublev(GL_MODELVIEW_MATRIX, tViewMatrix);
	glutPostRedisplay();
}

void 
CGlutWin::_PanModel()
{
	gluProject(
		tModelMatrix[12], tModelMatrix[13], tModelMatrix[14], 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdOldCoord[0], &pdOldCoord[1], &pdOldCoord[2]);

	gluUnProject(
		(double)iCursorX + dXDiffOnScreen, (double)iCursorY + dYDiffOnScreen, pdOldCoord[2],
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdNewCoord[0], &pdNewCoord[1], &pdNewCoord[2]);

	for(int i=0; i<3; i++)
		tModelMatrix[12 + i] = pdNewCoord[i];

	glutPostRedisplay();
}

void 
CGlutWin::_RotateModel()
{
	trackball_ptov(piViewport[2]/2, piViewport[3]/2, piViewport[2], piViewport[3], pdBeginPos); 
	trackball_ptov(piViewport[2]/2 + iCursorX - iBeginX, piViewport[3]/2 + iCursorY - iBeginY, piViewport[2], piViewport[3], pdCurPos);

	dX = pdCurPos[0] - pdBeginPos[0];
	dY = pdCurPos[1] - pdBeginPos[1];
	dZ = pdCurPos[2] - pdBeginPos[2];

	if (dX || dY || dZ) 
	{
		/* compute theta and cross product */
		double pdAxis[3];
		double dAngle = 0.1 * 90.0 * sqrt(dX*dX + dY*dY + dZ*dZ);
		pdAxis[0] = pdBeginPos[1] * pdCurPos[2] - pdBeginPos[2] * pdCurPos[1];
		pdAxis[1] = pdBeginPos[2] * pdCurPos[0] - pdBeginPos[0] * pdCurPos[2];
		pdAxis[2] = pdBeginPos[0] * pdCurPos[1] - pdBeginPos[1] * pdCurPos[0];

		_AlignRotate(pdAxis);

		// Produce a new model matrix M' s.t.
		// the rotation R should be consistent to the final camera coordinate
		// i.e., M * V * R = M' * V 
		// => M' = M * V * R * V^-1 = M * V * R * V^T
		// Here assums that the transform of the view matrix is always a rigid transform.
		// Note the since only rotation is considered, the translation offsets are ignored 
		// during the multipilication. 
		static TMatrix tTempMatrix;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// reset the temp. matrix
		memset(tTempMatrix, 0, sizeof(tTempMatrix));

		// apply transpose
		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
				tTempMatrix[i * 4 + j] = tViewMatrix[j * 4 + i];

		for(int i=0; i<3; i++)
			tTempMatrix[4 * i + 3] = 0;

		glMultMatrixd(tTempMatrix);

		// now apply the rotation
		glRotated(dAngle, pdAxis[0], pdAxis[1], pdAxis[2]);

		// apply the view matrix
		memcpy(tTempMatrix, tViewMatrix, sizeof(tTempMatrix));
		memset(&tTempMatrix[12], 0, 3 * sizeof(tTempMatrix[0]));
		glMultMatrixd(tTempMatrix);

		// apply the original model matrix
		memcpy(tTempMatrix, tModelMatrix, sizeof(tTempMatrix));
		memset(&tTempMatrix[12], 0, 3 * sizeof(tTempMatrix[0]));
		glMultMatrixd(tTempMatrix);

		// get the final matrix
		glGetDoublev(GL_MODELVIEW_MATRIX, tTempMatrix);

		// restore the translation offset
		memcpy(tModelMatrix, tTempMatrix, 12 * sizeof(tTempMatrix[0]));

		glutPostRedisplay();
	}
}

void 
CGlutWin::_ZoomModel()
{
	gluProject(
		tModelMatrix[12], tModelMatrix[13], tModelMatrix[14], 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdOldCoord[0], &pdOldCoord[1], &pdOldCoord[2]);

	pdOldCoord[2] -= 0.01 * ((double)(iCursorY - iBeginY)/(double)piViewport[3]);

	gluUnProject(
		pdOldCoord[0], pdOldCoord[1], pdOldCoord[2],
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdNewCoord[0], &pdNewCoord[1], &pdNewCoord[2]);

	for(int i=0; i<3; i++)
		tModelMatrix[12 + i] = pdNewCoord[i];

	glutPostRedisplay();
}

// ADD-BY-LEETEN 08/12/2008-BEGIN
char *
CGlutWin::SZSprintf
(
	const char *szFormat, ...
)
{
	static char szBuffer[4096+1];
	va_list args;
	va_start (args, szFormat);
	vsprintf(szBuffer, szFormat, args);
	va_end (args);
	return szBuffer;
}

				// draw a string on the screen. It will be drawn in the origin in current coordinate
void
CGlutWin::_DrawString(char *szString)
{
	glRasterPos3f(0.0, 0.0, 0.0);
	for(const char *pc = szString; *pc; pc++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *pc);
}

				// print out a string on the console with a prefix to indicate this window 
				// a newline will be printed at the end.
void			
CGlutWin::_AddToLog(char *szString, FILE* fpOutput)
{
	fprintf(fpOutput, "[GLUTWIN-%s]: %s\n", szTitle, szString);
}
// ADD-BY-LEETEN 08/12/2008-END

void 
CGlutWin::_DisplayCB()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixd(tProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(tViewMatrix);

	glMultMatrixd(tModelMatrix);

	_DisplayFunc();

	// ADD-BY-LEETEN 08/12/2008-BEGIN
						// update the FPS
	cFps.Update();

						// draw the FPS on the screen
	if( bDisplayFps )
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-1.0f, -1.0f, 0.0f);
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);

		float fFps = cFps.GetFps();
		_DrawString(SZSprintf("FPS = %f", fFps));
	}

	if( CGlutWin::bSwapBuffer )
		glutSwapBuffers();	
	// ADD-BY-LEETEN 08/12/2008-END

	CHECK_OPENGL_ERROR(szDisplay, true);
}

void 
CGlutWin::_ReshapeCB(int w, int h)
{
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport(tx, ty, tw, th);
	// ADD-BY-LEETEN 08/13/2008-BEGIN
	glGetIntegerv(GL_VIEWPORT, piViewport);
	// ADD-BY-LEETEN 08/13/2008-END

	if( tw & th )
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0f, (float)piViewport[2]/(float)piViewport[3], 0.05f, 20.0f);
		glGetDoublev(GL_PROJECTION_MATRIX, tProjectionMatrix);
	
		_ReshapeFunc(tw, th);
	}

	CHECK_OPENGL_ERROR(szReshape, true);
}

// ADD-BY-LEETEN 08/13/2008-BEGIN
void 
CGlutWin::_UpdateWinCoord(int *piX, int *piY, bool bFlipY)
{
	*piX -= piViewport[0];
	*piY -= piViewport[1];
	if( bFlipY )
		*piY = piViewport[3] - *piY;
}
// ADD-BY-LEETEN 08/13/2008-END

void 
CGlutWin::_KeyboardCB(unsigned char key, int x, int y)
{
	// ADD-BY-LEETEN 08/13/2008-BEGIN
	_UpdateWinCoord(&x, &y);
	// ADD-BY-LEETEN 08/13/2008-END

	switch(key) {
		case 'x':	case 'X':
			memcpy(tViewMatrix, tInitViewMatrix, sizeof(tViewMatrix));;

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glMultMatrixd(tInitModelMatrix);
			glRotated(('x'==key)?90.0:-90, 0.0, 1.0, 0.0);
			glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);

			glutPostRedisplay();
			break;

		case 'y':	case 'Y':
			memcpy(tViewMatrix, tInitViewMatrix, sizeof(tViewMatrix));;

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glMultMatrixd(tInitModelMatrix);
			glRotated(('y'==key)?90.0:-90, 1.0, 0.0, 0.0);
			glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);

			glutPostRedisplay();
			break;

		case 'z':	case 'Z':
			memcpy(tViewMatrix, tInitViewMatrix, sizeof(tViewMatrix));;

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glMultMatrixd(tInitModelMatrix);
			glRotated(('z'==key)?0.0:180, 0.0, 1.0, 0.0);
			glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);

			glutPostRedisplay();
			break;

		case 'r': case 'R':
			memcpy(tViewMatrix, tInitViewMatrix, sizeof(tViewMatrix));;
			memcpy(tModelMatrix, tInitModelMatrix, sizeof(tModelMatrix));;

			glutPostRedisplay();
			break;

		case 'N': case 'n':
			glutPostRedisplay();
			break;

		default:
			_KeyboardFunc(key, x, y);
			;
	}
}

void 
CGlutWin::_SpecialCB(int skey, int x, int y)
{
	// ADD-BY-LEETEN 08/13/2008-BEGIN
	_UpdateWinCoord(&x, &y);
	// ADD-BY-LEETEN 08/13/2008-END

	switch(skey) {
		case GLUT_KEY_LEFT:
			break;

		case GLUT_KEY_RIGHT:
			break;

		case GLUT_KEY_UP:
			break;

		case GLUT_KEY_DOWN:
			break;

		default:
			_SpecialFunc(skey, x, y);
	}
}

void 
CGlutWin::_MotionCB(int x, int y)
{
	_UpdateWinCoord(&x, &y);

	iCursorX = x;
	iCursorY = y;
}

void 
CGlutWin::_MouseCB(int button, int state, int x, int y)
{
	_UpdateWinCoord(&x, &y);

	iCursorX = x;
	iCursorY = y;
	iBeginX = x;
	iBeginY = y;
	eMouseButton = button;
	eModifier = glutGetModifiers();
	bMoving = (state == GLUT_DOWN)?true:false;

	double pdCurrentModelCenterOnScreen[3];
	gluProject(
		tModelMatrix[12], tModelMatrix[13], tModelMatrix[14], 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdCurrentModelCenterOnScreen[0], &pdCurrentModelCenterOnScreen[1], &pdCurrentModelCenterOnScreen[2]);

	dXDiffOnScreen = pdCurrentModelCenterOnScreen[0] - (double)iCursorX;
	dYDiffOnScreen = pdCurrentModelCenterOnScreen[1] - (double)iCursorY;
}

void 
CGlutWin::_IdleCB()
{
	if(bMoving)
	{
		switch(eMouseButton) 
		{
		case GLUT_LEFT_BUTTON: // pan
			switch( eModifier & ~GLUT_ACTIVE_ALT )
			{
			case 0:
				_PanModel();
				glutPostRedisplay();	
				break;

			case GLUT_ACTIVE_CTRL: // manipulate the object
				_RotateModel();
				glutPostRedisplay();	
				break;

			case GLUT_ACTIVE_SHIFT:
				_ZoomModel();
				glutPostRedisplay();	
				break;
			} // switch(eModifier)
			break;

		case GLUT_RIGHT_BUTTON:
			switch( eModifier & ~GLUT_ACTIVE_ALT )
			{
			case 0:
				_PanCamera();
				glutPostRedisplay();	
				break;

			case GLUT_ACTIVE_CTRL: // manipulate the object
				_RotateCamera();
				glutPostRedisplay();	
				break;

			case GLUT_ACTIVE_SHIFT:
				_ZoomCamera();
				glutPostRedisplay();	
				break;
			} // switch(eModifier)
			break;
		} // switch(eMouseButton) 
	} // if(bMoving)

	_IdleFunc();

	// ADD-BY-LEETEN 08/12/2008-BEGIN
	if( bKeepUpdate )
		_Redisplay();
	// ADD-BY-LEETEN 08/12/2008-END
}

void 
CGlutWin::_DisplayFunc()
{
}

void 
CGlutWin::_ReshapeFunc(int w, int h)
{
}

void 
CGlutWin::_KeyboardFunc(unsigned char key, int x, int y)
{
}

void 
CGlutWin::_SpecialFunc(int skey, int x, int y)
{
}

void 
CGlutWin::_IdleFunc()
{
}

void 
CGlutWin::_InitFunc()
{
}

// constructor
CGlutWin::CGlutWin()
{
	iBeginX = 0;
	iBeginY = 0;
	iCursorX = 0;
	iCursorY = 0;
	eMouseButton = 0;
	eModifier = 0;
	bMoving = false;

	// ADD-BY-LEETEN 08/11/2008-BEGIN
	iGluiEnum = GLUI_NONE;	// by default there is no GLUI control
	pcGluiWin = NULL;		// by default there is no GLUI window
	pcGluiSubwin = NULL;	// by default there is no GLUI sub-window
	iSubwinPosistion = 0;	
	// ADD-BY-LEETEN 08/11/2008-END

	// ADD-BY-LEETEN 08/12/2008-BEGIN
	bDisplayFps = false;	// by default the FPS is not shown
	bKeepUpdate = false;	// by default the frame is not keep updating
	// ADD-BY-LEETEN 08/12/2008-END

}

// destructor
CGlutWin::~CGlutWin()
{
}

int 
CGlutWin::IGetId()
{
	return iId;
}

// ADD-BY-LEETEN 08/12/2008-BEGIN

			// enabling keeping sending redisplay event
void 
CGlutWin::_KeepUpdateOn()
{
	bKeepUpdate = true;
}

			// disabling keeping sending redisplay event
void 
CGlutWin::_KeepUpdateOff()
{
	bKeepUpdate = false;
}

			// return the FPS of previous frame
float 
CGlutWin::fGetFps()
{
	return cFps.GetFps();
}

			// enable displaying FPS on the screen
void 
CGlutWin::_DisplayFpsOn()
{
	bDisplayFps = true;
}

			// disable displaying FPS on the screen
void 
CGlutWin::_DisplayFpsOff()
{
	bDisplayFps = false;
}

// ADD-BY-LEETEN 08/12/2008-END

int 
CGlutWin::ICreate(
	char *szTitle, 
	bool bUseDefault,
	int iX, int iY, int iW, int iH)
{
	strcpy(this->szTitle, szTitle);
	sprintf(szReshape,	"%s: _Reshape() ", szTitle);
	sprintf(szDisplay,	"%s: _Display() ", szTitle);
	sprintf(szInit,		"%s: _Init() ",	szTitle);
	iId = glutCreateWindow(szTitle);

	if( !bUseDefault )
	{
		glutPositionWindow(iX, iY);
		glutReshapeWindow(iW, iH);
	}

	// the projection matrix is decided in the _ReshapeCB();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glGetDoublev(GL_MODELVIEW_MATRIX, tViewMatrix);
	memcpy(tInitViewMatrix, tViewMatrix, sizeof(tViewMatrix));
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);
	memcpy(tInitModelMatrix, tModelMatrix, sizeof(tModelMatrix));

	// ADD-BY-TLEE 08/16/2008-BEGIN
										// move the creation of GLUI win/subwin from the static method CGlutWin::_AddWin to here
	if( iGluiEnum & GLUI_SUBWIN )
	{
		pcGluiSubwin = GLUI_Master.create_glui_subwindow(iId, iSubwinPosistion);
		pcGluiSubwin->set_main_gfx_window( iId );
	}

	if( iGluiEnum & GLUI_WIN )
	{
		static char szGluiTitle[1024+1];
		sprintf(szGluiTitle, "%s-GLUI", szTitle);
		pcGluiWin = GLUI_Master.create_glui(szGluiTitle);
		pcGluiWin->set_main_gfx_window( iId );
	}

	_InitFunc();

	_AddWin(this);
	// ADD-BY-TLEE 08/16/2008-END

	CHECK_OPENGL_ERROR(szInit, true);

	return iId;
}

// ADD-BY-LEETEN 08/09/2008-BEGIN
void 
CGlutWin::_Set()
{
	if( iId > 0 )
	{
		glutSetWindow(iId);
	}
	else
	{
		fprintf(stderr, "Warning in CGlutWin::_Set(): the window has ben not created yet.\n");
	}
}

// ADD-BY-LEETEN 08/09/2008-END

// ADD-BY-LEETEN 08/11/2008-BEGIN

// add a GLUI window 
void 
CGlutWin::_AddGluiWin()
{
	iGluiEnum |= GLUI_WIN;
}

// return the pointer to the GLUI window
GLUI *
CGlutWin::PCGetGluiWin()
{
	assert(pcGluiWin);
	return pcGluiWin;
}

// add a GLUI subwindow in the specified position
void 
CGlutWin::_AddGluiSubwin(int iPosition)
{
	iGluiEnum |= GLUI_SUBWIN;
	iSubwinPosistion = iPosition;
}

// return the pointer to the GLUI sub-window
GLUI *
CGlutWin::PCGetGluiSubwin()
{
	assert(pcGluiSubwin);
	return pcGluiSubwin;
}

// ADD-BY-LEETEN 08/11/2008-END

// ADD-BY-LEETEN 08/11/2008-BEGIN
void 
CGlutWin::_Redisplay()
{
_Begin();
	glutPostRedisplay();
_End();
}

void
CGlutWin::_TimerFunc(int value)
{
}

						// The member method to handle the callback
						// It is called by CGlutWin_static:_TimerCB.
void 
CGlutWin::_TimerCB(int value)
{
	// call the timer func
	_TimerFunc(value);
}

void 
CGlutWin::_AddTimer(unsigned int msecs, short value)
{
						// call the static method to create a timer event for this window
						// the static method will implicitly combine this window's id and the value

	CGlutWin::_AddTimer(this, msecs, value);
}
// ADD-BY-LEETEN 08/11/2008-END

// ADD-BY-LEETEN 08/13/2008-BEGIN
void
CGlutWin::_GluiFunc(unsigned short usValue)
{
}

void 
CGlutWin::_GluiCB(unsigned short usValue)
{
	_GluiFunc(usValue);
}

#if	0	// MOD-BY-LEETEN 08/15/2008-FROM:
	void 
	CGlutWin::_AddButton(char *szName, unsigned short usValue)
	{
		CGlutWin::_AddButton(this, szName, usValue);
	}
#else	// MOD-BY-LEETEN 08/15/2008-TO:
								// Add a new parameter to specify the panel

void 
CGlutWin::_AddButton(char *szName, unsigned short usValue, GLUI_Panel *pcPanel)
{
	CGlutWin::_AddButton(this, szName, usValue, pcPanel);
}

#endif	// MOD-BY-LEETEN 08/15/2008-END

// ADD-BY-LEETEN 08/13/2008-END

		// push this window
void 
CGlutWin::_Push()
{
	if( iId > 0 )
		glutPushWindow();

}

		// pop this window
void 
CGlutWin::_Pop()
{
	if( iId > 0 )
		glutPopWindow();
}

		// push current window ID
void 
CGlutWin::_PushWid()
{
	if( iId )
		cWid_stack.push(glutGetWindow());
}

// pop current window ID
void				
CGlutWin::_PopWid()
{
	if( iId )
	{
		glutSetWindow(cWid_stack.top());
		cWid_stack.pop();
	}
}

		// beginning of a new method
void 
CGlutWin::_Begin()
{
	_PushWid();
	_Set();
}

		// ending of a new method
void 
CGlutWin::_End()
{
	_PopWid();
}


/*

$Log: not supported by cvs2svn $
Revision 1.6  2008/08/15 14:36:18  leeten

[2008/08/15]
1. [CHANGE] Add a new parameter to method _AddButton to specify the panel.

Revision 1.5  2008/08/13 20:55:36  leeten

[2008/08/13]
1. [ADD] Define a new method SZSprint to format string.
2. [ADD] Define a new method _DrawString() to draw a string in the left bottom corner.
3. [ADD] Define a new method _AddToLog to add a string to specifed file (by default STDERR). When printing this string, a prefix will be added to indicate which window display this string, and a new line will be automatically appended at the end.
4. [ADD] Add code to measure FPS. FPS can be shown on screen via another new method _DisplayFpsOn();
5. [CHANGE] Since GLUI subwindow can change the both location and size of the viewport, the viewport is recorded, and a new method _UpdateWinCoord is defined to adjust the passed coordinate.
6. [ADD] Add a new method _KeepUpdateOn() to keep displaying the screen. It can be disabled via another method _KeepUpdateOff().
7. [CHANGE] Change the order of _InitFunc() s.t. it is called after the GLUI win/subwindows have been created.
8. [ADD] Add a new callback _GluiCB() to handle event from GLUI control. It will call another method _GluiFunc().
9. [ADD] Add a new method _AddButton() to add a button to the GLUI windows/subwindows.

Revision 1.4  2008/08/12 16:38:34  leeten

[2008/08/12]
1. [CHANGE] In _Set, if the windows is not created yet, send a warning other than terminating the application.
2. [ADD] Define new methods to similar GLUT APIs: _Push(), _Pop() and _Redisplay().
3. [ADD] Support the timer event. To trigger a timer event, the application can call a static method _AddTimer(). to process the timer event, a new callback _TimerCB() is defined. This callback will invoke another method _TimerFunc().
4. [ADD] Define methods _Begin(), _End(), _PushWid(), _PopWid() to avoid the misorder of active windows since the timer event can shuffle the order among the GLUT windows.
5. [ADD] Define a new method _GetWin() to conver a GLUT window ID to the order in the vector vcWins.

Revision 1.3  2008/08/11 04:26:32  leeten

[2008/08/11]
1. [ADD] Combine with GLUI.
2. [ADD] Modify the reshape func. s.t. the final viewport is decided via GLUI_Master.get_viewport_area().
3. [ADD] Add new routine to enable GLUI subwin/win and return the GLUI pointers.

Revision 1.2  2008/08/10 18:50:53  leeten

[2008/08/10]
1. [DEL] Remove the header gw_api.h.

Revision 1.1  2008/08/10 05:00:27  leeten

[2008/08/10]
1. First time checkin. Now we use the static member methods to add windows.

Revision 1.1.1.1  2008/08/09 13:50:10  leeten

[2008/08/09]
1. First time checkin. LIBGW is a OO-based library to create GLUT window. In each window, there is a 3D coordinate for the object and mouse countrol to manipulate the coordinate and viewpoint.


*/