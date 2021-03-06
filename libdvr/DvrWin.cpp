#include <GL/glew.h>
#include "shader.h"
#include "fbo.h"

#include "DvrWin.h"

CDvrWin::CDvrWin(void)
{
	t3dVol = 0;
	t1dTf = 0;
	t2dScreenColor = 0;
	t2dScreenDepth = 0;

						// shader object handle
	uPidTf1d = 0;		// handle to the shader that executes classification via 1D transfer function

						// FBO handle
	uFidScreen = 0;

	iMaxNrOfSlices = 4096;
	iNrOfSlices = 128;

						// dim. of the volume
	iXDim = 128;
	iYDim = 128;
	iZDim = 128;

						// the gain of the thickness
	fThicknessRatio = 1.0f;

						// not used FBO by default
	ibRenderToFbo = false;

	_AddGluiWin();				// there is a sub GLUI window

							// variable for setting the window resolution
	pcWidth_spinner  = NULL;
	pcHeight_spinner = NULL;

	fTfDomainMin = 0.0f;
	fTfDomainMax = 1.0f;
	fDataValueMin = 0.0f;
	fDataValueMax = 1.0f;
}

CDvrWin::~CDvrWin(void)
{
}

void
CDvrWin::_DisplayVolume()
{
	assert(t3dVol);
	assert(t1dTf);

	glPushAttrib(GL_ENABLE_BIT);
	glEnable( GL_DEPTH_TEST);
	glEnable( GL_BLEND);
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	glEnable( GL_TEXTURE_GEN_R );
	glEnable( GL_TEXTURE_GEN_Q );

								// step-1:
								// setup clipping planes
	static GLdouble pdClipPlanes[][4] = {
		{-1.0, 0.0, 0.0, 1.0},
		{ 1.0, 0.0, 0.0, 1.0},
		{ 0.0,-1.0, 0.0, 1.0},
		{ 0.0, 1.0, 0.0, 1.0},
		{ 0.0, 0.0,-1.0, 1.0},
		{ 0.0, 0.0, 1.0, 1.0},
	};

	for(int i=0; i<sizeof(pdClipPlanes)/sizeof(pdClipPlanes[0]); i++) 
	{
		glEnable(GL_CLIP_PLANE0+i);
		glClipPlane(GL_CLIP_PLANE0+i, pdClipPlanes[i]);
	}

								// step-2:
								// setup texture generation 
	glActiveTexture(GL_TEXTURE0);
	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGeni( GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );

	static TMatrix tTextureMatrix = { // this matrix is used to convert the coordinate from [-1, +1] to [0.0, 1.0].
		0.5f, 0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	glTexGendv( GL_S, GL_OBJECT_PLANE, &tTextureMatrix[0] );
	glTexGendv( GL_T, GL_OBJECT_PLANE, &tTextureMatrix[4] );
	glTexGendv( GL_R, GL_OBJECT_PLANE, &tTextureMatrix[8] );
	glTexGendv( GL_Q, GL_OBJECT_PLANE, &tTextureMatrix[12] );

								// step-3:
								// find the bounding box in eye space
	static GLdouble pdCornerCoord[][3] = {
		{-1.0, -1.0, -1.0},
		{-1.0, -1.0,  1.0},
		{-1.0,  1.0, -1.0},
		{-1.0,  1.0,  1.0},
		{ 1.0, -1.0, -1.0},
		{ 1.0, -1.0,  1.0},
		{ 1.0,  1.0, -1.0},
		{ 1.0,  1.0,  1.0},
	};

	double dMinX, dMinY, dMinZ;
	double dMaxX, dMaxY, dMaxZ;
	TMatrix tModelviewMatrix;

	dMinX = dMinY = dMinZ = HUGE_VAL;
	dMaxX = dMaxY = dMaxZ = -HUGE_VAL;
	glGetDoublev(GL_MODELVIEW_MATRIX, tModelviewMatrix);
	for(int i=0; i<sizeof(pdCornerCoord)/sizeof(pdCornerCoord[0]); i++) 
	{
		double dX, dY, dZ;
		gluProject(
			pdCornerCoord[i][0], pdCornerCoord[i][1], pdCornerCoord[i][2],
			tModelviewMatrix, tProjectionMatrix, piViewport,
			&dX, &dY, &dZ);

		if( dX < dMinX )	dMinX = dX;
		if( dX > dMaxX )	dMaxX = dX;

		if( dY < dMinY )	dMinY = dY;
		if( dY > dMaxY )	dMaxY = dY;

		if( dZ < dMinZ )	dMinZ = dZ;
		if( dZ > dMaxZ )	dMaxZ = dZ;
	}
								// bind the 3D texture of the volume
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, t3dVol);	

								// bind the 1D texture of the transfer func.
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, t1dTf);	

								// launch the shader object
	glUseProgramObjectARB(uPidTf1d);

	float fThickness;
	fThickness = fThicknessRatio * (float)(dMaxZ - dMinZ) / (float)iNrOfSlices;
	SET_1F_VALUE_BY_NAME(uPidTf1d, "fThickness", fThickness);
	SET_1F_VALUE_BY_NAME(uPidTf1d, "fWindowWidth", (float)piViewport[2]);
	SET_1F_VALUE_BY_NAME(uPidTf1d, "fWindowHeight", (float)piViewport[3]);

						// setup the range of the data
	SET_1F_VALUE_BY_NAME(uPidTf1d, "fDataValueMin", fDataValueMin);
	SET_1F_VALUE_BY_NAME(uPidTf1d, "fDataValueMax", fDataValueMax);

						// setup the range of the TF's support 
	SET_1F_VALUE_BY_NAME(uPidTf1d, "fTfDomainMin", fTfDomainMin);
	SET_1F_VALUE_BY_NAME(uPidTf1d, "fTfDomainMax", fTfDomainMax);

	glColor4d(1.0, 1.0, 1.0, 1.0 / (double)iNrOfSlices);
	for(int z=0; z<=iNrOfSlices; z++)
	{
		double dX, dY, dZ;

		double dColor = (double)z / (double)iNrOfSlices;
		double dDepth = (1.0 - (double)z / (double)iNrOfSlices) * (dMaxZ - dMinZ) + dMinZ; 

		glBegin(GL_QUADS);
			gluUnProject(dMinX, dMinY, dDepth,
				tModelviewMatrix, tProjectionMatrix, piViewport,
				&dX, &dY, &dZ);
			glVertex3d(dX, dY, dZ);

			gluUnProject(dMaxX, dMinY, dDepth,
				tModelviewMatrix, tProjectionMatrix, piViewport,
				&dX, &dY, &dZ);
			glVertex3d(dX, dY, dZ);

			gluUnProject(dMaxX, dMaxY, dDepth,
				tModelviewMatrix, tProjectionMatrix, piViewport,
				&dX, &dY, &dZ);
			glVertex3d(dX, dY, dZ);

			gluUnProject(dMinX, dMaxY, dDepth,
				tModelviewMatrix, tProjectionMatrix, piViewport,
				&dX, &dY, &dZ);
			glVertex3d(dX, dY, dZ);
		glEnd();
	}

	glPopAttrib();	// glPushAttrib(GL_ENABLE_BIT);	

	// disable the fragment shader
	glUseProgramObjectARB(0);

	// reset the active texture to texture 0
	glActiveTexture(GL_TEXTURE0);
}

