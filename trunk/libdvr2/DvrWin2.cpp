	#include "shader.h"	

	#include "libdvr2/DvrWin2.h"

//////////////////////////////////////////////////////
void 
CDvrWin2::_BeginDisplay()
{
}

void 
CDvrWin2::_EndDisplay()
{
}

// ADD-BY-LEETEN 04/06/2010-BEGIN
void 
CDvrWin2::_SetVolume(
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

void 
CDvrWin2::_SetTransferFunc(
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
// ADD-BY-LEETEN 04/06/2010-END

// ADD-BY-LEETEN 12/30/2009-BEGIN
void 
CDvrWin2::_RenderSlab(
	int iSlab, int iNrOfSlabs,
	double pdModelviewMatrix[],
	double pdProjectionMatrix[],
	int piViewport[],
	double dMinX, double dMaxX, 
	double dMinY, double dMaxY, 
	double dMinZ, double dMaxZ)
{
	TMatrix tIdentityMatrix = 
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	};

	double dX, dY, dZ;
	double dX_win, dY_win, dZ_win;
	double dColor = (double)iSlab / (double)iNrOfSlabs;
	double dDepth = ((double)iSlab / (double)iNrOfSlabs) * (dMaxZ - dMinZ) + dMinZ; 

	glBegin(GL_QUADS);
		gluProject(		dMinX,	dMinY,	dDepth,	tIdentityMatrix,	pdProjectionMatrix, piViewport, &dX_win, &dY_win, &dZ_win);
		gluUnProject(	dX_win, dY_win, dZ_win, pdModelviewMatrix,	pdProjectionMatrix, piViewport, &dX, &dY, &dZ);
		glVertex3d(		dX, dY, dZ);

		gluProject(		dMaxX,	dMinY, dDepth,	tIdentityMatrix,	pdProjectionMatrix, piViewport, &dX_win, &dY_win, &dZ_win);
		gluUnProject(	dX_win, dY_win, dZ_win, pdModelviewMatrix,	pdProjectionMatrix, piViewport, &dX, &dY, &dZ);
		glVertex3d(		dX, dY, dZ);

		gluProject(		dMaxX,	dMaxY, dDepth,	tIdentityMatrix,	pdProjectionMatrix, piViewport, &dX_win, &dY_win, &dZ_win);
		gluUnProject(	dX_win, dY_win, dZ_win, pdModelviewMatrix,	pdProjectionMatrix, piViewport, &dX, &dY, &dZ);
		glVertex3d(		dX, dY, dZ);

		gluProject(		dMinX,	dMaxY, dDepth,	tIdentityMatrix,	pdProjectionMatrix, piViewport, &dX_win, &dY_win, &dZ_win);
		gluUnProject(	dX_win, dY_win, dZ_win, pdModelviewMatrix,	pdProjectionMatrix, piViewport, &dX, &dY, &dZ);
		glVertex3d(		dX, dY, dZ);
	glEnd();
}
// ADD-BY-LEETEN 12/30/2009-END


//////////////////////////////////////////////////////
void 
CDvrWin2::_DisplayFunc()
{
	this->_EnableFbo();

	_BeginDisplay();

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

	// ADD-BY-LEETEN 12/30/2009-BEGIN
	TMatrix tIdentityMatrix = 
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	};
	// ADD-BY-LEETEN 12/30/2009-END


	dMinX = dMinY = dMinZ = HUGE_VAL;
	dMaxX = dMaxY = dMaxZ = -HUGE_VAL;
	glGetDoublev(GL_MODELVIEW_MATRIX, tModelviewMatrix);
	for(int i=0; i<sizeof(pdCornerCoord)/sizeof(pdCornerCoord[0]); i++) 
	{
		#if	0		// MOD-BY-LEETEN 12/30/2009-FROM:
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
		#else	// MOD-BY-LEETEN 12/30/2009-TO:
		double dX_win, dY_win, dZ_win;
		double dX_eye, dY_eye, dZ_eye;
		gluProject(
			pdCornerCoord[i][0], pdCornerCoord[i][1], pdCornerCoord[i][2],
			tModelviewMatrix, tProjectionMatrix, piViewport,
			&dX_win, &dY_win, &dZ_win);

		gluUnProject(
			dX_win, dY_win, dZ_win,
			tIdentityMatrix, tProjectionMatrix, piViewport,
			&dX_eye, &dY_eye, &dZ_eye);

		dMinX = min(dMinX, dX_eye);
		dMaxX = max(dMaxX, dX_eye);
		dMinY = min(dMinY, dY_eye);
		dMaxY = max(dMaxY, dY_eye);
		dMinZ = min(dMinZ, dZ_eye);
		dMaxZ = max(dMaxZ, dZ_eye);
		#endif	// MOD-BY-LEETEN 12/30/2009-END
	}

						// initialize the index for pint-pong rendering			
	int iSlab = 0;

						// clean the buffer for ping pong rendering
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	for(int s = 0; s < 2; s++)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pcSlabs[s].fid);
		static GLuint puRids[] = {
			GL_COLOR_ATTACHMENT1_EXT,
			GL_COLOR_ATTACHMENT2_EXT,
		};
		glDrawBuffers(sizeof(puRids)/sizeof(puRids[0]), puRids);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	glPopAttrib();	// glPushAttrib(GL_COLOR_BUFFER_BIT);

						// fixed the FBO to the one for the 1st slab
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pcSlabs[0].fid);

	glColor4d(1.0, 1.0, 1.0, 1.0 / (double)iNrOfSlices);
	for(int z=0; z<=iNrOfSlices; z++)
	{
							// setup the dst. FBO
		if( 0 == iSlab )
		{
			static GLuint puRids[] = {
				GL_COLOR_ATTACHMENT0_EXT,
				GL_COLOR_ATTACHMENT1_EXT
			};
			glDrawBuffers(sizeof(puRids)/sizeof(puRids[0]), puRids);
		}
		else
		{
			static GLuint puRids[] = {
				GL_COLOR_ATTACHMENT0_EXT,
				GL_COLOR_ATTACHMENT2_EXT
			};
			glDrawBuffers(sizeof(puRids)/sizeof(puRids[0]), puRids);
		}
							// setup the src. FBO
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(pcSlabs[1 - iSlab].cBuffer.eTarget, pcSlabs[1 - iSlab].cBuffer.t2d);
	
							// rendering the quad
		#if	0	// MOD-BY-LEETEN 12/30/2009-FROM:
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
		#else	// MOD-BY-LEETEN 12/30/2009-TO:
		_RenderSlab(
			z, iNrOfSlices, 

			tModelviewMatrix, tProjectionMatrix, piViewport,
			
			dMinX, dMaxX, 
			dMinY, dMaxY, 
			dMinZ, dMaxZ);
		#endif	// MOD-BY-LEETEN 12/30/2009-END

							// switch the slab index for ping-pong rendering
		iSlab = 1 - iSlab;
	}

	glPopAttrib();	// glPushAttrib(GL_ENABLE_BIT);	

	_EndDisplay();

	// reset the active texture to texture 0
	glActiveTexture(GL_TEXTURE0);

	this->_DisableFbo();
}

