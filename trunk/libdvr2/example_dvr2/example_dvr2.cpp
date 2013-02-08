///////////////////////////////////////////////////////////////////////
// headers
	#include <GL/glew.h>

	#include <stdio.h>

	// ADD-By-LEETEN 10/21/2011-BEGIN
	#if		WITH_NRRD
	#include "NrrdIO.h"
	#ifdef WIN32
		#if		defined(_DEBUG)
		#pragma comment (lib, "ITKNrrdIOd.lib")      
		#pragma comment (lib, "zlibd.lib")      // link with my own library libfps
		#else	// #if	defined(_DEBUG)
		#pragma comment (lib, "ITKNrrdIO.lib")     
		#pragma comment (lib, "zlib.lib")      // link with my own library libfps
		#endif	// #if	defined(_DEBUG)
	#endif	// #ifdef	WIN32
	#endif	// #if		WITH_NRRD
	// ADD-By-LEETEN 10/21/2011-END

	
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
	TBuffer3D<float> p3DfVol;
	double dValueMin, dValueMax;

					// histogram of the buffer
	TBuffer<float> pfHist;
	// ADD-BY-LEETEN 07/05/2011-BEGIN
	static 	float fMaxCount;		// max. count in the histogram
	// ADD-BY-LEETEN 07/05/2011-END

					// buffer of the histogram
	TBuffer<float> pfTransFunc;

// ADD-BY-LEETEN 10/21/2011-BEGIN
#if		WITH_NRRD
	Nrrd *nin;

template<typename T>
void 
_BuildHistogram()
{	
	T *data = (T*)nin->data;	

	// search for the range
	dValueMin = HUGE_VAL;
	dValueMax = -HUGE_VAL;
	for(int v = 0,	z = 0; z < (int)nin->axis[2].size; z++)
		for(int		y = 0; y < (int)nin->axis[1].size; y++)
			for(int x = 0; x < (int)nin->axis[0].size; x++, v++)
			{
				dValueMin = min(dValueMin, (double)data[v]);
				dValueMax = max(dValueMax, (double)data[v]);
			}
	LOG_VAR(dValueMin);
	LOG_VAR(dValueMax);

	pfHist.alloc(iNrOfTfEntries);
	for(int i = 0,	z = 0; z < (int)nin->axis[2].size; z++)
		for(int		y = 0; y < (int)nin->axis[1].size; y++)
			for(int x = 0; x < (int)nin->axis[0].size; x++, i++)
			{
				int iTfEntry = (int)ceil((double)(iNrOfTfEntries - 1) * (double)(data[i] - dValueMin) / (double)(dValueMax - dValueMin));
				iTfEntry = min(max(iTfEntry, 0), iNrOfTfEntries - 1);	// ADD-BY-LEETEN 01/11/2013
				pfHist[iTfEntry] += 1.0f;
			}

	// normalize the histogram
	fMaxCount = -(float)HUGE_VAL;
	for(int b = 0; b < iNrOfTfEntries; b++) 
		fMaxCount = max(fMaxCount, pfHist[b]);

	for(int b = 0; b < iNrOfTfEntries; b++) 
		pfHist[b] /= fMaxCount;
}

void
_ReadVolume(char* szPathFilename)
{
	/* create a nrrd; at this point this is just an empty container */
	nin = nrrdNew();

	if (nrrdLoad(nin, szPathFilename, NULL)) {
		char *err = biffGetDone(NRRD);
		LOG_ERROR(fprintf(stderr, "%s", err));
		free(err);
		return;
	}
	switch(nin->type)
	{
	case nrrdTypeUChar:	_BuildHistogram<unsigned char>();	break;
	case nrrdTypeChar:	_BuildHistogram<char>();			break;
	case nrrdTypeShort:	_BuildHistogram<short>();			break;
	case nrrdTypeUShort:_BuildHistogram<unsigned short>();	break;
	case nrrdTypeInt:	_BuildHistogram<int>();				break;
	case nrrdTypeUInt:	_BuildHistogram<unsigned int>();	break;
	case nrrdTypeFloat:	_BuildHistogram<float>();			break;

	default:
		break;
	}

	// ADD-BY-LEETEN 01/11/2013-BEGIN
	if( nrrdTypeFloat == nin->type )
	{
		float *data = (float*)nin->data;	

		for(int v = 0,	z = 0; z < (int)nin->axis[2].size; z++)
			for(int		y = 0; y < (int)nin->axis[1].size; y++)
				for(int x = 0; x < (int)nin->axis[0].size; x++, v++)
					data[v] = (data[v] - (float)dValueMin)/(float)(dValueMax - dValueMin);

		LOG_VAR(dValueMin);
		LOG_VAR(dValueMax);
	}
	// ADD-BY-LEETEN 01/11/2013-END
	return;
}

