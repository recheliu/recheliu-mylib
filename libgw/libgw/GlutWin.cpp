	#include <typeinfo>

	#include <GL/glew.h>

	#ifdef WIN32	
	#include <GL/wglew.h>

	#if	defined(WITH_STATIC_LINK)
		#pragma comment (lib, "glew32s.lib")      
	#else	// #if	defined(WITH_STATIC_LINK)
	#pragma comment (lib, "glew32.lib")      
	#endif	// #if	defined(WITH_STATIC_LINK)	
	#endif

	#include <time.h>

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

		_AlignRotate(pdAxis);

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

			// draw a string in the origin of current 3D coordinate
void 
CGlutWin::_DrawString3D(char *szString, float fX, float fY, float fZ)		
{
_Begin();
	glPushMatrix();
							// normalize the coordinate to be window coordinate


		glTranslatef(fX, fY, fZ);

		glRasterPos3f(0.0, 0.0, 0.0);
		for(const char *pc = szString; *pc; pc++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *pc);

	glPopMatrix();
_End();
}

				// draw a string on the screen. It will be drawn in the origin in current coordinate
void
CGlutWin::_DrawString(
	char *szString, int iX, int iY, bool bAlignToRight)
{
_Begin();
	glPushMatrix();
							// normalize the coordinate to be window coordinate
	glLoadIdentity();
	glTranslatef(-1.0f, -1.0f, -1.0f);
	glScalef(2.0f/(float)piViewport[2], 2.0f/(float)piViewport[3], 2.0f);


	float fX, fY;
							// calculate the x coordinate
	fX = (float)(( iX >= 0 ) ? iX : (piViewport[2] + iX));

	if( bAlignToRight )
	{
		int iLen = (int)strlen(szString);
		fX -= iLen * 9;					// 9 is the width of the font
	}

							// calculate the y coordinate
	fY = (float)(( iY >= 0 ) ? iY : (piViewport[3] + iY));

	glTranslatef(fX, fY, 0.0f);

		glRasterPos3f(0.0, 0.0, 0.0);
		for(const char *pc = szString; *pc; pc++)
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *pc);

	glPopMatrix();
_End();
}

				// print out a string on the console with a prefix to indicate this window 
				// a newline will be printed at the end.
void			
CGlutWin::_AddToLog(char *szString, FILE* fpOutput)
{
	fprintf(fpOutput, "[GLUTWIN-%s]: %s\n", szTitle, szString);
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
		_DrawString(SZSprintf("FPS = %.2f", fFps));
	}

	if( iGluiEnum | GLUI_WIN || iGluiEnum | GLUI_SUBWIN  )
		GLUI_Master.sync_live_all();

	if( CGlutWin::bSwapBuffer )
		glutSwapBuffers();	

	CHECK_OPENGL_ERROR(szDisplay, true);
}

void 
CGlutWin::_ReshapeCB(int w, int h)
{
	iGlutWindowWidth = w;
	iGlutWindowHeight = h;

	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport(tx, ty, tw, th);
	glGetIntegerv(GL_VIEWPORT, piViewport);

	if( tw && th )
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(cViewFrustrum.fAngle_degree, (float)piViewport[2]/(float)piViewport[3], cViewFrustrum.fNear, cViewFrustrum.fFar);
		glGetDoublev(GL_PROJECTION_MATRIX, tProjectionMatrix);
	
		_ReshapeFunc(tw, th);
	}

	CHECK_OPENGL_ERROR(szReshape, true);
}

void 
CGlutWin::_UpdateWinCoord(int *piX, int *piY, bool bFlipY)
{
	*piX -= piViewport[0];
	if( bFlipY )
		*piY = iGlutWindowHeight - *piY;
	*piY -= piViewport[1];
}

