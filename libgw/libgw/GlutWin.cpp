// MOD-BY-LEETEN 08/09/2008-FROM:
	// #include "gw.h"
// TO:
#include "GlutWin.h"
// DEL-BY-LEETEN 08/10/2008-BEGIN
	// #include "gw_api.h"
// DEL-BY-LEETEN 08/10/2008-END
// MOD-BY-LEETEN 08/09/2008-END

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

	CHECK_OPENGL_ERROR(szDisplay, true);
}

void 
CGlutWin::_ReshapeCB(int w, int h)
{
	#if 0		// MOD-BY-LEETEN 08/11/2008-FROM:

		if( w & h )
		{
			glViewport(0, 0, w, h);

			glGetIntegerv(GL_VIEWPORT, piViewport);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(30.0f, (float)piViewport[2]/(float)piViewport[3], 0.05f, 20.0f);
			glGetDoublev(GL_PROJECTION_MATRIX, tProjectionMatrix);
		
			_ReshapeFunc(w, h);
		}

	#else	// MOD-BY-LEETEN 08/11/2008-TO:

	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport(tx, ty, tw, th);
	if( tw & th )
	{
		glGetIntegerv(GL_VIEWPORT, piViewport);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0f, (float)piViewport[2]/(float)piViewport[3], 0.05f, 20.0f);
		glGetDoublev(GL_PROJECTION_MATRIX, tProjectionMatrix);
	
		_ReshapeFunc(tw, th);
	}

	#endif	// MOD-BY-LEETEN 08/11/2008-END

	CHECK_OPENGL_ERROR(szReshape, true);
}

void 
CGlutWin::_KeyboardCB(unsigned char key, int x, int y)
{
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

		/*
		case 27:
			exit(0);
			break;
		*/

		default:
			_KeyboardFunc(key, x, y);
			;
	}
}

void 
CGlutWin::_SpecialCB(int skey, int x, int y)
{
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
	// flip the y coordinate
	y = piViewport[3] - y;

	iCursorX = x;
	iCursorY = y;
}

void 
CGlutWin::_MouseCB(int button, int state, int x, int y)
{
	// flip the y coordinate
	y = piViewport[3] - y; 

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

	_InitFunc();

	CHECK_OPENGL_ERROR(szInit, true);

	return iId;
}

// ADD-BY-LEETEN 08/09/2008-BEGIN
void 
CGlutWin::_Set()
{
	#if 0	// MOD-BY-LEETEN 08/12/2008-FROM:
		assert(iId > 0);
		glutSetWindow(iId);
	#else	// MOD-BY-LEETEN 08/12/2008-FROM:
	if( iId > 0 )
	{
		glutSetWindow(iId);
	}
	else
	{
		fprintf(stderr, "Warning in CGlutWin::_Set(): the window has ben not created yet.\n");
	}
	#endif	// MOD-BY-LEETEN 08/12/2008-FROM:
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
// The reason why it 

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

// ADD-BY-LEETEN 08/12/2008-BEGIN
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


// ADD-BY-LEETEN 08/12/2008-BEGIN

/*

$Log: not supported by cvs2svn $
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