#else	// #if		WITH_NRRD
// ADD-BY-LEETEN 10/21/2011-END

void
_ReadVolume(char* szPathFilename)
{
	// read the volume from file
	FILE *fpData;
	fpData = fopen(szPathFilename, "rb");		// load the hipip dataset for test

	ASSERT_OR_LOG(fpData, perror(szPathFilename));
	fread(&iXDim, sizeof(iXDim), 1, fpData);
	fread(&iYDim, sizeof(iYDim), 1, fpData);
	fread(&iZDim, sizeof(iZDim), 1, fpData);
	LOG_VAR(iXDim);
	LOG_VAR(iYDim);
	LOG_VAR(iZDim);
	p3DfVol.alloc(iXDim, iYDim, iZDim);
	fread(&p3DfVol[0], sizeof(p3DfVol[0]), iXDim * iYDim * iZDim, fpData);
	fclose(fpData);

	// ADD-BY-TLEE 2008/08/17-BEGIN
									// this value is for hipip only!
	dValueMin = +HUGE_VAL;
	dValueMax = -HUGE_VAL;
	for(int i = 0, 	z = 0; z < iZDim; z++)
		for(int 	y = 0; y < iYDim; y++)
			for(int x = 0; x < iXDim; x++, i++)
			{
				// ADD-BY-LEETEN 02/23/2012-BEGIN
				if( (float)HUGE_VAL == p3DfVol[i] )
					continue;
				// ADD-BY-LEETEN 02/23/2012-END

				double dValue = (double)p3DfVol[i];
				dValueMin = min(dValueMin, dValue);
				dValueMax = max(dValueMax, dValue);
			}

	// normalize the value range
	for(int i = 0, 	z = 0; z < iZDim; z++)
		for(int 	y = 0; y < iYDim; y++)
			for(int x = 0; x < iXDim; x++, i++)
			{
				// ADD-BY-LEETEN 02/23/2012-BEGIN
				if( (float)HUGE_VAL == p3DfVol[i] )
				{
					p3DfVol[i] = 0.0f;
					continue;
				}
				// ADD-BY-LEETEN 02/23/2012-END

				double dValue = (double)p3DfVol[i];
				p3DfVol[i] = (dValue - dValueMin)/(dValueMax - dValueMin);
			}
	// ADD-BY-TLEE 2008/08/17-END

	// ADD-By-LEETEN 10/21/2011-BEGIN
	void _BuildHistogram();
	_BuildHistogram();
	// ADD-By-LEETEN 10/21/2011-END
}

void
_BuildHistogram()
{
	pfHist.alloc(iNrOfTfEntries);
	for(int i = 0,	z = 0; z < iZDim; z++)
		for(int		y = 0; y < iYDim; y++)
			for(int x = 0; x < iXDim; x++, i++)
			{
				float fV = p3DfVol[i];
				// ADD-BY-LEETEN 02/23/2012-BEGIN
				if( (float)HUGE_VAL == fV )
					continue;
				// ADD-BY-LEETEN 02/23/2012-END
				int iEntry = (int)((float)iNrOfTfEntries * fV);
				iEntry = min(max(iEntry, 0), iNrOfTfEntries - 1);
				pfHist[iEntry] += 1.0f;
			}

	// normalize the histogram
	::fMaxCount = -HUGE_VAL;
	for(int b = 0; b < iNrOfTfEntries; b++) 
		fMaxCount = max(fMaxCount, pfHist[b]);

	for(int b = 0; b < iNrOfTfEntries; b++) 
		pfHist[b] /= fMaxCount;
}
#endif		// #if		WITH_NRRD	// ADD-BY-LEETEN 10/21/2011

