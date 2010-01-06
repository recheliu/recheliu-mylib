// ADD-BY-LEETEN 01/05/2010-BEGIN
#include <assert.h>
// ADD-BY-LEETEN 01/05/2010-END

#include "TransFunc.h"

void 
CTransFunc::_LoadRainBow()
{

			// setup the red channel
	plSplines[COLOR_R].clear();
	plSplines[COLOR_R].push_back(CKnot(0.0f, 1.0f));
	plSplines[COLOR_R].push_back(CKnot(1.0f, 0.0f));

			// setup the green channel
	plSplines[COLOR_G].clear();
	plSplines[COLOR_G].push_back(CKnot(0.0f, 0.0f));
	plSplines[COLOR_G].push_back(CKnot(0.5f, 1.0f));
	plSplines[COLOR_G].push_back(CKnot(1.0f, 0.0f));

			// setup the blue channel
	plSplines[COLOR_B].clear();
	plSplines[COLOR_B].push_back(CKnot(0.0f, 0.0f));
	plSplines[COLOR_B].push_back(CKnot(1.0f, 1.0f));

			// setup the alpha channel
	plSplines[COLOR_A].clear();
	#if	0	// MOD-BY-TLEE 2008/08/16-FROM:
		plSplines[COLOR_A].push_back(CKnot(0.0f, 0.2f));
		plSplines[COLOR_A].push_back(CKnot(1.0f, 0.2f));
	#else	// MOD-BY-TLEE 2008/08/16-TO:
	plSplines[COLOR_A].push_back(CKnot(0.0f, 0.5f));
	plSplines[COLOR_A].push_back(CKnot(1.0f, 0.5f));
	#endif	// MOD-BY-TLEE 2008/08/16-END
}

#if	0		// MOD-BY-LEETEN 08/15/2008-FROM:

	void 
	CTransFunc::_Load(char szFilename[])
	{
	}

	void 
	CTransFunc::_Save(char szFilename[])
	{
	}

#else		// MOD-BY-LEETEN 08/15/2008-TO:

										// this method checks the validabilty of the file
										// if the file is valid, return TRUE else FALSE
bool
CTransFunc::BCheckFile(const char szFilename[])
{
	if( !BCheckFilenameExt(szFilename) )
	{
		fprintf(stderr, "Warning in CTransFunc::BCheckFile(): %s contain invalid extension.\n", szFilename);
		return false;
	}

	FILE *fpTf = fopen(szFilename, "rt");
	if( NULL == fpTf )
	{
		fprintf(stderr, "Warning in CTransFunc::BCheckFile(): %s cannot be opened.\n", szFilename);
		return false;
	}

	list<CKnot> plTempSplines[NR_OF_COLORS];

	// ADD-BY-TLEE 08/21/2008-BEGIN
	float fTempMin, fTempMax;
	fscanf(fpTf, "%f\n", &fTempMin);
	fscanf(fpTf, "%f\n", &fTempMax);
	// ADD-BY-TLEE 08/21/2008-END

	for(int c = 0; c < NR_OF_COLORS; c++)
	{
		int n;
		fscanf(fpTf, "%d\n", &n);

		for(int i = 0; i < n; i++)
		{
			CKnot cKnot;
			fscanf(fpTf, "%f %f", &cKnot.fX, &cKnot.fY);
			plTempSplines[c].push_back(cKnot);
		}

		if( 0.0 != plTempSplines[c].begin()->fX ||
			1.0 != plTempSplines[c].rbegin()->fX )
		{
			fprintf(stderr, "Warning s in CTransFunc::BCheckFile(): The splines' ranges are not in [0, 1] .\n", szFilename);
			return false;
		}
	}
	fclose(fpTf);

	return true;
}

										// this method open the specified file 
										// if true, return TRUE else FALSE
bool
CTransFunc::BOpenFile(const char szFilename[])
{
	if( !BCheckFile(szFilename) )
		return false;

	FILE *fpTf = fopen(szFilename, "rt");

	// ADD-BY-LEETEN 08/12/2009-BEGIN
	float fDomainMin, fDomainMax;
	// ADD-BY-LEETEN 08/12/2009-END

	// ADD-BY-TLEE 08/21/2008-BEGIN
	fscanf(fpTf, "%f\n", &fDomainMin);
	fscanf(fpTf, "%f\n", &fDomainMax);
	// ADD-BY-TLEE 08/21/2008-END

	// ADD-BY-LEETEN 08/12/2009-BEGIN
	cDomainMin._SetValue(fDomainMin);
	cDomainMax._SetValue(fDomainMax);
	// ADD-BY-LEETEN 08/12/2009-END

	for(int c = 0; c < NR_OF_COLORS; c++)
	{
		plSplines[c].clear();

		int n;
		fscanf(fpTf, "%d\n", &n);

		for(int i = 0; i < n; i++)
		{
			CKnot cKnot;
			fscanf(fpTf, "%f %f", &cKnot.fX, &cKnot.fY);
			plSplines[c].push_back(cKnot);
		}
	}
	fclose(fpTf);

	return true;
}

										// this method write the four splines to the specified file 
										// if true, return TRUE else FALSE