///////////////////////////////////////////////////////////////
// GLUT Callbacks 
void
CDvrWin::_DisplayFunc()
{
	if( ibRenderToFbo )
		// bind the FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, uFidScreen);

	// clear the depth/color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// to maintain the aspect ratio, the coordinate is normalized 
	glScaled(1.0, (double)iYDim/(double)iXDim, (double)iZDim/(double)iXDim);

	// draw the bounding box
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(2.0f);

		glColor4f(0.6f, 0.6f, 0.2f, 1.0f);
		glutWireCube(2.0);

	glPopAttrib();	// glPushAttrib(GL_LINE_BIT);

	// apply direct volume rendering 
	_DisplayVolume();

	// bind the buffer back and render the FBO as texture on current screen
	if( ibRenderToFbo )
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t2dScreenColor);
		glEnable(GL_TEXTURE_2D);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f( 1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f( 1.0f,  1.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(-1.0f,  1.0f);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
}

void
CDvrWin::_ReshapeFunc(int w, int h)
{
									// since the viewport has been decided in _ReshapeCB, 
									// this one is not needed
	// allocate new FBO
	if( w && h )
	{
		if( ibRenderToFbo )
		{
			glBindTexture(GL_TEXTURE_2D, t2dScreenColor);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB,
				w, h, 0, GL_RGBA, GL_FLOAT, NULL);	

			glBindTexture(GL_TEXTURE_2D, t2dScreenDepth);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
				w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);	

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, uFidScreen);
			glFramebufferTexture2DEXT(
				GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
				GL_TEXTURE_2D, t2dScreenColor, 0);

			glFramebufferTexture2DEXT(
				GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
				GL_TEXTURE_2D, t2dScreenDepth, 0);

			assert( GL_FRAMEBUFFER_COMPLETE_EXT == EGetFboStatus(true) );

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}
	}

							// variable for setting the window resolution
	iWindowWidth = w;
	iWindowHeight = h;
}

