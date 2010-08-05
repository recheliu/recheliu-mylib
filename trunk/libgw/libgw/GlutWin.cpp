	// ADD-BY-LEETEN 06/01/2008-BEGIN
	#include <typeinfo>
	// ADD-BY-LEETEN 06/01/2008-END

	// ADD-BY-LEETEN 08/25/2008-BEGIN
	#include <GL/glew.h>

	// ADD-BY-LEETEN 11/17/2008-BEGIN

	// ADD-BY-LEETEN 06/01/2008-BEGIN
	#ifdef WIN32	
	// ADD-BY-LEETEN 06/01/2008-END
	#include <GL/wglew.h>
	#pragma comment (lib, "glew32.lib")      
	// ADD-BY-LEETEN 06/01/2008-BEGIN
	#endif
	// ADD-BY-LEETEN 06/01/2008-END
	// ADD-BY-LEETEN 11/17/2008-END

	#include <time.h>
	// ADD-BY-LEETEN 08/25/2008-END

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

// ADD-BY-LEETEN 2008/12/21-BEGIN
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
// ADD-BY-LEETEN 2008/12/21-END

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
	// MOD-BY-LEETEN 2009/01/09-FROM:
		// fX = (float)(( iX >= 0 ) ? iX : (piViewport[2] - iX));
	// TO:
	fX = (float)(( iX >= 0 ) ? iX : (piViewport[2] + iX));
	// MOD-BY-LEETEN 2009/01/09-END

	if( bAlignToRight )
	{
		int iLen = (int)strlen(szString);
		fX -= iLen * 9;					// 9 is the width of the font
	}

							// calculate the y coordinate
	// MOD-BY-LEETEN 2009/01/09-FROM:
		// fY = (float)(( iY >= 0 ) ? iY : (piViewport[3] - iY));
	// TO:
	fY = (float)(( iY >= 0 ) ? iY : (piViewport[3] + iY));
	// MOD-BY-LEETEN 2009/01/09-END

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
		// MOD-BY-LEETEN 2008/08/17-FROM:
			// _DrawString(SZSprintf("FPS = %f", fFps));
		// TO:
		_DrawString(SZSprintf("FPS = %.2f", fFps));
		// MOD-BY-LEETEN 2008/08/17-END
	}

	// ADD-BY-TLEE 2008/08/18-BEGIN
	if( iGluiEnum | GLUI_WIN || iGluiEnum | GLUI_SUBWIN  )
		GLUI_Master.sync_live_all();
	// ADD-BY-TLEE 2008/08/18-END

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

	// MOD-BY-LEETEN 08/18/2008-FROM:
		// if( tw & th )
	// TO:
	if( tw && th )
	// MOD-BY-LEETEN 08/18/2008-END
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// MOD-BY-TLEE 2008/08/20-FROM:
			// gluPerspective(30.0f, (float)piViewport[2]/(float)piViewport[3], 0.05f, 20.0f);
		// TO:
		gluPerspective(cViewFrustrum.fAngle_degree, (float)piViewport[2]/(float)piViewport[3], cViewFrustrum.fNear, cViewFrustrum.fFar);
		// MOD-BY-TLEE 2008/08/20-END
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
		// ADD-BY-LEETEN 2008/11/29-BEGIN
		case 's':
			_SaveSnapshot();
			break;
		// ADD-BY-LEETEN 2008/11/29-END

		// ADD-BY-LEETEN 08/20/2008-BEGIN
		case 'm':		// load matrix
			_OpenMatrix(szMatrixFilename);
			break;

		case 'M':		// save matrix
			_SaveMatrix(szMatrixFilename);
			break;
		// ADD-BY-LEETEN 08/20/2008-END

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

		// ADD-BY-LEETEN 08/05/2010-BEGIN
		case 'F': case 'f':
			{
				static int piPrevViewport[4]; // the viewport beforethe 
				static bool bIsFullScreened = false;
				bIsFullScreened = !bIsFullScreened;
				if(bIsFullScreened)
				{
					memcpy(piPrevViewport, this->piViewport, sizeof(piPrevViewport));
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
		// ADD-BY-LEETEN 08/05/2010-END

		// ADD-BY-LEETEN 01/04/2010-BEGIN
		case '0':
			// reset the model matrix
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);
			// ADD-BY-LEETEN 01/31/2010-BEGIN
			glutPostRedisplay();
			// ADD-BY-LEETEN 01/31/2010-END
			break;
		// ADD-BY-LEETEN 01/04/2010-END

		// ADD-BY-LEETEN 01/02/2010-BEGIN
		case '1':	case '2':	case '3':
		// MOD-BY-LEETEN 01/31/2010-FROM:
			// case '4':	case '5':	case '6':
		// TO:
		case '4':				case '6':
		// MOD-BY-LEETEN 01/31/2010-END
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
				#if	0	// MOD-BY-LEETEN 01/19/2010-FROM:
					case '1':	dAngle = +45.0;		break;
					case '7':	dAngle = +135.0;	break;
					case '3':	dAngle = -45.0;		break;
					case '9':	dAngle = -135.0;	break;
				#else	// MOD-BY-LEETEN 01/19/2010-TO:
				case '1':	dAngle = +5.0;		break;
				case '7':	dAngle = +45.0;		break;
				case '3':	dAngle = -5.0;		break;
				case '9':	dAngle = -45.0;		break;
				#endif	// MOD-BY-LEETEN 01/19/2010-END
				#if	0	// MOD-BY-LEETEN 01/04/2010-FROM:
					case '4':	dAngle = +45.0;		break;
					case '6':	dAngle = -45.0;		break;
					case '2':	dAngle = -45.0;		break;
					case '8':	dAngle = +45.0;		break;
				#else	// MOD-BY-LEETEN 01/04/2010-TO:
				case '4':	dAngle = -45.0;		break;
				case '6':	dAngle = +45.0;		break;
				case '2':	dAngle = +45.0;		break;
				case '8':	dAngle = -45.0;		break;
				#endif	// MOD-BY-LEETEN 01/04/2010-END
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
			// ADD-BY-LEETEN 01/31/2010-BEGIN
			glutPostRedisplay();
			// ADD-BY-LEETEN 01/31/2010-END
			break;
		// ADD-BY-LEETEN 01/02/2010-END

		#if	0	// DEL-BY-LEETEN 2009/11/04-BEGIN
			default:
				_KeyboardFunc(key, x, y);
				;
		#endif	// ADD-BY-LEETEN 2009/11/04-END
	}

	// ADD-BY-LEETEN 2009/11/04-BEGIN
	_KeyboardFunc(key, x, y);
	// ADD-BY-LEETEN 2009/11/04-END
}

