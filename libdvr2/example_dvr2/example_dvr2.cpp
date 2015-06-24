///////////////////////////////////////////////////////////////////////
// headers
	#include <GL/glew.h>

	#include <stdio.h>

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
	static 	float fMaxCount;		// max. count in the histogram

					// buffer of the histogram
	TBuffer<float> pfTransFunc;

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
				iTfEntry = min(max(iTfEntry, 0), iNrOfTfEntries - 1);	
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
	return;
}

#else	// #if		WITH_NRRD

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

									// this value is for hipip only!
	dValueMin = +HUGE_VAL;
	dValueMax = -HUGE_VAL;
	for(int i = 0, 	z = 0; z < iZDim; z++)
		for(int 	y = 0; y < iYDim; y++)
			for(int x = 0; x < iXDim; x++, i++)
			{
				if( (float)HUGE_VAL == p3DfVol[i] )
					continue;

				double dValue = (double)p3DfVol[i];
				dValueMin = min(dValueMin, dValue);
				dValueMax = max(dValueMax, dValue);
			}

	// normalize the value range
	for(int i = 0, 	z = 0; z < iZDim; z++)
		for(int 	y = 0; y < iYDim; y++)
			for(int x = 0; x < iXDim; x++, i++)
			{
				if( (float)HUGE_VAL == p3DfVol[i] )
				{
					p3DfVol[i] = 0.0f;
					continue;
				}

				double dValue = (double)p3DfVol[i];
				p3DfVol[i] = (dValue - dValueMin)/(dValueMax - dValueMin);
			}

	void _BuildHistogram();
	_BuildHistogram();
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
				if( (float)HUGE_VAL == fV )
					continue;
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
#endif		// #if		WITH_NRRD	

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
		cTransFunc._SetTfDomain((float)dValueMin, (float)dValueMax);

										// create the Transfer Func. window
	cTfWin._SetTransFunc(&cTransFunc);
	cTfWin._SetNrOfEntries(256);

	cTfWin.ICreate("Transfer Function",		false, 100, 100, 256, 128);
	cTfWin._Set();
		cTfWin._SetHistogram(&pfHist[0], iNrOfTfEntries, ::fMaxCount, dValueMin, dValueMax);
		cTfWin._KeepUpdateOn();

										// upload the transfer function to the TF window
										// create the TF Editor window
	cTfUi._SetTransFunc(&cTransFunc);
	cTfUi.ICreate("Transfer Function Editor");
		cTfUi._SetHistogramAsBackground(&pfHist[0], iNrOfTfEntries, dValueMin, dValueMax);
		cTfUi._SetUpdateFunc(_UpdateTf);
										// create the DVR window
	cDvrWin.ICreate("Direct Volume Rendering"); 
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
		cDvrWin._SetVolume(GL_LUMINANCE, &p3DfVol[0], GL_LUMINANCE, GL_FLOAT, iXDim, iYDim, iZDim);
	#endif	// #if 	WITH_NRRD	
		cDvrWin._SetTransferFunc(&pfTransFunc[0], GL_RGBA, GL_FLOAT, iNrOfTfEntries);
		cDvrWin._LoadSavedMatrix();		
		cDvrWin._SetDataValue((float)dValueMin, (float)dValueMax);
		cDvrWin._SetTfDomain((float)dValueMin, (float)dValueMax);

	glutMainLoop();

	return 0;
}

