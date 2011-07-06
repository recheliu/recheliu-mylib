/*

GCB (Glut CallBacks): this is a template for developing GLUT-vbased OpenGL programs. 
My goal is to use this file as a template to speed up the development of OpenGL/GLUT 
programs

*/

#include <string.h>

#include "gcb.h"
#include "libopengl.h"

#ifndef fopen_s
#define fopen_s(pfp, path, flag) *(pfp) = fopen((path), (flag))
#endif

// ADD-BY-LEETEN 03/12/2008-BEGIN
#include "libppm.h"

static CImage cSnapshotBuffer;
static bool bSnapshot;
static char *_szSnapshotFilename;

static bool _bSnapshotThenExit;
// ADD-BY-LEETEN 03/12/2008-END

// ADD-BY-LEETEN 03/30/2008-BEGIN
bool	bAnime;
size_t	uNrOfAnimeFrames;
size_t	uAnimeFrame;
// ADD-BY-LEETEN 03/30/2008-END

///////////////////////////////////////////////////////////////
// 
TMatrix tViewMatrix, tInitViewMatrix;
TMatrix tModelMatrix, tInitModelMatrix;
TMatrix tProjectionMatrix;

int piViewport[4];

// ADD-BY-LEETEN 03/31/2008-BEGIN
// the different between the origin of the model and the cursor the on screen.
// it is used to adjust the shift of the model
double dXDiffOnScreen, dYDiffOnScreen;	

// a flag to indicate if the matrix should be dumped
bool bAnimeRecordMatrix;

// a flag to indicate if the matrix should be readed
bool bAnimePlayMatrix;
bool bAnimeSaveFrames;

static FILE *fpMatrix;
void (*QuitFunc)();
// ADD-BY-LEETEN 03/31/2008-END

///////////////////////////////////////////////////////////////
// GLUT Callbacks 
void (*_DisplayFunc)();
void (*_ReshapeFunc)(int w, int h);
void (*_KeyboardFunc)(unsigned char key, int w, int h);
void (*_SpecialFunc)(int skey, int w, int h);
void (*_IdleFunc)();
// ADD-BY-LEETEN 07/14/2008-BEGIN
void (*_AnimeFunc)();
// ADD-BY-LEETEN 07/14/2008-END

///////////////////////////////////////////////////////////////
// The callbacks, functions and vairable for the mouse interfaces.
// The reference point of every kind of motion is the point where the mouse is just clicked.
static int iBeginX, iBeginY; // the 2D coordinate of the cursor
static int iCursorX, iCursorY; // the 2D coordinate of the cursor
static GLenum eMouseButton = 0;
static GLenum eModifier = 0;
static bool bMoving = false;

// ADD-BY-LEETEN 03/31/2008-BEGIN
void
gcbMatrixRecord(char _szMatrixFilename[])
{
	bAnimeRecordMatrix = true;
	fopen_s(&fpMatrix, _szMatrixFilename, "wb");
	assert( fpMatrix );

	fprintf(stderr, "The matrices will be recorded to file %s\n", _szMatrixFilename);
}

void
gcbMatrixPlay(char _szMatrixFilename[])
{
	bAnimePlayMatrix = true;
	fopen_s(&fpMatrix, _szMatrixFilename, "rb");
	assert( fpMatrix );

	fprintf(stderr, "The matrices will be loaded from file %s\n", _szMatrixFilename);
}

void
_Quit()
{
	if( fpMatrix ) 
		fclose(fpMatrix);

	if( QuitFunc )
		QuitFunc();
}

void
_AlignPan(double pdNewCoord[3], double pdOldCoord[3])
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
_AlignRotate(double pdAxis[3])
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

// ADD-BY-LEETEN 03/31/2008-END

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

static double pdOldCoord[3], pdNewCoord[3];
static double pdCurPos[3], pdBeginPos[3];
static double dX, dY, dZ;