void 
CGlutWin::_SpecialCB(int skey, int x, int y)
{
	// ADD-BY-LEETEN 08/13/2008-BEGIN
	_UpdateWinCoord(&x, &y);
	// ADD-BY-LEETEN 08/13/2008-END

	switch(skey) {
		#if	0	// MOD-BY-LEETEN 01/04/2010-FROM:
			case GLUT_KEY_LEFT:
				break;

			case GLUT_KEY_RIGHT:
				break;

			case GLUT_KEY_UP:
				break;

			case GLUT_KEY_DOWN:
				break;
		#else	// MOD-BY-LEETEN 01/04/2010-TO:
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
			// ADD-BY-LEETEN 01/31/2010-BEGIN
			glutPostRedisplay();
			// ADD-BY-LEETEN 01/31/2010-END
			break;
		#endif	// MOD-BY-LEETEN 01/04/2010-END

		default:
			_SpecialFunc(skey, x, y);
	}
}

// ADD-BY-LEETEN 2009/03/04-BEGIN
void 
CGlutWin::_PassiveMotionCB(int x, int y)
{
	_UpdateWinCoord(&x, &y);

	iPassiveCursorX = x;
	iPassiveCursorY = y;

	_PassiveMotionFunc(x, y);
}
// ADD-BY-LEETEN 2009/03/04-END

