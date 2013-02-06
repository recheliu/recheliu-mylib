#include <GL/glew.h>

#include <stdio.h>

#include "shader.h"

#include "DvrSampleWin.h"

void 
CDvrSampleWin::_BeginDisplay()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	float fMaxDim = max(iXDim, max(iYDim, iZDim));
	glScalef(
		iXDim / fMaxDim,
		iYDim / fMaxDim,
		iZDim / fMaxDim);

	// ADD-BY-LEETEN 09/10/2010-BEGIN
	glutWireCube(2.0);
	// ADD-BY-LEETEN 09/10/2010-END

	CClipVolume::_Create();

	glUseProgramObjectARB(	pidRayIntegral);
	SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fWindowWidth",				(float)piViewport[2]);
	SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fWindowHeight",			(float)piViewport[3]);
	SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fThicknessGain",			fThicknessGain);

	SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t2dPrevLayer",				0);
	SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t3dVolume",				1);
	SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t1dTf",					2);
	SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t2dClipVolume",			4);
	SET_1I_VALUE_BY_NAME(	pidRayIntegral, "t2dsDepth",				5);

	SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fTfDomainMin",				fTfDomainMin);
	SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fTfDomainMax",				fTfDomainMax);
	SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fDataValueMin",			fDataValueMin);
	SET_1F_VALUE_BY_NAME(	pidRayIntegral, "fDataValueMax",			fDataValueMax);

	glUseProgramObjectARB(0);

						// bind the volume, range, and the lookup table as textures
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_3D, t3dVol);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_1D, t1dTf);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(CDvrWin2::cColor.eTarget, CDvrWin2::cColor.t2d);

	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(CClipVolume::cTexture.eTarget, CClipVolume::cTexture.t2d);

	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(CDvrWin2::cDepth.eTarget, CDvrWin2::cDepth.t2d);

	glActiveTexture(GL_TEXTURE0);
}

void 
CDvrSampleWin::_EndDisplay()
{
	glUseProgramObjectARB(0);
	glPopMatrix();
}

void 
CDvrSampleWin::_RenderSlab(
		int iSlab, int iNrOfSlabs,
		double pdModelviewMatrix[], double pdProjectionMatrix[], int piViewport[],
		double dMinX, double dMaxX, 
		double dMinY, double dMaxY, 
		double dMinZ, double dMaxZ)
{
	glPushAttrib(
		GL_ENABLE_BIT |
		GL_DEPTH_BUFFER_BIT |
		0 );
	glEnable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);

	glUseProgramObjectARB(pidRayIntegral);
	CDvrWin2::_RenderSlab(
		iSlab, iNrOfSlabs, 

		pdModelviewMatrix, pdProjectionMatrix, piViewport,
		
		dMinX, dMaxX, 
		dMinY, dMaxY, 
		dMinZ, dMaxZ);

	glPopAttrib();	// glPushAttrib(GL_DEPTH_BUFFER_BIT);
}

void 
CDvrSampleWin::_ReshapeFunc(int w, int h)
{
	// ADD-BY-LEETEN 01/05/2010-BEGIN
	CClipVolume::_ReshapeFunc(w, h);
	// ADD-BY-LEETEN 01/05/2010-END
	CDvrWin2::_ReshapeFunc(w, h);
}


void 
CDvrSampleWin::_InitFunc()
{
	////////////////////////////////////////////////////////////////
	CDvrWin2::_InitFunc();

	// ADD-BY-LEETEN 09/10/2010-BEGIN
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_DisableVerticalSync();
	_DisplayFpsOn();		// the FPS will be displayed 
	_KeepUpdateOn();		// the frame will be keep updating
	// ADD-BY-LEETEN 09/10/2010-END

	pidRayIntegral = CSetShadersByString(
		NULL
		,
		#include "ray_integral.frag.h"	
	);
	assert( pidRayIntegral );	

	////////////////////////////////////////////////////////////////
	CClipVolume::_InitFunc();

	////////////////////////////////////////////////////////////////
	// setup GLUI 
	GLUI *pcGlui = PCGetGluiWin();

	GLUI_Spinner *pcSpinner_NrOfSlices = PCGetGluiWin()->add_spinner("#Slices", GLUI_SPINNER_INT, &iNrOfSlices);	
		pcSpinner_NrOfSlices->set_int_limits(1, 4096);

						// create a spinner to control the brightness gain 
	GLUI_Spinner *pcSpinner_ThicknessGain = PCGetGluiWin()->add_spinner("Thickness Gain", GLUI_SPINNER_FLOAT, &fThicknessGain);	
	pcSpinner_ThicknessGain->set_float_limits(0.0f, 4096.0f);
}

CDvrSampleWin::CDvrSampleWin(void)
{
	ibIsFboEnabled = 1;							// enable the rendering to FBO
	_SetInternalColorFormat(GL_RGBA32F_ARB);	// set the depths of each chanel of the FBO as 32 bits 
	_SetInternalDepthFormat(GL_DEPTH_COMPONENT);

	fThicknessGain = 1.0f;

	iNrOfSlices = 128;

	_AddGluiWin();
}

CDvrSampleWin::~CDvrSampleWin(void)
{
}

/*

$Log: not supported by cvs2svn $
Revision 1.2  2010/09/10 14:07:15  leeten

[09/10/2010]
1. [ADD] Draw a 3D cube as the bounding box.
2. [ADD] Explictly enable the depth test and blending.

Revision 1.1  2010/04/06 20:01:15  leeten

[04/06/2010]
1. [1ST] First time checkin.


*/