/////////////////////////////////////////////////////////
// ADD-BY-TLEE 08/13/2008-BEGIN
void
_UpdateTf()
{
	cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);

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

	bool bIsOptParsed = BOPTParse(argv, argn, 1, &iNextArg);
	assert(bIsOptParsed);

	atexit(quit);

	// load the transfer func. and data set
		// load the data set
		assert(iNextArg < argn);
		_ReadVolume(argv[iNextArg]);

		// use the rainbow transfer func. for testing
		pfTransFunc.alloc(CTransFunc::NR_OF_COLORS * iNrOfTfEntries);
		cTransFunc._LoadRainBow();
		cTransFunc._ExportColorMap(&pfTransFunc[0], iNrOfTfEntries);
		// ADD-BY-TLEE 08/14/2008-BEGIN
		cTransFunc._SetTfDomain((float)dValueMin, (float)dValueMax);
		// ADD-BY-TLEE 08/14/2008-END

										// create the Transfer Func. window
	// ADD-BY-TLEE 08/14/2008-BEGIN
	cTfWin._SetTransFunc(&cTransFunc);
	cTfWin._SetNrOfEntries(256);
	// ADD-BY-TLEE 08/14/2008-END

	cTfWin.ICreate("Transfer Function",		false, 100, 100, 256, 128);
	cTfWin._Set();
		// ADD-BY-LEETEN 07/05/2011-BEGIN
		cTfWin._SetHistogram(&pfHist[0], iNrOfTfEntries, ::fMaxCount, dValueMin, dValueMax);
		// ADD-BY-LEETEN 07/05/2011-END
		cTfWin._KeepUpdateOn();

										// upload the transfer function to the TF window
	// ADD-BY-TLEE 08/13/2008-BEGIN
										// create the TF Editor window
	// ADD-BY-TLEE 08/14/2008-BEGIN
	cTfUi._SetTransFunc(&cTransFunc);
	cTfUi.ICreate("Transfer Function Editor");
		cTfUi._SetHistogramAsBackground(&pfHist[0], iNrOfTfEntries, dValueMin, dValueMax);
		cTfUi._SetUpdateFunc(_UpdateTf);
	// ADD-BY-TLEE 08/13/2008-END

	// ADD-BY-LEETEN 01/26/2011-BEGIN
										// create the DVR window
	cDvrWin.ICreate("Direct Volume Rendering"); 
	// ADD-BY-LEETEN 10/21/2011-BEGIN
	#if 	WITH_NRRD	
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

		cDvrWin._SetVolume(GL_LUMINANCE32F_ARB, nin->data, iType, GL_LUMINANCE, (int)nin->axis[0].size, (int)nin->axis[1].size, (int)nin->axis[2].size);
	#else	// #if 	WITH_NRRD	
	// ADD-BY-LEETEN 10/21/2011-END
		cDvrWin._SetVolume(GL_LUMINANCE, &p3DfVol[0], GL_LUMINANCE, GL_FLOAT, iXDim, iYDim, iZDim);
	#endif	// #if 	WITH_NRRD	// ADD-BY-LEETEN 10/21/2011
		cDvrWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);
		cDvrWin._LoadSavedMatrix();		
		cDvrWin._SetDataValue((float)dValueMin, (float)dValueMax);
		cDvrWin._SetTfDomain((float)dValueMin, (float)dValueMax);
	// ADD-BY-LEETEN 01/26/2011-END

	glutMainLoop();

	return 0;
}

/*

$Log: not supported by cvs2svn $
Revision 1.4  2011-07-06 03:37:57  leeten
[07/15/2011]
1. [MOD] Change the program to read the file in .b3d format (3 integers for X, Y, Z length and then the raw data in single type.)

Revision 1.3  2011-01-27 02:43:33  leeten

[01/26/2011]
1. [MOD] Move the creation of the DVR windows after the creation of transfer func. windows.

Revision 1.2  2011/01/25 06:21:00  leeten

[01/25/2011]
1. [ADD] Specify the path of the dataset HIPIP via the argument.

Revision 1.1  2011/01/24 19:42:37  leeten

[01/24/2011]
1. [1ST] First time checkin.

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