bool
CTransFunc::BSaveFile(const char szFilename[])
{
	if( !BCheckFilenameExt(szFilename) )
	{
		fprintf(stderr, "Warning in CTransFunc::BOpenFile(): %s contain invalid extension.\n", szFilename);
		return false;
	}

	FILE *fpTf = fopen(szFilename, "wt");
	if( NULL == fpTf )
	{
		fprintf(stderr, "Warning s in CTransFunc::BOpenFile(): %s cannot be opened.\n", szFilename);
		return false;
	}

	#if	0	// MOD-BY-LEETEN 2009/08/13-FROM:
		// ADD-BY-LEETEN 08/12/2009-BEGIN
		float fDomainMin, fDomainMax;
		// ADD-BY-LEETEN 08/12/2009-END
		fDomainMin = cDomainMin.FGetValue();
		fDomainMax = cDomainMax.FGetValue();

		// ADD-BY-TLEE 08/21/2008-BEGIN
		fprintf(fpTf, "%f\n", fDomainMin);
		fprintf(fpTf, "%f\n", fDomainMax);
		// ADD-BY-TLEE 08/21/2008-END
	#else	// MOD-BY-LEETEN 2009/08/13-TO:
	CFloatValue cTemp;
	cTemp._SetValue(cDomainMin.FGetValue());
	fprintf(fpTf, "%fe%d\n", cTemp.fSignificand, (int)cTemp.fExponent);

	cTemp._SetValue(cDomainMax.FGetValue());
	fprintf(fpTf, "%fe%d\n", cTemp.fSignificand, (int)cTemp.fExponent);
	#endif	// MOD-BY-LEETEN 2009/08/13-END

	for(int c = 0; c < NR_OF_COLORS; c++)
	{
		int n = (int)plSplines[c].size();
		fprintf(fpTf, "%d\n", n);
		for(list<CKnot>::iterator 
				viKnot = plSplines[c].begin();
			viKnot != plSplines[c].end();
			viKnot++)
		{
			fprintf(fpTf, "%f %f\n", viKnot->fX, viKnot->fY);
		}
	}

	fclose(fpTf);

	return true;
}

#endif		// MOD-BY-LEETEN 08/15/2008-END


void 
CTransFunc::_ExportColorMap(float pfColorMap[], int iNrOfEntries)
{
	for(int			c = 0; c < CTransFunc::NR_OF_COLORS; c++)
	{
		list<CKnot>::iterator liKnot = plSplines[c].begin();
		list<CKnot>::iterator liPrevKnot;
		for(int		e = 0; e < iNrOfEntries; e++)
		{
			float v = (0.5f + (float)e) / (float)iNrOfEntries;
			for( ; liKnot != plSplines[c].end() && liKnot->fX <= v; liKnot++)
			{
				liPrevKnot = liKnot;
			}

			float fNextX = liKnot->fX;
			float fNextY = liKnot->fY;
			float fPrevX = liPrevKnot->fX;
			float fPrevY = liPrevKnot->fY;

			float fY = (fPrevY * (fNextX - v) + fNextY * (v - fPrevX) ) / (fNextX - fPrevX);
			pfColorMap[e * CTransFunc::NR_OF_COLORS + c] = fY;
		}
	}
}

// ADD-BY-LEETEN 01/05/2010-BEGIN
CTransFunc::CTransFunc()
{
	CKnot cBegin(0.0f, 0.0f), cEnd(1.0f, 0.0f);
	for(int c = 0; c < NR_OF_COLORS; c++)	
	{
		plSplines[c].push_back(cBegin);
		plSplines[c].push_back(cEnd);
	}

	FILE *fpFragShader;
	fpFragShader = fopen("tf1d_frag_func.frag.h", "wt");
	assert(fpFragShader);
	fputs(
		#include "tf1d_frag_func.frag.h"
		,
		fpFragShader);
	fclose(fpFragShader);
}

CTransFunc::~CTransFunc()
{
}

// ADD-BY-LEETEN 01/05/2010-END

/*

$Log: not supported by cvs2svn $
Revision 1.7  2009/08/13 17:26:36  leeten

[2009/08/13]
1. [MOD] Output the range of the transfer function to significand and exponent.

Revision 1.6  2009/08/12 22:11:48  leeten

[2009/08/12]
1. [MOD] Change the range of transfer function's domain and the data's domain from floating point numbers to the structure CFloatValue.

Revision 1.5  2008/08/21 14:50:10  leeten

[2008/08/21]
1. [CHANGE] Add the domain of the TF to the .tf file format.

Revision 1.4  2008/08/17 23:55:58  leeten

[2008/08/17]
1[CHANGE] Fix the typo in the output message when file cannot be opened.

Revision 1.3  2008/08/16 16:39:56  leeten

[2008/08/14]
1. [CHANGE] Set the alpha value of the rainbow TF from 0.2 to 0.5.

Revision 1.2  2008/08/15 14:52:38  leeten

[2008/08/15]
1. [ADD] Add new functionalities to open/save the TF as files.
2. [ADD] Define a new methods BCheckFile to check the validabilty of a file. It is used to prevent from loading wrong TF and destroying the current TF because calling another method BOpenFile can immediately destroy the TF.

Revision 1.1.1.1  2008/08/14 14:44:02  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN]. This library defines classes for trasnfer functions, including editing and displaying.


*/