void
_RotateCamera()
{

	// trackball_ptov(iBeginX, iBeginY, piViewport[2], piViewport[3], pdBeginPos);
	// trackball_ptov(iCursorX, iCursorY, piViewport[2], piViewport[3], pdCurPos);
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

void _PanCamera()
{
	gluUnProject(
		(double)iBeginX, (double)iBeginY, 0.0, 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdOldCoord[0], &pdOldCoord[1], &pdOldCoord[2]);

	gluUnProject(
		(double)iCursorX, (double)iCursorY, 0.0, 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdNewCoord[0], &pdNewCoord[1], &pdNewCoord[2]);

	// ADD-BY-LEETEN 03/31/2008-BEGIN
	_AlignPan(pdNewCoord, pdOldCoord);
	// ADD-BY-LEETEN 03/31/2008-END

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(tViewMatrix);
	glTranslated(pdOldCoord[0] - pdNewCoord[0], (pdOldCoord[1] - pdNewCoord[1]), pdOldCoord[2] - pdNewCoord[2]);
	glGetDoublev(GL_MODELVIEW_MATRIX, tViewMatrix);
	glutPostRedisplay();
}

void 
_ZoomCamera()
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
_PanModel()
{
	gluProject(
		tModelMatrix[12], tModelMatrix[13], tModelMatrix[14], 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdOldCoord[0], &pdOldCoord[1], &pdOldCoord[2]);

	gluUnProject(
		// MOD-BY-LEETEN 03/31/2008-BEGIN
		// FROM: (double)iCursorX, (double)iCursorY, pdOldCoord[2],
		// TO:
		(double)iCursorX + dXDiffOnScreen, (double)iCursorY + dYDiffOnScreen, pdOldCoord[2],
		// MOD-BY-LEETEN 03/31/2008-END
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdNewCoord[0], &pdNewCoord[1], &pdNewCoord[2]);

	for(int i=0; i<3; i++)
		tModelMatrix[12 + i] = pdNewCoord[i];

	glutPostRedisplay();
}

void
_RotateModel()
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
_ZoomModel()
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

/////////////////////////////////////////////////////////////////////////////////
void
_DisplayCB()
{
	// DEL-BY-LEETEN 07/30/2008-FROM:
	// fprintf(stderr, "Frame %d\n", uAnimeFrame);
	// DEL-BY-LEETEN 07/30/2008-END

	// ADD-BY-LEETEN 03/31/2008-BEGIN
	if( bAnimePlayMatrix )
	{
		if( !feof(fpMatrix) )
		{
			fread(tViewMatrix, sizeof(double), 16, fpMatrix);
			fread(tModelMatrix, sizeof(double), 16, fpMatrix);
		}
	}
	// ADD-BY-LEETEN 03/31/2008-END

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixd(tProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(tViewMatrix);

	glMultMatrixd(tModelMatrix);

	if( _DisplayFunc )
		_DisplayFunc();

	// ADD-BY-LEETEN 03/12/2008-BEGIN
	// MOD-BY-LEETEN 03/30/2008-BEGIN
	// FROM: if( _bSnapshotThenExit || bSnapshot )
	// TO: 
	if( _bSnapshotThenExit || bSnapshot || bAnimeSaveFrames )
	// MOD-BY-LEETEN 03/30/2008-END
	{
		glReadPixels(0, 0, 
			cSnapshotBuffer.uWidth, cSnapshotBuffer.uHeight, 
			GL_RGBA, GL_FLOAT, cSnapshotBuffer.pfBuffer);

		// ADD-BY-LEETEN 03/30/2008-BEGIN
		if( bAnimeSaveFrames )
		{
			static char szAnimeFilename[1024+1];
			strcpy(szAnimeFilename, _szSnapshotFilename);
			if( strrchr(szAnimeFilename, '.') )
			{
				*strrchr(szAnimeFilename, '.') = '\0';
			}
			sprintf(&szAnimeFilename[strlen(szAnimeFilename)], "_%04d.ppm", uAnimeFrame);

			cSnapshotBuffer.BWrite( szAnimeFilename );
			fprintf(stdout, "Snapshot %s was saved\n", szAnimeFilename);
		}
		else
		{
		// ADD-BY-LEETEN 03/30/2008-END
		cSnapshotBuffer.BWrite( _szSnapshotFilename );
		bSnapshot = false;
		fprintf(stdout, "Snapshot %s was saved\n", _szSnapshotFilename);
		// ADD-BY-LEETEN 03/30/2008-BEGIN
		}
		// ADD-BY-LEETEN 03/30/2008-END
	}

	if( _bSnapshotThenExit )
		exit(0);
	// ADD-BY-LEETEN 03/12/2008-END

	// ADD-BY-LEETEN 03/31/2008-END
	uAnimeFrame++;
	// ADD-BY-LEETEN 05/01/2008-BEGIN
	if( bAnimeSaveFrames )
	{
	// ADD-BY-LEETEN 05/01/2008-END

	if( uAnimeFrame > uNrOfAnimeFrames )
		exit(0);

	// ADD-BY-LEETEN 05/01/2008-BEGIN
	}
	// ADD-BY-LEETEN 05/01/2008-END

	if( bAnimeRecordMatrix )
	{
		fwrite(tViewMatrix, sizeof(double), 16, fpMatrix);
		fwrite(tModelMatrix, sizeof(double), 16, fpMatrix);
	}
	// ADD-BY-LEETEN 03/31/2008-END

	CHECK_OPENGL_ERROR("_DisplayCB()", true);
}

void
_ReshapeCB(int w, int h)
{
	if( w & h )
	{
		// ADD-BY-LEETEN 03/12/2008-BEGIN
		cSnapshotBuffer.uWidth = w;
		cSnapshotBuffer.uHeight = h;
		assert( cSnapshotBuffer.BAlloc() );
		// ADD-BY-LEETEN 03/12/2008-END

		glViewport(0, 0, w, h);

		glGetIntegerv(GL_VIEWPORT, piViewport);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// MOd-BY-LEETEN 03/05/2008-BEGIN
		// FROM: gluPerspective(30.0f, (float)piViewport[2]/(float)piViewport[3], 1.0f, 20.0f);
		gluPerspective(30.0f, (float)piViewport[2]/(float)piViewport[3], 0.05f, 20.0f);
		// MOd-BY-LEETEN 03/05/2008-END
		glGetDoublev(GL_PROJECTION_MATRIX, tProjectionMatrix);
	
		if( _ReshapeFunc )
			_ReshapeFunc(w, h);
	}


	CHECK_OPENGL_ERROR("_ReshapeCB()", true);
}

void 
_KeyCB(unsigned char key, int x, int y)
{
	switch(key) {
		// ADD-BY-LEETEN 03/24/2008-BEGIN
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
		// ADD-BY-LEETEN 03/24/2008-END

		// ADD-BY-LEETEN 03/12/2008-BEGIN
		case 's': case 'S':
			bSnapshot = true;
			glutPostRedisplay();
			break;
		// ADD-BY-LEETEN 03/12/2008-END

		case 'r': case 'R':
			memcpy(tViewMatrix, tInitViewMatrix, sizeof(tViewMatrix));;
			memcpy(tModelMatrix, tInitModelMatrix, sizeof(tModelMatrix));;

			glutPostRedisplay();
			break;

		// ADD-BY-LEETEN 04/02/2008-BEGIN
		case 'N': case 'n':
			glutPostRedisplay();
			break;
		// ADD-BY-LEETEN 04/02/2008-END

		case 27:
			exit(0);
			break;

		default:
			if( _KeyboardFunc )
				_KeyboardFunc(key, x, y);
			;
	}
}

void 
_SpecialCB(int skey, int x, int y)
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
			if( _SpecialFunc )
				_SpecialFunc(skey, x, y);
	}
}

