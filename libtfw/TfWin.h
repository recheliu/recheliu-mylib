#pragma once
#include "GlutWin.h"

	// ADD-BY-TLEE 08/14/2008-BEGIN
	#include "TransFunc.h"
	#include "libbuf.h"
	// ADD-BY-TLEE 08/14/2008-END

class CTfWin :
	public CGlutWin
{
	// inhered interfaces
	void _DisplayFunc();
	void _InitFunc();

	// MOD-BY-TLEE 08/14/2008-FROM:
		// GLuint iLid;
	// TO:
	CTransFunc *pcTransFunc;
	TBuffer<float> pfColorMap;
	int iNrOfEntries;
	// MOD-BY-TLEE 08/14/2008-END

	// ADD-BY-LEETEN 2009/08/16-BEGIN
	struct CHistogram{
		TBuffer<float> pfBins;
		CFloatValue cMin, cMax;
		float fMaxCount;

		void _SetHistogram(float *pfHistogram, int iNrOfHistBins, float fMaxCount, float fMin, float fMax)
		{
			pfBins.alloc(iNrOfHistBins);
			memcpy(&pfBins[0], &pfHistogram[0], sizeof(pfBins[0]) * pfBins.USize());
			cMin._SetValue(fMin);
			cMax._SetValue(fMax);
			this->fMaxCount = fMaxCount;
		}
	};
	CHistogram cHistogram;
	// ADD-BY-LEETEN 2009/08/16-END

public:
	// MOD-BY-TLEE 08/14/2008-FROM:
		// void _SetTransferFunc(const float *pfTf, int iNrOfTfEntries);
	// TO:
	void _SetTransFunc(CTransFunc *pcTransFunc);
	void _SetNrOfEntries(int iNrOfEntries);
	// MOD-BY-TLEE 08/14/2008-END

	// ADD-BY-LEETEN 2009/08/16-BEGIN
	void _SetHistogram(float *pfHistogram, int iNrOfHistBins, float fMaxCount, float fMin, float fMax)
	{
		cHistogram._SetHistogram(pfHistogram, iNrOfHistBins, fMaxCount, fMin, fMax);
	}
	// ADD-BY-LEETEN 2009/08/16-END

public:
	CTfWin(void);
public:
	~CTfWin(void);
};

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/14 14:44:02  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN]. This library defines classes for trasnfer functions, including editing and displaying.

Revision 1.1.1.1  2008/08/12 16:58:49  leeten

[2008/08/12]
1. First time checkin. This is my DVR renderer for 3D regular volume. For testing, the dataset HIPIP is checkin too.


*/

