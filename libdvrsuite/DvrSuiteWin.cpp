#include <GL/glew.h>

#include "shader.h"	
#include "libclock.h"	

#include "DvrSuiteWin.h"

void
CDvrSuiteWin::
_Receive
(
	CTransFunc *pcTransFunc
)
{
	pcTransFunc->_ExportColorMap(
		(float*)&vf4TransFunc.front(),
		(int)vf4TransFunc.size());

	float fTfDomainMin, fTfDomainMax;
	cTransFunc._GetTfDomain(&fTfDomainMin, &fTfDomainMax);
	_SetTfDomain(fTfDomainMin, fTfDomainMax);
	_SetTransferFunc(
		&vf4TransFunc.front(), 
		GL_RGBA, 
		GL_FLOAT, 
		(int)vf4TransFunc.size());
	_Redisplay();
}

template<typename T>
void
CDvrSuiteWin::
	_ConvertDataToTexture
	(
		Nrrd *nin
	)
{
	T *data = (T*)nin->data;	

	size_t uNrOfDims = nin->dim;
	size_t uNrOfElements = 1;
	for(size_t d = 0; d < uNrOfDims; d++)
		uNrOfElements *= nin->axis[d].size;

	// search for the range
	dValueMin = HUGE_VAL;
	dValueMax = -HUGE_VAL;
	for(size_t v = 0; v < uNrOfElements; v++)
	{
		dValueMin = min(dValueMin, (double)data[v]);
		dValueMax = max(dValueMax, (double)data[v]);
	}
	LOG_VAR(dValueMin);
	LOG_VAR(dValueMax);

	size_t uNrOfBins = vfHist.size();
	for(size_t v = 0; v < uNrOfElements; v++)
	{
		size_t uBin = (size_t)floor((double)(uNrOfBins - 1) * ((double)data[v] - dValueMin) / (double)(dValueMax - dValueMin));
		uBin = min(max(uBin, (size_t)0), uNrOfBins - 1);	
		vfHist[uBin] += 1.0f;
	}

	// normalize the histogram
	fMaxCount = -(float)HUGE_VAL;
	for(size_t b = 0; b < vfHist.size(); b++) 
		fMaxCount = max(fMaxCount, vfHist[b]);

	for(size_t b = 0; b < vfHist.size(); b++) 
		vfHist[b] /= fMaxCount;

	// normalize the volume to [0, 1] for floating point data
	if( nrrdTypeFloat == nin->type )
	{
		float *data = (float*)nin->data;	
		for(size_t v = 0; v < uNrOfElements; v++)
			data[v] = (data[v] - (float)dValueMin)/(float)(dValueMax - dValueMin);
	}
}

void
CDvrSuiteWin::
	_SetHistogramSize
	(
		size_t uNrOfBins
	)
{
	vfHist.assign(uNrOfBins, 0.0f);
}

void
CDvrSuiteWin::
	_SetTfSize
	(
		size_t uNrOfTfEntries
	)
{
	vf4TransFunc.assign(uNrOfTfEntries, make_float4(0.0f, 0.0f, 0.0f, 0.0f));
}

void
CDvrSuiteWin::
	_LoadData
	(
		char* szFilepath,
		void* _Reserved
	)
{
	nin = nrrdNew();
	if (nrrdLoad(nin, szFilepath, NULL)) {
		char *err = biffGetDone(NRRD);
		LOG_ERROR(fprintf(stderr, "%s", err));
		free(err);
		exit(EXIT_FAILURE);
	}

	switch(nin->type)
	{
	case nrrdTypeUChar:	_ConvertDataToTexture<unsigned char>(nin);	break;
	case nrrdTypeChar:	_ConvertDataToTexture<char>(nin);			break;
	case nrrdTypeShort:	_ConvertDataToTexture<short>(nin);			break;
	case nrrdTypeUShort:_ConvertDataToTexture<unsigned short>(nin);	break;
	case nrrdTypeInt:	_ConvertDataToTexture<int>(nin);			break;
	case nrrdTypeUInt:	_ConvertDataToTexture<unsigned int>(nin);	break;
	case nrrdTypeFloat:	_ConvertDataToTexture<float>(nin);			break;
	default:
		break;
	}

	// build the transfer function
	cTransFunc._LoadRainBow();
	cTransFunc._ExportColorMap(
		(float*)&vf4TransFunc.front(), 
		(int)vf4TransFunc.size());
	cTransFunc._SetTfDomain((float)dValueMin, (float)dValueMax);
}

