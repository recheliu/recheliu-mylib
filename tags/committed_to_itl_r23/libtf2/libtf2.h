#ifndef __LIB_TRANSFER__FUNCTION_2_H__
#define __LIB_TRANSFER__FUNCTION_2_H__

#include <iostream>
#include <vector>
using namespace std;

#define INTERP(d, dmin, dmax, vmin, vmax)	((vmin) + ((vmax) - (vmin)) * ((d) - (dmin)) / ((dmax) - (dmin))) 

typedef struct CColorAlpha{
	double pdColor[3];
	double dAlpha;

	CColorAlpha() 
	{
		pdColor[0] = pdColor[1] = pdColor[2] = 0.0;
		dAlpha = 0.0;
	}

	void
	_AssignToFloatArray(float *pf)
	{
		pf[0] = (float)pdColor[0];
		pf[1] = (float)pdColor[1];
		pf[2] = (float)pdColor[2];
		pf[3] = (float)dAlpha;
	}
} CColorAlpha;

CColorAlpha CInterpolate(double dV, double dLeft, double dRight, CColorAlpha cLeft, CColorAlpha cRight);
CColorAlpha CFrontToBackComposition(CColorAlpha cFront, CColorAlpha cBack);
// ADD-BY-LEETEN 04/03/2007-BEGIN
CColorAlpha CAssociateColor(const CColorAlpha& cCa);
// ADD-BY-LEETEN 04/03/2007-END

typedef struct CColorAlphaEntry{
	double dScalar;
	CColorAlpha cColorAlpha;

	CColorAlphaEntry() {
		dScalar = 0.0;
	}
	CColorAlphaEntry(double dS, double dR, double dG, double dB, double dA) {
		dScalar = dS;
		cColorAlpha.pdColor[0] = dR;
		cColorAlpha.pdColor[1] = dG;
		cColorAlpha.pdColor[2] = dB;
		cColorAlpha.dAlpha = dA;
	}
} CColorAlphaEntry;

typedef vector<CColorAlphaEntry>::iterator vcaIterator;
typedef vector<CColorAlphaEntry>::reverse_iterator vcaRiterator;


class CTransferFunction 
{
	vector <CColorAlphaEntry> vcFunction;

	double dScalarRangeMin, dScalarRangeMax;
	double dSampleDistnace;

	float *pfLookupTable;
	float *pfPreIntegratedTable;

public:

	CTransferFunction () 
	{
		dSampleDistnace = -1.0;	// if it is negative, it is ignored
		pfLookupTable = NULL;
		pfPreIntegratedTable = NULL;
	}

	void _Clear()
	{
		if ( pfLookupTable )
		{
			delete [] pfLookupTable;
			pfLookupTable = NULL;
		}

		if( pfPreIntegratedTable )
		{
			delete [] pfPreIntegratedTable;
			pfPreIntegratedTable = NULL;
		}
	}
	
	~CTransferFunction () 
	{
		/*
		if ( pfLookupTable )
			delete [] pfLookupTable;

		if( pfPreIntegratedTable )
			delete [] pfPreIntegratedTable;
		*/
		_Clear();
	}

	// load the transfer function 
	bool BLoad(const char* szColorMapPathFilename, const char* szOpacityMapPathFilename, double dMinScalar = 0.0, double dMaxScalar=1.0);

	bool BLoadCmap(const char *szCmapPathFilename, double dMinScalar = 0.0, double dMaxScalar=1.0);

	CColorAlpha CIntegrate(double dFrontValue, double dBackValue, double dIntervalDepth);

	// find the entry e s.t e.s < dScalar < (e+1).s
	vcaIterator CFindEntry(double dScalar);

	// find the entry e s.t (e-1).s < dScalar < e.s
	vcaRiterator CFindEntryReversely(double dScalar);

	const float* PFGet1DLookupTable(size_t uNrOfEntries, double dScalarMin, double dScalarMax); 
	/*
	GLuint ULoad1DTexture()
	{
		GLuint uTidTf = 0;
		glGenTextures(1, &uTidTf);
		glBindTexture(GL_TEXTURE_1D, uTidTf);
		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, uNrOf1DEntries, 0, GL_RGBA, GL_FLOAT, pfLookupTable);

		GLint iError;
		if( (iError = glGetError()) )
		{			
			fprintf(stderr, "Error in CTransferFunction::ULoad1DTexture(): %s\n", gluErrorString(iError));
			return 0;
		}
		return uTidTf;
	}
	*/

	const float* PFGet3DPreIntegratedTable(size_t uNrOfFrontValues, size_t uNrOfBackValues, size_t uNrOfDepthIntervals, double dScalarMin, double dScalarMax, double dDepthScale);
};

#endif	// __LIB_TRANSFER__FUNCTION_2_H__

/*

$Log: not supported by cvs2svn $
Revision 1.1  2007/03/12 23:44:40  leeten

[03/12/2007]
1. First time checkin. This is a new version of transfer function library.


*/