void 
CGlutWin::_MotionCB(int x, int y)
{
	_UpdateWinCoord(&x, &y);

	iCursorX = x;
	iCursorY = y;

	// ADD-BY-LEETEN 2009/01/22-BEGIN
	_MotionFunc(x, y);
	// ADD-BY-LEETEN 2009/01/22-END
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

	// ADD-BY-LEETEN 2009/01/22-BEGIN
	_MouseFunc(button, state, x, y);
	// ADD-BY-LEETEN 2009/01/22-END
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

// ADD-BY-LEETEN 2009/01/22-BEGIN
void 
CGlutWin::_MotionFunc(int x, int y)
{
}

void 
CGlutWin::_MouseFunc(int button, int state, int x, int y)
{
}
// ADD-BY-LEETEN 2009/01/22-END

// ADD-BY-LEETEN 2009/03/04-BEGIN
void 
CGlutWin::_PassiveMotionFunc(int x, int y)
{
}
// ADD-BY-LEETEN 2009/03/04-END

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

	// ADD-BY-TLEE 2008/08/20-BEGIN
	szMatrixFilename[0] = '\0';
	// ADD-BY-TLEE 2008/08/20-END

	// ADD-BY-LEETEN 08/25/2008-BEGIN
	pcSnapshot = NULL;
	// ADD-BY-LEETEN 08/25/2008-END

	// ADD-BY-LEETEN 12/05/2008-BEGIN
	iSnapshotIndex = 0;
	// ADD-BY-LEETEN 12/05/2008-END

}

// destructor
CGlutWin::~CGlutWin()
{
	// ADD-BY-LEETEN 08/25/2008-BEGIN
	if( pcSnapshot )
		cvReleaseImage(&pcSnapshot);
	// ADD-BY-LEETEN 08/25/2008-END
}

// ADD-BY-TLEE 2008/08/20-BEGIN
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
// ADD-BY-TLEE 2008/08/20-END

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

	// ADD-BY-TLEE 2008/08/20-BEGIN
	sprintf(szMatrixFilename, "%s.matrix.txt", szTitle);
	for(char *cp = NULL; NULL != (cp = strchr(szMatrixFilename, ' ')); *cp = '-')
		;
	// ADD-BY-TLEE 2008/08/20-END

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

// ADD-BY-LEETEN 08/25/2008-BEGIN
void 
CGlutWin::_SaveSnapshot(char *szSnapshotFilename)
{
	// ADD-BY-LEETEN 12/05/2008-BEGIN
_Begin();
	// ADD-BY-LEETEN 12/05/2008-END
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
		// MOD-BY-LEETEN 2008/11/29-FROM:
			// sprintf(szTempSnapshotFilename, "%s_snapshot_%d.png", typeid(*this).name(), time(NULL));
		// TO:
		// DEL-BY-LEETEN 12/05/2008-BEGIN
			// static int iSnapshotIndex = 0;
		// DEL-BY-LEETEN 12/05/2008-END
		sprintf(szTempSnapshotFilename, "%s_snapshot.%d.png", typeid(*this).name(), iSnapshotIndex++);
		// MOD-BY-LEETEN 2008/11/29-END
		#if	0	// DEL-BY-LEETEN 2009/01/09-BEGIN
			for(char *pc = strchr(szTempSnapshotFilename, ' '); pc; pc = strchr(pc, ' '))
				*pc = '_';
		#endif	// END
		szSnapshotFilename  = szTempSnapshotFilename;
	}
	#if	1	// ADD-BY-LEETEN 2009/01/09-BEGIN
	for(char *pc = strchr(szTempSnapshotFilename, ' '); pc; pc = strchr(pc, ' '))
		*pc = '_';

	for(char *pc = strchr(szTempSnapshotFilename, ':'); pc; pc = strchr(pc, ':'))
		*pc = '_';
	#endif	// ADD-BY-LEETEN 2009/01/09-END

	glReadPixels(piViewport[0], piViewport[1], piViewport[2], piViewport[3], GL_BGR, GL_UNSIGNED_BYTE, pcSnapshot->imageData);

	#if	1	// ADD-BY-LEETEN 2009/01/09-BEGIN
	cvFlip(pcSnapshot);
	#endif	// END
	cvSaveImage(szSnapshotFilename, pcSnapshot);

	// ADD-BY-LEETEN 2008/11/29-BEGIN
	_AddToLog(SZSprintf("Snapshot %s was saved\n", szSnapshotFilename));
	// ADD-BY-LEETEN 2008/11/29-END

// ADD-BY-LEETEN 12/05/2008-BEGIN
_End();
// ADD-BY-LEETEN 12/05/2008-END
}

// ADD-BY-LEETEN 08/25/2008-END

// ADD-BY-LEETEN 11/17/2008-BEGIN
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
// ADD-BY-LEETEN 11/17/2008-END