void 
CDvrSuiteWin::
	_CreateTfWins()
{
	///////////////////////////////////////////////////////////////
	// create the TF window beforehand 
	cTfWin._SetTransFunc(&cTransFunc);
	cTfWin._SetNrOfEntries(256);
	cTfWin.ICreate("Transfer Function");
	cTfWin._Set();
	cTfWin._SetHistogram(
		&vfHist.front(), 
		(int)vfHist.size(), 
		fMaxCount, 
		(float)dValueMin, 
		(float)dValueMax);
	cTfWin._KeepUpdateOn();

	cTfUi._SetTransFunc(&cTransFunc);
	cTfUi.ICreate("Transfer Function Editor");
	cTfUi._SetHistogramAsBackground(
		&vfHist.front(), 
		(int)vfHist.size(), 
		dValueMin, 
		dValueMax);
	cTfUi._SetReceiver((CTfUi::CReceiver*)this);	

	// reset the window back to the current window
	_Set();
}

void 
CDvrSuiteWin::_InitFunc()
{
	CDvrWin2::_InitFunc();

	_KeepUpdateOn();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_DisableVerticalSync();
	_DisplayFpsOn();		// the FPS will be displayed 

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

	////////////////////////////////////////////////////////////////
	// upload the volume as a texture
	int iType;
	switch(nin->type)
	{
	case nrrdTypeUChar:	iType = GL_UNSIGNED_BYTE;	break;
	case nrrdTypeChar:	iType = GL_BYTE;			break;
	case nrrdTypeShort:	iType = GL_SHORT;			break;
	case nrrdTypeUShort:iType = GL_UNSIGNED_SHORT;	break;
	case nrrdTypeInt:	iType = GL_INT;				break;
	case nrrdTypeUInt:	iType = GL_UNSIGNED_INT;	break;
	case nrrdTypeFloat:	iType = GL_FLOAT;			break;
	}

	// upload the data as a texture
	switch(nin->dim)
	{
	case 3:
		_SetVolume(
			GL_LUMINANCE32F_ARB, 
			nin->data, 
			iType, 
			GL_LUMINANCE, 
			(int)nin->axis[0].size, 
			(int)nin->axis[1].size, 
			(int)nin->axis[2].size);
		break;
	case 2:
		// not supported yet. It requires the overloading of _SetVolume().		
		break;
	}

	////////////////////////////////////////////////////////////////
	_SetTransferFunc(
		(float*)&vf4TransFunc.front(), 
		GL_RGBA, 
		GL_FLOAT, 
		(int)vf4TransFunc.size());
	_LoadSavedMatrix();		
	_SetDataValue((float)dValueMin, (float)dValueMax);
	_SetTfDomain((float)dValueMin, (float)dValueMax);
}

void 
CDvrSuiteWin::_ReshapeFunc(int w, int h)
{
	CDvrWin2::_ReshapeFunc(w, h);
	CClipVolume::_ReshapeFunc(w, h);
}

//////////////////////////////////////////////////////
void 
CDvrSuiteWin::
	_BeginDisplay()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	float fMaxDim = (float)max(iXDim, max(iYDim, iZDim));
	glScalef(
		(float)iXDim / fMaxDim,
		(float)iYDim / fMaxDim,
		(float)iZDim / fMaxDim);
	glutWireCube(2.0);	// plot the bounding box

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
CDvrSuiteWin::
	_EndDisplay()
{
	glUseProgramObjectARB(0);
	glPopMatrix();
}

void 
CDvrSuiteWin::
	_RenderSlab(
		int iSlab, int iNrOfSlabs,
		double pdModelviewMatrix[], double pdProjectionMatrix[], int piViewport[],
		double dMinX, double dMaxX, 
		double dMinY, double dMaxY, 
		double dMinZ, double dMaxZ
	)
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

//////////////////// Constructors/Destructors //////////////////// 
CDvrSuiteWin::
	CDvrSuiteWin(void)
{
	ibIsFboEnabled = 1;							// enable the rendering to FBO

	_SetInternalColorFormat(GL_RGBA32F_ARB);	// set the depths of each chanel of the FBO as 32 bits 
	_SetInternalDepthFormat(GL_DEPTH_COMPONENT);

	fThicknessGain = 1.0f;
	iNrOfSlices = 128;

	// initiate the histogram size to the default value
	_SetHistogramSize();
	_SetTfSize();

	// create a separate panel
	_AddGluiWin();	
}

CDvrSuiteWin::
	~CDvrSuiteWin(void)
{
}