void 
CGlutWin::_KeyboardCB(unsigned char key, int x, int y)
{
	_UpdateWinCoord(&x, &y);

	switch(key) {
		case 's':
			_SaveSnapshot();
			break;

		case 'm':		// load matrix
			_OpenMatrix(szMatrixFilename);
			break;

		case 'M':		// save matrix
			_SaveMatrix(szMatrixFilename);
			break;

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

		case 'F': case 'f':
			{
				static int piPrevViewport[4]; // the viewport beforethe 
				static bool bIsFullScreened = false;
				bIsFullScreened = !bIsFullScreened;
				if(bIsFullScreened)
				{
					piPrevViewport[2] = glutGet(GLUT_WINDOW_WIDTH);
					piPrevViewport[3] = glutGet(GLUT_WINDOW_HEIGHT);
					piPrevViewport[0] = glutGet(GLUT_WINDOW_X);
					piPrevViewport[1] = glutGet(GLUT_WINDOW_Y);
					glutFullScreen();
				}
				else
				{
					this->_Reshape(piPrevViewport[2], piPrevViewport[3]);
					this->_Set();
					glutPositionWindow(piPrevViewport[0], piPrevViewport[1]);
				}
			}
			break;

		case '0':
			// reset the model matrix
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);
			glutPostRedisplay();
			break;

		case '1':	case '2':	case '3':
		case '4':				case '6':
		case '7':	case '8':	case '9':
			{
				double pdAxis[3]; 
				double dAngle;
				switch(key){
				case '1': case '3': case '7': case '9':
					pdAxis[0] = 0.0;	pdAxis[1] = 0.0;	pdAxis[2] = 1.0;
					break;

				case '4': case '6':
					pdAxis[0] = 0.0;	pdAxis[1] = 1.0;	pdAxis[2] = 0.0;
					break;

				case '2': case '8':
					pdAxis[0] = 1.0;	pdAxis[1] = 0.0;	pdAxis[2] = 0.0;
					break;
				}
				switch(key){
				case '1':	dAngle = +5.0;		break;
				case '7':	dAngle = +45.0;		break;
				case '3':	dAngle = -5.0;		break;
				case '9':	dAngle = -45.0;		break;
				case '4':	dAngle = -45.0;		break;
				case '6':	dAngle = +45.0;		break;
				case '2':	dAngle = +45.0;		break;
				case '8':	dAngle = -45.0;		break;
				}

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				glTranslated(tModelMatrix[12], tModelMatrix[13], tModelMatrix[14]);
				glRotated( dAngle, pdAxis[0], pdAxis[1], pdAxis[2]);
				TMatrix tNormalMatrix;
				memcpy(tNormalMatrix, tModelMatrix, sizeof(tNormalMatrix));
				memset(&tNormalMatrix[12], 0, 3 * sizeof(tNormalMatrix[12]));
				glMultMatrixd(tNormalMatrix);
				glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);
			}
			glutPostRedisplay();
			break;
	}

	_KeyboardFunc(key, x, y);
}

void 
CGlutWin::_SpecialCB(int skey, int x, int y)
{
	_UpdateWinCoord(&x, &y);

	switch(skey) {
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT:
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN:
			{
			double pdAxis[3]; 
			double dAngle;
			switch(skey){
			case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
				pdAxis[0] = 0.0;	pdAxis[1] = 1.0;	pdAxis[2] = 0.0;
				break;

			case GLUT_KEY_DOWN: case GLUT_KEY_UP:
				pdAxis[0] = 1.0;	pdAxis[1] = 0.0;	pdAxis[2] = 0.0;
				break;
			}
			switch(skey){
			case GLUT_KEY_LEFT:		dAngle = -5.0;		break;
			case GLUT_KEY_RIGHT:	dAngle = +5.0;		break;
			case GLUT_KEY_UP:		dAngle = -5.0;		break;	
			case GLUT_KEY_DOWN:		dAngle = +5.0;		break;
			}

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glTranslated(tModelMatrix[12], tModelMatrix[13], tModelMatrix[14]);
			glRotated( dAngle, pdAxis[0], pdAxis[1], pdAxis[2]);
			TMatrix tNormalMatrix;
			memcpy(tNormalMatrix, tModelMatrix, sizeof(tNormalMatrix));
			memset(&tNormalMatrix[12], 0, 3 * sizeof(tNormalMatrix[12]));
			glMultMatrixd(tNormalMatrix);
			glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);
			}
			glutPostRedisplay();
			break;

		default:
			_SpecialFunc(skey, x, y);
	}
}

void 
CGlutWin::_PassiveMotionCB(int x, int y)
{
	_UpdateWinCoord(&x, &y);

	iPassiveCursorX = x;
	iPassiveCursorY = y;

	_PassiveMotionFunc(x, y);
}

void 
CGlutWin::_MotionCB(int x, int y)
{
	_UpdateWinCoord(&x, &y);

	iCursorX = x;
	iCursorY = y;

	_MotionFunc(x, y);
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

	_MouseFunc(button, state, x, y);
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

	if( bKeepUpdate )
		_Redisplay();
}

