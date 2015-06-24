#pragma once
#include "GlutWin.h"

	#include "TransFunc.h"
	#include "libbuf.h"

class CTfWin :
	public CGlutWin
{
	// inhered interfaces
	void _DisplayFunc();
	void _InitFunc();

	CTransFunc *pcTransFunc;
	TBuffer<float> pfColorMap;
	int iNrOfEntries;

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

public:
	void _SetTransFunc(CTransFunc *pcTransFunc);
	void _SetNrOfEntries(int iNrOfEntries);

	void _SetHistogram(float *pfHistogram, int iNrOfHistBins, float fMaxCount, float fMin, float fMax)
	{
		cHistogram._SetHistogram(pfHistogram, iNrOfHistBins, fMaxCount, fMin, fMax);
	}

public:
	CTfWin(void);
public:
	~CTfWin(void);
};