void 
CDvrWin::_GluiFunc(unsigned short usValue)
{
	switch(usValue)
	{
	case WINDOW_SIZE:
		_Reshape(iWindowWidth, iWindowHeight);
		break;
	}
}

void 
CDvrWin::_SpecialFunc(int skey, int x, int y)
{
}

void
CDvrWin::_KeyboardFunc(unsigned char key, int x, int y)
{
	switch(key) 
	{
	case 'F': case 'f':	// F fior Fullscreen
		static bool bFullScreen;
		static int iWidth, iHeight;
		static int iWinPosX, iWinPosY;

		bFullScreen = !bFullScreen;
		if ( bFullScreen ) 
		{
			iWidth = glutGet( GLUT_WINDOW_WIDTH );
			iHeight = glutGet( GLUT_WINDOW_HEIGHT );
			iWinPosX = glutGet( GLUT_WINDOW_X );
			iWinPosY = glutGet( GLUT_WINDOW_Y );
			glutFullScreen();
		} 
		else
		{
			glutReshapeWindow(iWidth, iHeight);
			glutPositionWindow(iWinPosX, iWinPosY);
		}

		break;

	// double #slices
	case '*':
		if( iNrOfSlices * 2 <= iMaxNrOfSlices )
			iNrOfSlices *= 2;
		fprintf(stderr, "#Slices = %d\n", iNrOfSlices);

		glutPostRedisplay();
		break;

	// halve #slices
	case '/':
		if( iNrOfSlices / 2 > 0 )
			iNrOfSlices /= 2;
		fprintf(stderr, "#Slices = %d\n", iNrOfSlices);
		glutPostRedisplay();
		break;

	// change the thickness
	case '+':
		fThicknessRatio *= 2.0f;
		fprintf(stderr, "Thickness Ratio = %f\n", fThicknessRatio);
		glutPostRedisplay();
		break;

	// change the thickness
	case '-':
		if(	fThicknessRatio > 0.0f )
			fThicknessRatio /= 2.0f;
		fprintf(stderr, "Thickness Ratio = %f\n", fThicknessRatio);
		glutPostRedisplay();
		break;
	}
}

///////////////////////////////////////////////////////////////
// basic subroutines