void 
CGlutWin::_MotionFunc(int x, int y)
{
}

void 
CGlutWin::_MouseFunc(int button, int state, int x, int y)
{
}

void 
CGlutWin::_PassiveMotionFunc(int x, int y)
{
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

	iGluiEnum = GLUI_NONE;	// by default there is no GLUI control
	pcGluiWin = NULL;		// by default there is no GLUI window
	pcGluiSubwin = NULL;	// by default there is no GLUI sub-window
	iSubwinPosistion = 0;	

	bDisplayFps = false;	// by default the FPS is not shown
	bKeepUpdate = false;	// by default the frame is not keep updating

	szMatrixFilename[0] = '\0';

	#if				WITH_OPENCV	
	pcSnapshot = NULL;
	#endif	// #if	WITH_OPENCV	

	iSnapshotIndex = 0;
}

// destructor
CGlutWin::~CGlutWin()
{
	#if			WITH_OPENCV	
	if( pcSnapshot )
		cvReleaseImage(&pcSnapshot);
	#endif	// #if	WITH_OPENCV	
}

void 
CGlutWin::_SaveMatrix(char *szMatrixFilename)
{
	FILE *fpMatrix;
	fpMatrix = fopen(szMatrixFilename, "wt");
	if( !fpMatrix )
	{
		_AddToLog("Error in CGlutWin::_KeyboardCB(): storing matrix failed.");
		return;
	}
	for(int i = 0,	r = 0; r < 4; r++)
	{
		for(int		c = 0; c < 4; c++, i++)
			fprintf(fpMatrix, "%f ", tModelMatrix[i]);
		fprintf(fpMatrix, "\n");
	}
	for(int i = 0,	r = 0; r < 4; r++)
	{
		for(int		c = 0; c < 4; c++, i++)
			fprintf(fpMatrix, "%f ", tViewMatrix[i]);
		fprintf(fpMatrix, "\n");
	}
	fclose(fpMatrix);
}

void 
CGlutWin::_OpenMatrix(char *szMatrixFilename)
{
	if( !iId )
	{
		_AddToLog("Wanring in CGlutWin::_SetMatrix(): This window hasn't been created yet.");
		return;
	}

	FILE *fpMatrix;
	fpMatrix = fopen(szMatrixFilename, "rt") ;

	if( !fpMatrix )
	{
		_AddToLog(SZSprintf("Error in CGlutWin::_OpenMatrix(): the matrix file %s cannot be opened.", szMatrixFilename));
		return;
	}

	for(int i = 0,	r = 0; r < 4; r++)
		for(int		c = 0; c < 4; c++, i++)
		{
			float fTemp;
			fscanf(fpMatrix, "%f", &fTemp);
			tModelMatrix[i] = (double)fTemp;
		}

	for(int i = 0,	r = 0; r < 4; r++)
		for(int		c = 0; c < 4; c++, i++)
		{
			float fTemp;
			fscanf(fpMatrix, "%f", &fTemp);
			tViewMatrix[i] = (double)fTemp;
		}

	fclose(fpMatrix);

}

void 
CGlutWin::_LoadSavedMatrix(char *szMatrixFilename)
{
	if( szMatrixFilename )
		strcpy(this->szMatrixFilename, szMatrixFilename);
	_OpenMatrix(this->szMatrixFilename);
}

int 
CGlutWin::IGetId()
{
	return iId;
}


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

	sprintf(szMatrixFilename, "%s.matrix.txt", szTitle);
	for(char *cp = NULL; NULL != (cp = strchr(szMatrixFilename, ' ')); *cp = '-')
		;

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

	CHECK_OPENGL_ERROR(szInit, true);

	return iId;
}

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

void 
CGlutWin::_Redisplay()
{
_Begin();
	glutPostRedisplay();
_End();
}

void
CGlutWin::_TimerFunc(unsigned short value)
{
}

						// The member method to handle the callback
						// It is called by CGlutWin_static:_TimerCB.
void 
CGlutWin::_TimerCB(unsigned short value)
{
	// call the timer func
	_TimerFunc(value);
}

void 
CGlutWin::_AddTimer(unsigned int msecs, unsigned short value)
{
						// call the static method to create a timer event for this window
						// the static method will implicitly combine this window's id and the value

	CGlutWin::_AddTimer(this, msecs, value);
}

