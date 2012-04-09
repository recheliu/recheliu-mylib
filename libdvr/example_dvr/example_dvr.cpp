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
	// ADD-BY-TLEE 08/13/2008-BEGIN
	CTfUi	cTfUi;	
	// ADD-BY-TLEE 08/13/2008-END

					// buffer for the 3D volume
	int iXDim, iYDim, iZDim;
	TBuffer<unsigned char> pubVol;
	// ADD-BY-TLEE 2008/08/17-BEGIN
	double dValueMin, dValueMax;
	// ADD-BY-TLEE 2008/08/17-END

	// ADD-BY-TLEE 08/13/2008-BEGIN
					// histogram of the buffer
	TBuffer<float> pfHist;

					// buffer of the histogram
	TBuffer<float> pfTransFunc;
	// ADD-BY-TLEE 08/13/2008-END

void
// MOD-BY-LEETEN 01/25/2011-FROM:
	// _ReadVolume()
// TO:
_ReadVolume(char *szPathFilename)
// MOD-BY-LEETEN 01/25/2011-END
{
	// read the volume from file
	FILE *fpData;
	// MOD-BY-LEETEN 01/25/2011-FROM:
		// fpData = fopen("../data/hipip", "rb");		// load the hipip dataset for test
	// TO:
	fpData = fopen(szPathFilename, "rb");		// load the hipip dataset for test
	// MOD-BY-LEETEN 01/25/2011-END
	assert(fpData);

	fscanf(fpData, "%d %d %d\n", &iXDim, &iYDim, &iZDim);
	fseek(fpData, 9, SEEK_SET);

	pubVol.alloc(iXDim * iYDim * iZDim);

	fread(&pubVol[0], sizeof(pubVol[0]), iXDim * iYDim * iZDim, fpData);

	fclose(fpData);

	// ADD-BY-TLEE 2008/08/17-BEGIN
									// this value is for hipip only!
	dValueMin = 0.0;
	dValueMax = 255.0;
	// ADD-BY-TLEE 2008/08/17-END
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
// ADD-BY-TLEE 08/13/2008-BEGIN
void
_UpdateTf()
{
	#if	0	// MOD-BY-TLEE 08/14/2008-FROM:
		cTfUi._CreateTransferFunc(iNrOfTfEntries, &pfTransFunc[0]);

		cTfWin._SetTransferFunc(&pfTransFunc[0], iNrOfTfEntries);
		cTfWin._Redisplay();
	#else	// MOD-BY-TLEE 08/14/2008-TO:

	cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);

	#endif	// MOD-BY-TLEE 08/14/2008-END

	// ADD-BY-TLEE 08/14/2008-BEGIN
	float fTfDomainMin, fTfDomainMax;
	cTransFunc._GetTfDomain(&fTfDomainMin, &fTfDomainMax);
	cDvrWin._SetTfDomain(fTfDomainMin, fTfDomainMax);
	// ADD-BY-TLEE 08/14/2008-END

	cDvrWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);
	cDvrWin._Redisplay();
}
// ADD-BY-TLEE 08/13/2008-END

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
		// DEL-BY-LEETEN 04/05/2012:		_OPTAlign(			"--dvr-nr_of_slices", "-S");
		_OPTAddComment(		"--dvr-nr_of_slices", "Specify #slices.");

	_OPTAddFloatingVector(	"--dvr-thickness_ratio", 1, &cDvrWin.fThicknessRatio, 1.0f);
		// DEL-BY-LEETEN 04/05/2012:		_OPTAlign(			"--dvr-thickness_ratio", "-T");
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

	// MOD-BY-LEETEN 08/09/2010-FROM:
		// assert( BOPTParse(argv, argn, 1, &iNextArg) ); 
	// TO:
	bool bIsOptParsed = BOPTParse(argv, argn, 1, &iNextArg);
	assert( bIsOptParsed ); 
	// MOD-BY-LEETEN 08/09/2010-END

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
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// _ReadVolume();
		// TO:
		assert(iNextArg < argn);
		_ReadVolume(argv[iNextArg]);
		// MOD-BY-LEETEN 01/25/2011-END

		// use the rainbow transfer func. for testing
		pfTransFunc.alloc(CTransFunc::NR_OF_COLORS * iNrOfTfEntries);
		cTransFunc._LoadRainBow();
		cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);
		// ADD-BY-TLEE 08/14/2008-BEGIN
		cTransFunc._SetTfDomain((float)dValueMin, (float)dValueMax);
		// ADD-BY-TLEE 08/14/2008-END

	#endif	// TEST

	// ADD-BY-TLEE 08/13/2008-BEGIN
	_BuildHistogram();
	// ADD-BY-TLEE 08/13/2008-END

										// create the DVR window
	cDvrWin.ICreate("Direct Volume Rendering",	false, 100, 328, 256, 256);
		// MOD-BY-TLEE 2008/08/20-FROM:
			// cDvrWin._SetVolume(&pubVol[0], GL_LUMINANCE, GL_UNSIGNED_BYTE, iXDim, iYDim, iZDim);
		// TO:
		cDvrWin._SetVolume(GL_LUMINANCE, &pubVol[0], GL_LUMINANCE, GL_UNSIGNED_BYTE, iXDim, iYDim, iZDim);
		// MOD-BY-TLEE 2008/08/20-END
		cDvrWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);

		// ADD-BY-TLEE 08/21/2008-BEGIN
		cDvrWin._LoadSavedMatrix();	
		// ADD-BY-TLEE 08/21/2008-END

		// ADD-BY-TLEE 08/21/2008-BEGIN
		cDvrWin._SetDataValue((float)dValueMin, (float)dValueMax);
		cDvrWin._SetTfDomain((float)dValueMin, (float)dValueMax);
		// ADD-BY-TLEE 08/21/2008-END
										// create the Transfer Func. window
	// ADD-BY-TLEE 08/14/2008-BEGIN
	cTfWin._SetTransFunc(&cTransFunc);
	cTfWin._SetNrOfEntries(256);
	// ADD-BY-TLEE 08/14/2008-END

	cTfWin.ICreate("Transfer Function",		false, 100, 100, 256, 128);
	cTfWin._Set();
		cTfWin._KeepUpdateOn();

										// upload the transfer function to the TF window
	// ADD-BY-TLEE 08/13/2008-BEGIN
										// create the TF Editor window
	// ADD-BY-TLEE 08/14/2008-BEGIN
	cTfUi._SetTransFunc(&cTransFunc);
	cTfUi.ICreate("Transfer Function Editor");
		// MOD-BY-TLEE 2008/08/17-FROM:
			// cTfUi._SetHistogramAsBackground(iNrOfTfEntries, &pfHist[0]);
		// TO:
		cTfUi._SetHistogramAsBackground(&pfHist[0], iNrOfTfEntries, dValueMin, dValueMax);
		// MOD-BY-TLEE 2008/08/17-END
		cTfUi._SetUpdateFunc(_UpdateTf);
	// ADD-BY-TLEE 08/13/2008-END

	glutMainLoop();

	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.2  2011/01/25 06:20:27  leeten

