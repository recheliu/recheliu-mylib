///////////////////////////////////////////////////////////////////////
// headers
	#include <GL/glew.h>

	#include <stdio.h>

	// my own program argument parser
	#include "libopt.h"

	// my own template library for allocating buffers
	#include "libbuf.h"

	#include "libdvr/DvrWin.h"
	#include "libtfw/TfWin.h"
	#include "libtfw/TfUi.h"	
	#include "libtfw/TransFunc.h"

///////////////////////////////////////////////////////////////////////
// variables

	// variable for the transfer func.
	int iNrOfTfEntries = 256;
	CTransFunc cTransFunc;

					// the GLUT wins
	CDvrWin cDvrWin;
	CTfWin	cTfWin;
	CTfUi	cTfUi;	

					// buffer for the 3D volume
	int iXDim, iYDim, iZDim;
	TBuffer<unsigned char> pubVol;
	double dValueMin, dValueMax;

					// histogram of the buffer
	TBuffer<float> pfHist;

					// buffer of the histogram
	TBuffer<float> pfTransFunc;

void
_ReadVolume(char *szPathFilename)
{
	// read the volume from file
	FILE *fpData;
	fpData = fopen(szPathFilename, "rb");		// load the hipip dataset for test
	assert(fpData);

	fscanf(fpData, "%d %d %d\n", &iXDim, &iYDim, &iZDim);
	fseek(fpData, 9, SEEK_SET);

	pubVol.alloc(iXDim * iYDim * iZDim);

	fread(&pubVol[0], sizeof(pubVol[0]), iXDim * iYDim * iZDim, fpData);

	fclose(fpData);

									// this value is for hipip only!
	dValueMin = 0.0;
	dValueMax = 255.0;
}

void
_BuildHistogram()
{
	pfHist.alloc(iNrOfTfEntries);
	for(int i = 0,	z = 0; z < iZDim; z++)
		for(int		y = 0; y < iYDim; y++)
			for(int x = 0; x < iXDim; x++, i++)
				pfHist[pubVol[i]] += 1.0f;

	// normalize the histogram
	float fMaxCount = -HUGE_VAL;
	for(int b = 0; b < iNrOfTfEntries; b++) 
		fMaxCount = max(fMaxCount, pfHist[b]);

	for(int b = 0; b < iNrOfTfEntries; b++) 
		pfHist[b] /= fMaxCount;
}

/////////////////////////////////////////////////////////
void
_UpdateTf()
{
	cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);

	float fTfDomainMin, fTfDomainMax;
	cTransFunc._GetTfDomain(&fTfDomainMin, &fTfDomainMax);
	cDvrWin._SetTfDomain(fTfDomainMin, fTfDomainMax);

	cDvrWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);
	cDvrWin._Redisplay();
}

/////////////////////////////////////////////////////////
void
quit()
{
}

int
main(int argn, char *argv[])
{
	CGlutWin::_Init(
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

	int iNextArg;
	_OPTInit(true);

	_OPTAddIntegerVector(	"--dvr-nr_of_slices", 1, &cDvrWin.iNrOfSlices, 128);
		_OPTAddComment(		"--dvr-nr_of_slices", "Specify #slices.");

	_OPTAddFloatingVector(	"--dvr-thickness_ratio", 1, &cDvrWin.fThicknessRatio, 1.0f);
		_OPTAddComment(		"--dvr-thickness_ratio", "Specify the thickness gain.");

	_OPTAddIntegerVector(	"--dvr-max_nr_of_slices", 1, &cDvrWin.iMaxNrOfSlices, 4096);
		_OPTAddComment(		"--dvr-max_nr_of_slices", "Specify max. #slices.");

	_OPTAddBoolean(			"--dvr-rendering-to-fbo", &cDvrWin.ibRenderToFbo, true);
		_OPTAddComment(		"--dvr-rendering-to-fbo", 
			"Specify whether the composition result is rendered into a FBO then mapped onto the screen. "
			"The benefit of rendering to FBO is that it can provide higher numerical precision, "
			"but it can cause segmentation fault at exit on certain platforms (such as nVida Quadro NVS 100M). "
			"As a result, it can be disabled via this option. "
			);

	bool bIsOptParsed = BOPTParse(argv, argn, 1, &iNextArg);
	assert( bIsOptParsed ); 

	atexit(quit);

	#if 0 // TEST
		if( argn - iNextArg < 4 )
		{
			fprintf(stderr, "Usage: %s [<options>] <data.vol> <trasnfer_function.txt>\n", argv[0]);
			return 0;
		}
	#endif	// TEST

	// load the transfer func. and data set
	#if	1	// TEST
		// load the data set
		assert(iNextArg < argn);
		_ReadVolume(argv[iNextArg]);

		// use the rainbow transfer func. for testing
		pfTransFunc.alloc(CTransFunc::NR_OF_COLORS * iNrOfTfEntries);
		cTransFunc._LoadRainBow();
		cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);
		cTransFunc._SetTfDomain((float)dValueMin, (float)dValueMax);

	#endif	// TEST

	_BuildHistogram();

										// create the DVR window
	cDvrWin.ICreate("Direct Volume Rendering",	false, 100, 328, 256, 256);
		cDvrWin._SetVolume(GL_LUMINANCE, &pubVol[0], GL_LUMINANCE, GL_UNSIGNED_BYTE, iXDim, iYDim, iZDim);
		cDvrWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);

		cDvrWin._LoadSavedMatrix();	

		cDvrWin._SetDataValue((float)dValueMin, (float)dValueMax);
		cDvrWin._SetTfDomain((float)dValueMin, (float)dValueMax);
										// create the Transfer Func. window
	cTfWin._SetTransFunc(&cTransFunc);
	cTfWin._SetNrOfEntries(256);

	cTfWin.ICreate("Transfer Function",		false, 100, 100, 256, 128);
	cTfWin._Set();
		cTfWin._KeepUpdateOn();

										// upload the transfer function to the TF window
										// create the TF Editor window
	cTfUi._SetTransFunc(&cTransFunc);
	cTfUi.ICreate("Transfer Function Editor");
		cTfUi._SetHistogramAsBackground(&pfHist[0], iNrOfTfEntries, dValueMin, dValueMax);
		cTfUi._SetUpdateFunc(_UpdateTf);

	glutMainLoop();

	return 0;
}