void
_MotionCB(int x, int y)
{
	// flip the y coordinate
	y = piViewport[3] - y;

	iCursorX = x;
	iCursorY = y;
}

void
_MouseCB(int button, int state, int x, int y)
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

	// ADD-BY-LEETEN 03/31/2008-BEGIN
	double pdCurrentModelCenterOnScreen[3];
	gluProject(
		tModelMatrix[12], tModelMatrix[13], tModelMatrix[14], 
		tViewMatrix, tProjectionMatrix, piViewport, 
		&pdCurrentModelCenterOnScreen[0], &pdCurrentModelCenterOnScreen[1], &pdCurrentModelCenterOnScreen[2]);

	dXDiffOnScreen = pdCurrentModelCenterOnScreen[0] - (double)iCursorX;
	dYDiffOnScreen = pdCurrentModelCenterOnScreen[1] - (double)iCursorY;
	// ADD-BY-LEETEN 03/31/2008-END

	
}

void
_IdleCB()
{
	// ADD-BY-LEETEN 03/31/2008-BEGIN
	// ADD-BY-LEETEN 03/31/2008-END

	if(bMoving)
	{
		switch(eMouseButton) 
		{
		case GLUT_LEFT_BUTTON: // pan
			// MOD-BY-LEETEN 03/31/2008-BEGIN
			// FROM: switch(eModifier)
			// TO:
			switch( eModifier & ~GLUT_ACTIVE_ALT )
			// MOD-BY-LEETEN 03/31/2008-END
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
			// MOD-BY-LEETEN 03/31/2008-BEGIN
			// FROM: switch(eModifier)
			// TO:
			switch( eModifier & ~GLUT_ACTIVE_ALT )
			// MOD-BY-LEETEN 03/31/2008-END
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

	if( _IdleFunc )
		_IdleFunc();
}

// ADD-BY-LEETEN 04/02/2008-BEGIN
void
_TimerCB(int value)
{
	// ADD-BY-LEETEN 07/14/2008-BEGIN
	_AnimeFunc();
	// ADD-BY-LEETEN 07/14/2008-END

	glutPostRedisplay();

	// ADD-BY-LEETEN 07/25/2008-BEGIN
	if( bAnime )
	// ADD-BY-LEETEN 07/25/2008-END
	glutTimerFunc(33, _TimerCB, 0);
}
// ADD-BY-LEETEN 04/02/2008-END

///////////////////////////////////////////////////////////////
// basic subroutines

// ADD-BY-LEETEN 07/25/2008-BEGIN
void 
gcbAnimePause()
{
	bAnime = false;
}

void 
gcbAnimeResume()
{
	gcbAnimeOn();
}
// ADD-BY-LEETEN 07/25/2008-END

// ADD-BY-LEETEN 03/30/2008-BEGIN
void
gcbAnimeOn()
{
	bAnime = true;
	glutTimerFunc(33, _TimerCB, 0);
}

void 
gcbAnimeNrOfFrames(size_t _uNrOfFrames)
{
	if( _uNrOfFrames > 0 )
	{
		uNrOfAnimeFrames = _uNrOfFrames;
		uAnimeFrame = 0;
	}
}

void 
gcbAnimeSaveFrames()
{
	bAnimeSaveFrames = true;
}

// ADD-BY-LEETEN 03/30/2008-END

// ADD-BY-LEETEN 03/12/2008-BEGIN
void 
gcbSetSnapshotFilename(char* szSnapshotFilename, bool bSnapshotThenExit)
{
	_szSnapshotFilename = szSnapshotFilename;
	_bSnapshotThenExit = bSnapshotThenExit;
}

// ADD-BY-LEETEN 03/12/2008-END

void
gcbDisplayFunc(void (*_MyDisplayFunc)())
{
	_DisplayFunc = _MyDisplayFunc;
}

void
gcbReshapeFunc(void (*_MyReshapeFunc)(int, int))
{
	_ReshapeFunc = _MyReshapeFunc;
}

void
gcbKeyboardFunc(void (*_MyKeyboardFunc)(unsigned char, int, int))
{
	_KeyboardFunc = _MyKeyboardFunc;
}

void
gcbSpecialFunc(void (*_MySpecialFunc)(int, int, int))
{
	_SpecialFunc = _MySpecialFunc;
}

void
gcbIdleFunc(void (*_MyIdleFunc)())
{
	_IdleFunc = _MyIdleFunc;
}

// ADD-BY-LEETEN 07/14/2008-BEGIN
void
gcbAnimeFunc(void (*_MyAnimeFunc)())
{
	_AnimeFunc = _MyAnimeFunc;
}
// ADD-BY-LEETEN 07/14/2008-END

void
gcbInit(void (*_InitFunc)(), void (*_QuitFunc)())
{
	// register GLUT callbacks
	glutDisplayFunc(_DisplayCB);
	glutReshapeFunc(_ReshapeCB);
	glutKeyboardFunc(_KeyCB);
	glutSpecialFunc(_SpecialCB);
	glutMotionFunc(_MotionCB);
	glutMouseFunc(_MouseCB);
	glutIdleFunc(_IdleCB);

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

	if( _InitFunc )
		_InitFunc();

	#if 0	// MOD-BY-LEETEN 03/31/2008-FROM:
	if( _QuitFunc )
		atexit(_QuitFunc );
	#else	// MOD-BY-LEETEN 03/31/2008-TO:
	QuitFunc = _QuitFunc;
	atexit(_Quit);
	#endif	// MOD-BY-LEETEN 03/31/2008-END

	CHECK_OPENGL_ERROR("gcbInit()", true);
}

/*

$Log: not supported by cvs2svn $
Revision 1.2  2008/07/25 22:02:48  leeten

[07/25/2008]
1. [ADD] Define new function gcbAnimePause()/gcbAnimeResume() to toggle the animation.
2. [ADD] Define a new function gcbAnimeFunc() to specify user-specified callback.

Revision 1.1.1.1  2008/06/12 22:43:35  leeten

[06/12/2008]
1. Frist time checkin.

Revision 1.7  2008/06/12 21:21:59  leeten

[06/12/2008]
1. Add code to prevent the GLUT application to stop itself if the animation flag is undefined.

Revision 1.6  2008/04/02 19:35:05  leeten

[04/02/2008]
1. When shift the object, instead of moving the origin of the  model coordinate, the place where the mous is first clicked is shifted.
2. Register the quit callback to _Quit other than user-specified call back.
3. When ALT key is pressed, the object will be roated about the xi, y- or z- axies or pan in parallel with the X- or Y- axis.
4. Change the angle to rotate from 0.01 to 0.1.
5. Add new hot key 'N' or trigger the function glutPostRedisplay().

Revision 1.5  2008/03/30 21:28:11  leeten

[03/30/2008]
1. Add function gcbEnableAnime to enable the animation mode. When the frames are captured, the program will be terminated automatically.

Revision 1.4  2008/03/29 02:58:31  leeten

[03/28/2008]
1. Add hotkey 'x,''y' and 'z' to change the view direction to +X, Y and Z directionm, respectively.
2. Add hotkey 'X,''Y' and 'Z' to change the view direction to -X, Y and Z directionm, respectively.

Revision 1.3  2008/03/14 22:56:19  leeten

[03/14/2008]
1. Add  function gcbSetSnapshotFilename() to specify the filename of the snapshot and if the program should be terminated afer the first snapshot is taken.
2. Add hotkey 'S' to take snashot.

Revision 1.2  2008/03/06 21:54:23  leeten

[03/06/2008]
1. Change the near distnace of the cemera.

Revision 1.1.1.1  2008/02/23 03:05:24  leeten

[02/22/2008]
1. First time checkin.


*/