[01/25/2011]
1. [ADD] Specify the path of the dataset HIPIP via the argument.

Revision 1.1  2011/01/24 19:41:46  leeten

[01/24/2011]
1. [1ST] First time checkin.

Revision 1.10  2010/08/11 14:17:29  leeten

[08/06/2010]
1. [MOD] Avoid the expression in assert.

Revision 1.9  2008/08/21 15:01:33  leeten

[2008/08/21]
1. [ADD] Include GL/glew.h
2. [CHANGE] Change the data range from [0, 256] to [0, 255]
3. [ADD] Add code to specifty the new TF domain to TransFunc and DVR in _UpdateTf().
4. [CHANGE] Read the volume before creating windows.
5. [CHANGE] When calling CDvrWin::_SetVolume, pass the internal format.
6. [ADD] Load the pre-saved modelview matrix.
7. [ADD] Add code to specifty the new data range to DVR in main().

Revision 1.8  2008/08/17 23:45:30  leeten

[2008/08/17]
1. [ADD] Record the scalar range of the data and pass it to TfUi.

Revision 1.7  2008/08/16 21:38:07  leeten

[2008/08/16]
1. [DEL] Remove old deleted codes.

Revision 1.6  2008/08/16 21:25:51  leeten

[2008/08/16]
1. [CHANGE] Move the setting of the DVR window to the _InitFunc.
2. [CHANGE] Use the method ICreate() to replace the static method GLutWin::_AddWin().
3. [DEL] Remove old deleted code.

Revision 1.5  2008/08/15 14:23:12  leeten

[2008/08/15]
1. [ADD] Set current dir. and filename to be displayed in the TfUi window.
2. [DEL] Remove old deleted code.

Revision 1.4  2008/08/14 23:04:12  leeten

[2008/08/14]
1. [CHANGE] Use the new library LIBDVR to replace the local DvrWin.h/DvrWin.cpp

Revision 1.3  2008/08/14 14:55:23  leeten

[2008/08/18]
1. [CHANGE] Use a new library LIBTFW to replace LIBTF2 to access transfer functions.

Revision 1.2  2008/08/13 21:15:53  leeten

[2008/08/13]
1. [ADD] Include a new window CTwUi to edit transfer functions.
2. [ADD] Build the histogram of the input volume.
3. [ADD] Define a new function _UpdateTf() to update the new transfer function to TfWin and DvrWin.
4. [CHANGE] Use CGlutWin::_Init to replace glutInit.

Revision 1.1.1.1  2008/08/12 16:58:49  leeten

[2008/08/12]
1. First time checkin. This is my DVR renderer for 3D regular volume. For testing, the dataset HIPIP is checkin too.


*/
