#include <assert.h>

#include "TransFunc.h"

void 
CTransFunc::_LoadRainBow()
{
			// setup the red channel
			// setup the blue channel
	plSplines[COLOR_B].clear();
	plSplines[COLOR_B].push_back(CKnot(0.0f, 1.0f));
	plSplines[COLOR_B].push_back(CKnot(1.0f, 0.0f));

			// setup the green channel
	plSplines[COLOR_G].clear();
	plSplines[COLOR_G].push_back(CKnot(0.0f, 0.0f));
	plSplines[COLOR_G].push_back(CKnot(0.5f, 1.0f));
	plSplines[COLOR_G].push_back(CKnot(1.0f, 0.0f));

			// setup the red channel
	plSplines[COLOR_R].clear();
	plSplines[COLOR_R].push_back(CKnot(0.0f, 0.0f));
	plSplines[COLOR_R].push_back(CKnot(1.0f, 1.0f));
			// setup the alpha channel
	plSplines[COLOR_A].clear();
	plSplines[COLOR_A].push_back(CKnot(0.0f, 0.5f));
	plSplines[COLOR_A].push_back(CKnot(1.0f, 0.5f));
}

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

	float fTempMin, fTempMax;
	fscanf(fpTf, "%f\n", &fTempMin);
	fscanf(fpTf, "%f\n", &fTempMax);

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

	float fDomainMin, fDomainMax;

	fscanf(fpTf, "%f\n", &fDomainMin);
	fscanf(fpTf, "%f\n", &fDomainMax);

	cDomainMin._SetValue(fDomainMin);
	cDomainMax._SetValue(fDomainMax);

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

	CFloatValue cTemp;
	cTemp._SetValue(cDomainMin.FGetValue());
	fprintf(fpTf, "%fe%d\n", cTemp.fSignificand, (int)cTemp.fExponent);

	cTemp._SetValue(cDomainMax.FGetValue());
	fprintf(fpTf, "%fe%d\n", cTemp.fSignificand, (int)cTemp.fExponent);

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