void 
CDvrWin2::_ReshapeFunc(int w, int h)
{
	CFboWin::_ReshapeFunc(w, h);

	if( w > 0 && h > 0 )
	{
		for(int s = 0; s < 2; s++)
		{
			pcSlabs[s].cBuffer._SetResolution(w, h);
		}

		for(int s = 0; s < 2; s++)
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pcSlabs[s].fid);
			glFramebufferTexture2DEXT(
				GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
				cColor.eTarget, cColor.t2d, 0);

			glFramebufferTexture2DEXT(
				GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
				pcSlabs[s].cBuffer.eTarget, pcSlabs[s].cBuffer.t2d, 0);

			glFramebufferTexture2DEXT(
				GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
				pcSlabs[1-s].cBuffer.eTarget, pcSlabs[1-s].cBuffer.t2d, 0);

			glFramebufferTexture2DEXT(
				GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
				cDepth.eTarget, cDepth.t2d, 0);

			// MOD-BY-LEETEN 08/05/2010-FROM:
				// assert( GL_FRAMEBUFFER_COMPLETE_EXT == EGetFboStatus(true) );
			// TO:
			unsigned int iFboStatus = EGetFboStatus(true);
			assert( GL_FRAMEBUFFER_COMPLETE_EXT == iFboStatus );
			// MOD-BY-LEETEN 08/05/2010-END
		}

		// reset the FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

void 
CDvrWin2::_InitFunc()
{
	glewInit();

						// setup the blending equation for back-to-front composition
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						// initialize the default FBO
	_InitFbo();

						// initialize the 2 FBO for ping-pong rendering
	for(int s = 0; s < 2; s++)
	{
		pcSlabs[s].cBuffer._Create();
		glGenFramebuffersEXT(1, &pcSlabs[s].fid);	// allocate framebuffer object
	}
}

//////////////////////////////////////////////////////
CDvrWin2::CDvrWin2(void)
{
	// ADD-BY-LEETEN 04/06/2010-BEGIN
	t1dTf = 0;
	t3dVol = 0;
	iNrOfSlices = 128;
	fThicknessGain = 1.0f;
	// ADD-BY-LEETEN 04/06/2010-END
}

CDvrWin2::~CDvrWin2(void)
{
}

/*

$Log: not supported by cvs2svn $
Revision 1.3  2010/04/06 19:55:23  leeten

[04/06/2010]
1. [ADD] Define a new method _SetVolume to upload the specified volume as a 3D texture.
2. [ADD] Define a new method _SetTransferFunc to specify the transfer function.

Revision 1.2  2009/12/31 01:43:14  leeten

[12/30/2009]
1. [ADD] Define a new method _RenderSlab() to render a slab. This method can be redfined by the inherited classes.
2. [MOD] Change the space to do slicing from ndc to eye space.

Revision 1.1  2009/10/27 14:04:07  leeten

[2009/10/27]
1. [1ST] First time checkin. This library defined DvrWin2, a new class for volume rendering. This class already supports buildit-in ping-pong rendering.


*/