void
CGlutWin::_GluiFunc(unsigned short usValue)
{
}

void 
CGlutWin::_GluiCB(unsigned short usValue)
{
	_GluiFunc(usValue);
}

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

void 
CGlutWin::_SaveSnapshot(char *szSnapshotFilename)
{
	#if			WITH_OPENCV	
_Begin();
	if( !piViewport[2] || !piViewport[3] )
	{
		_AddToLog("Warning: the size of the viewport is zero.");
		return;
	}

								// check whether the snapshot buffer (type: IplImage) has been allocated
	if( pcSnapshot	&& 
		(pcSnapshot->width != piViewport[2] && pcSnapshot->height != piViewport[3]) )
	{
		cvReleaseImage(&pcSnapshot);
		pcSnapshot = NULL;
	}

	if( !pcSnapshot	)
	{
		pcSnapshot	 = cvCreateImage(cvSize(piViewport[2], piViewport[3]), IPL_DEPTH_8U, 3);
	}

								// check the filename 
	char szTempSnapshotFilename[1024+1];
	if( !szSnapshotFilename )
	{
		sprintf(szTempSnapshotFilename, "%s_snapshot.%d.png", typeid(*this).name(), iSnapshotIndex++);
		szSnapshotFilename  = szTempSnapshotFilename;
	}

	for(char *pc = strchr(szTempSnapshotFilename, ' '); pc; pc = strchr(pc, ' '))
		*pc = '_';

	for(char *pc = strchr(szTempSnapshotFilename, ':'); pc; pc = strchr(pc, ':'))
		*pc = '_';

	glReadPixels(piViewport[0], piViewport[1], piViewport[2], piViewport[3], GL_BGR, GL_UNSIGNED_BYTE, pcSnapshot->imageData);

	cvFlip(pcSnapshot);
	cvSaveImage(szSnapshotFilename, pcSnapshot);

	_AddToLog(SZSprintf("Snapshot %s was saved\n", szSnapshotFilename));

_End();
	#else
_Begin();
	if( !piViewport[2] || !piViewport[3] )
	{
		_AddToLog("Warning: the size of the viewport is zero.");
		return;
	}
								// check the filename 
	char szTempSnapshotFilename[1024+1];
	if( !szSnapshotFilename )
	{
		sprintf(szTempSnapshotFilename, "%s_snapshot.%d.png", typeid(*this).name(), iSnapshotIndex++);
		szSnapshotFilename  = szTempSnapshotFilename;
	}
	for(char *pc = strchr(szTempSnapshotFilename, ' '); pc; pc = strchr(pc, ' '))
		*pc = '_';

	for(char *pc = strchr(szTempSnapshotFilename, ':'); pc; pc = strchr(pc, ':'))
		*pc = '_';

	std::vector<unsigned char> vubSnapshot;
	vubSnapshot.resize(piViewport[2] * piViewport[3] * 4);
	glPushAttrib(
		GL_COLOR_BUFFER_BIT |
		0);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPopAttrib();
		// GL_COLOR_BUFFER_BIT |
	glReadPixels(piViewport[0], piViewport[1], piViewport[2], piViewport[3], GL_RGBA, GL_UNSIGNED_BYTE, &vubSnapshot.front());

	std::vector<unsigned char> vubFlipped;
	vubFlipped.resize(piViewport[2] * piViewport[3] * 4);
	for(size_t y = 0; y < piViewport[3]; y++)
		copy(
			vubSnapshot.begin() + piViewport[2] * 4 * y,
			vubSnapshot.begin() + piViewport[2] * 4 * (y + 1),
			vubFlipped.begin() + piViewport[2] * 4 * (piViewport[3] - 1 - y)
		);

	lodepng::encode(szSnapshotFilename, vubFlipped, piViewport[2], piViewport[3]);
	_AddToLog(SZSprintf("Snapshot %s was saved\n", szSnapshotFilename));
_End();
	#endif	// #if	WITH_OPENCV	
}

void 
CGlutWin::_DisableVerticalSync()
{
	if( GLEW_OK != glewInit() )
		fprintf(stderr, "Warning: GLEW cannot be initialized.\n");
	else
#ifdef WIN32
	if 	( !WGL_EXT_swap_control )
		fprintf(stderr, "Warning: WGL_EXT_swap_control is not supported.\n");
	else
		wglSwapIntervalEXT(0);
#else
	;
#endif
}