/*

$Log: not supported by cvs2svn $
Revision 1.25  2010/02/01 05:54:30  leeten

[01/30/2010]
1. [ADD] Add glutPostRedisplay() after the hot keys are pressed.
2. [DEL] Remove the undefined hotkey '5.'

Revision 1.24  2010/01/19 21:05:17  leeten

[01/19/2010]
1. [MOD] Change the rotation angle of hotkey '1' and '3'  from 45 degree to 5 degree.
2. [MOD] Change the rotation angle of hotkey '7' and '9'  from 135 degree to 45 degree.

Revision 1.23  2010/01/04 18:47:55  leeten

[01/04/2010]
1. [MOD] Switch the behavior of the hotkeys '2' and '8'.
2. [MOD] Switch the behavior of the hotkeys '4' and '6'.
3. [ADD] Define the behavior for the arrow keys UP, DOWN, LEFT and RIGHT.

Revision 1.22  2010/01/01 18:40:06  leeten

[01/01/2010]
1. [ADD] Define new hotkeys to control the orientation of the object.

Revision 1.21  2009/11/04 20:51:37  leeten

[2009/11/04]
1. [MOD] Call the _Keyboardfunc() after the switch-case statement.

Revision 1.20  2009/06/01 21:34:47  leeten

[2009/06/01]
1. [ADD] Include the header typeinfo.
2. [MOD] Only include wglew when the preprocessor WIN32 is defined.

Revision 1.19  2009/03/04 22:21:04  leeten

[2009/03/04]
1. [ADD] Support the handling of the passive mouse motion. The cursor coordinates will be stored in two variabel iPassiveCursorX and iPassiveCursorY. The subclass of CGlutWin can overload the callback _PassiveMotionFunc() (recommended) or _PassiveMotionCB().

Revision 1.18  2009/01/22 17:13:49  leeten

[2009/01/22]
1. [ADD] Define a new constant CB_MANUAL for user-defined events.
2. [ADD] Define new methods _MouseFunc and _MotionFunc to track the location of trhe cursor.

Revision 1.17  2009/01/10 05:04:54  leeten

[2009/01/09]
1. [DEBUG] Sovle the bug In the member method _AddString
2. [ADD] In the method _SaveSnapshot, flip the obtained framebuffer before saving to the specified file.

Revision 1.16  2008/12/21 21:59:29  leeten

[2008/12/21]
1. [ADD] Define a new method _DrawString3D() to draw a string in the local coordinate.

Revision 1.15  2008/12/06 21:36:59  leeten

[2008/12/05]
1. [ADD] When saving the snapshot, the filename is determined by the new-adeed member variabel iSnapshotIndex.

Revision 1.14  2008/11/30 04:18:10  leeten

[2008/11/29]
1. [ADD] Add a hotkey 's' to save the snapshot.
2. [ADD] Implicitly Include the glew32.lib.
3. [ADD] Change thje naming rule of the the snapshotot.
4. [ADD] Define a new method _DisableVerticalSync().

Revision 1.13  2008/08/25 20:39:23  leeten

[2008/08/25]
1. [ADD] Declare a new method _SaveSnapshot() to save current frame.

Revision 1.12  2008/08/21 14:44:19  leeten

[2008/08/21]
1. [ADD] Define methods to open/save the modelview matrixs
2. [ADD] Add hotkey 'm' to load the modelveiw matrix from the file and hotkey 'M' to store current modelview matrix to a file.

Revision 1.11  2008/08/20 19:33:58  leeten

[2008/08/20]
1. [CHANGE] Use three variables fAngle_degree, fNear and fFar to specify the view frustrum.

Revision 1.10  2008/08/18 21:28:31  leeten

[2008/08/18]
1. [CHANGE] Change the far distance of the view spectrum from 20..0f to 1000.0f.
2. [DEBUG] Change the condition check in _ReshapeCB from if( tw & th ) to if( tw && th ).
3. [ADD] Sync all GLUI live varables at the end of _DisplayCB().

Revision 1.9  2008/08/17 23:57:49  leeten

[2008/08/17]
1. [CHANGE] Redefined the method _DrawString such that the user can control the position of the string.
2. [CHANGE] Change the fraction digits to 2 when displaying FPS.

Revision 1.8  2008/08/16 21:20:25  leeten

[2008/08/16]
1. [CHANGE] Change the type of the value for timer event from int to unsigned short.

Revision 1.7  2008/08/16 15:58:44  leeten

[2008/08/16]
1. [DEL] Remove old deleted code segments.
2. [CHANGE] Add the initialization of GLUI win/subwin in and the invokation of _InitFunc in the method ICreate().

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
