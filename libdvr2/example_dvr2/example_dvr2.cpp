///////////////////////////////////////////////////////////////////////
// headers
	#include <GL/glew.h>

	#include <stdio.h>

	// my own program argument parser
	#include "libopt.h"

	// my own template library for allocating buffers
	#include "libbuf3d.h"

	#include "DvrSampleWin.h"

	#include "libtfw/TfWin.h"
	#include "libtfw/TfUi.h"	
	#include "libtfw/TransFunc.h"

///////////////////////////////////////////////////////////////////////
// variables

	// variable for the transfer func.
	int iNrOfTfEntries = 256;
	CTransFunc cTransFunc;

					// the GLUT wins
	CDvrSampleWin cDvrWin;

	CTfWin	cTfWin;
	CTfUi	cTfUi;	

					// buffer for the 3D volume
	int iXDim, iYDim, iZDim;
	TBuffer3D<unsigned char> p3DubVol;
	double dValueMin, dValueMax;

					// histogram of the buffer
	TBuffer<float> pfHist;

					// buffer of the histogram
	TBuffer<float> pfTransFunc;

void
_ReadVolume()
{
	// read the volume from file
	FILE *fpData;
	fpData = fopen("../data/hipip", "rb");		// load the hipip dataset for test
	assert(fpData);

	fscanf(fpData, "%d %d %d\n", &iXDim, &iYDim, &iZDim);
	fseek(fpData, 9, SEEK_SET);

	p3DubVol.alloc(iXDim, iYDim, iZDim);

	fread(&p3DubVol[0], sizeof(p3DubVol[0]), iXDim * iYDim * iZDim, fpData);

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
				pfHist[p3DubVol[i]] += 1.0f;

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

	// MOD-BY-LEETEN 08/09/2010-FROM:
		// assert( BOPTParse(argv, argn, 1, &iNextArg) ); 
	// TO:
	bool bIsOptParsed = BOPTParse(argv, argn, 1, &iNextArg);
	assert(bIsOptParsed);
	// MOD-BY-LEETEN 08/09/2010-END

	atexit(quit);

	// load the transfer func. and data set
	#if 0	// MOD-BY-LEETEN 09/10/2010-FROM:
		if( argn - iNextArg < 4 )
		{
			fprintf(stderr, "Usage: %s [<options>] <data.vol> <trasnfer_function.txt>\n", argv[0]);
			return 0;
		}

	#else	// MOD-BY-LEETEN 09/10/2010-FROM:
		// load the data set
		_ReadVolume();

		// use the rainbow transfer func. for testing
		pfTransFunc.alloc(CTransFunc::NR_OF_COLORS * iNrOfTfEntries);
		cTransFunc._LoadRainBow();
		cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);
		// ADD-BY-TLEE 08/14/2008-BEGIN
		cTransFunc._SetTfDomain((float)dValueMin, (float)dValueMax);
		// ADD-BY-TLEE 08/14/2008-END

	#endif	// MOD-BY-LEETEN 09/10/2010-END

	// ADD-BY-TLEE 08/13/2008-BEGIN
	_BuildHistogram();
	// ADD-BY-TLEE 08/13/2008-END

										// create the DVR window
	cDvrWin.ICreate("Direct Volume Rendering",	false, 100, 328, 256, 256);
		cDvrWin._SetVolume(GL_LUMINANCE, &p3DubVol[0], GL_LUMINANCE, GL_UNSIGNED_BYTE, iXDim, iYDim, iZDim);
		cDvrWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);

		cDvrWin._LoadSavedMatrix();	
		cDvrWin._SetDataValue((float)dValueMin, (float)dValueMax);
		cDvrWin._SetTfDomain((float)dValueMin, (float)dValueMax);
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
Revision 1.3  2010/09/10 14:07:50  leeten

[09/10/2010]
1. [MOD] Remove the TEST tag.

Revision 1.2  2010/08/11 14:16:40  leeten

[08/06/2010]
1. [MOD] Avoid the expression in assert.

Revision 1.1  2010/04/06 20:01:15  leeten

[04/06/2010]
1. [1ST] First time checkin.


*/