void
CDvrWin::_InitFunc()
{
	// initialize GLEW and check required OpenGL extension
	int iGlewInit = glewInit();
	assert(GLEW_OK == iGlewInit);

	// load the shader
	uPidTf1d = CSetShadersByString(
		NULL, 
		#include "tf_1d.frag.h"
	);

	assert( uPidTf1d );

	// setup value for variable inside the shader
	glUseProgramObjectARB(uPidTf1d);
	SET_1I_VALUE_BY_NAME(uPidTf1d, "t3dVol",	0);
	SET_1I_VALUE_BY_NAME(uPidTf1d, "t1dTf",	2);
	glUseProgramObjectARB(0);

	// setup the blending equation for back-to-front composition
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// allocate the textures for FBOs
	if( ibRenderToFbo )
	{
		CREATE_2D_TEXTURE(
			GL_TEXTURE_2D, t2dScreenColor, 
			GL_NEAREST, GL_RGBA16F_ARB, 
			128, 128, // the real resolution will be determined later
			GL_RGBA, GL_FLOAT, NULL);

		CREATE_2D_TEXTURE(
			GL_TEXTURE_2D, t2dScreenDepth, 
			GL_NEAREST, GL_DEPTH_COMPONENT24,
			128, 128, // the real resolution will be determined later
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 

		glGenFramebuffersEXT(1, &uFidScreen);	// allocate framebuffer object
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	_DisplayFpsOn();		// the FPS will be displayed 
	_KeepUpdateOn();		// the frame will be keep updating

							// set the lower/upper limits of #slices
	GLUI_Spinner *pcSpinner = PCGetGluiWin()->add_spinner("#Slices", GLUI_SPINNER_INT, &iNrOfSlices);	
	pcSpinner->set_int_limits(1, iMaxNrOfSlices);

							// set the thickness ratio for DVR
	GLUI_Spinner *pcSpinner_ThicknessRatio = PCGetGluiWin()->add_spinner("Thickness Gain", GLUI_SPINNER_FLOAT, &fThicknessRatio);	

							// create two spinners to specify the window resolution
	GLUI_Rollout* pcWindowPaenl = PCGetGluiWin()->add_rollout("Window", false);
	pcWidth_spinner = 
		PCGetGluiWin()->add_spinner_to_panel(pcWindowPaenl, "Width", GLUI_SPINNER_INT, &iWindowWidth, IAddWid(WINDOW_SIZE), &CGlutWin::_GluiCB_static);	
	pcWidth_spinner->set_int_limits(1, 1024);
	pcHeight_spinner = 
		PCGetGluiWin()->add_spinner_to_panel(pcWindowPaenl, "Height", GLUI_SPINNER_INT, &iWindowHeight, IAddWid(WINDOW_SIZE), &CGlutWin::_GluiCB_static);
	pcHeight_spinner->set_int_limits(1, 1024);

		// get the size of this window
		// since it is assumed to be w/o subwindow, 
		// just call glutGet to obtain the size
	iWindowWidth	= glutGet(GLUT_WINDOW_WIDTH);
	iWindowHeight	= glutGet(GLUT_WINDOW_HEIGHT);

	_DisableVerticalSync();
}

// load the volume and upload it as a 3D texture 
void 
CDvrWin::_SetVolume(
	GLenum eInternalFormat,
	const void *pVol, 
	GLenum eType, 
	GLenum eFormat, 
	int iXDim, 
	int iYDim, 
	int iZDim)
{
_Begin();

	this->iXDim = iXDim;
	this->iYDim = iYDim;
	this->iZDim = iZDim;

	glActiveTexture(GL_TEXTURE0);

	if( !t3dVol )
		glGenTextures(1, &t3dVol);	

	glBindTexture(GL_TEXTURE_3D, t3dVol);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexImage3D(GL_TEXTURE_3D, 0, eInternalFormat,	
		(GLsizei)iXDim, (GLsizei)iYDim, (GLsizei)iZDim, 0, eType, eFormat, pVol);

_End();
}

// load the transfer function and upload it as a 1D texture
void 
CDvrWin::_SetTransferFunc(
	const void *pTf, 
	GLenum eType, 
	GLenum eFormat, 
	int iNrOfTfEntries)
{
_Begin();

	// upload the transfer func. as a 1D texture
	glActiveTexture(GL_TEXTURE2);
	if( !t1dTf )
		glGenTextures(1, &t1dTf);	
	glBindTexture(GL_TEXTURE_1D, t1dTf);	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16F_ARB,	
		iNrOfTfEntries, 0, eType, eFormat, pTf);

_End();
}

