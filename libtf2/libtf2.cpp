#include "stdafx.h"
#include "libtf2.h"

#define SAMPLING_FREQUENCIES	256
#define SUPER_SAMPLING_FACTOR	16

// ADD-BY-LEETEN 04/03/2007-BEGIN
CColorAlpha
CAssociateColor(const CColorAlpha& cCa)
{
	CColorAlpha cColorAlpha;
	for(size_t c=0; c<3; c++)
		cColorAlpha.pdColor[c] = cCa.pdColor[c] * cCa.dAlpha;
	cColorAlpha.dAlpha = cCa.dAlpha;

	return cColorAlpha;
}
// ADD-BY-LEETEN 04/03/2007-END


CColorAlpha
CInterpolate(double dV, double dLeft, double dRight, CColorAlpha cLeft, CColorAlpha cRight)
{
	CColorAlpha cColorAlpha;
	cColorAlpha.dAlpha = INTERP(dV, dLeft, dRight, cLeft.dAlpha, cRight.dAlpha);
	for(size_t c=0; c<3; c++)
		cColorAlpha.pdColor[c] = INTERP(dV, dLeft, dRight, cLeft.pdColor[c], cRight.pdColor[c]);

	return cColorAlpha;
}

CColorAlpha
CFrontToBackComposition(CColorAlpha cFront, CColorAlpha cBack)
{
	CColorAlpha cColorAlpha;
	for(size_t c=0; c<3; c++)
		// MOD-BY-LEETEN 04/03/2007-BEGIN
		/* FROM: cColorAlpha.pdColor[c] = cFront.pdColor[c] + (1.0 - cFront.dAlpha) * cBack.dAlpha * cBack.pdColor[c]; TO: */
		cColorAlpha.pdColor[c] = cFront.pdColor[c] + (1.0 - cFront.dAlpha) * cBack.pdColor[c]; 
		// MOD-BY-LEETEN 04/03/2007-END
	cColorAlpha.dAlpha = 1.0 - (1.0 - cFront.dAlpha) * (1.0 - cBack.dAlpha);

	return cColorAlpha;
}

typedef struct CColor3D {
	double dScalar;
	double pdColor[3];
}CColor3D;

typedef struct CAlpha {
	double dScalar;
	double dAlpha;
}CAlpha ;

bool 
CTransferFunction::BLoad(
	const char* szColorMapPathFilename, 
	const char* szOpacityMapPathFilename, 
	double dMinScalar, double dMaxScalar)
{
	FILE *fpColor, *fpOpacity;

	static char szBuf[1024+1];
	vector<CColor3D>	vcColors;
	vector<CAlpha>		vcOpacities;

	if( !szColorMapPathFilename || !szOpacityMapPathFilename )
	{
		// Default rainbow colormap
		double dRange = dMaxScalar - dMinScalar;
		dScalarRangeMin = dMinScalar - dRange / 100.0;
		dScalarRangeMax = dMaxScalar + dRange / 100.0;
		CColorAlphaEntry red(		dMinScalar - dRange/100.0,			1.0, 0.0, 0.0, 0.2);
		CColorAlphaEntry yellow(	1.0 * dRange / 5.0 + dMinScalar,	1.0, 1.0, 0.0, 0.2);
		CColorAlphaEntry green(		2.0 * dRange / 5.0 + dMinScalar,	0.0, 1.0, 0.0, 0.2);
		CColorAlphaEntry cyan(		3.0 * dRange / 5.0 + dMinScalar,	1.0, 1.0, 1.0, 0.2);
		CColorAlphaEntry blue(		4.0 * dRange / 5.0 + dMinScalar,	1.0, 0.0, 1.0, 0.2);
		CColorAlphaEntry magenta(	dMaxScalar + dRange/100.0,			1.0, 0.0, 1.0, 0.2);
		vcFunction.push_back(red);
		vcFunction.push_back(yellow);
		vcFunction.push_back(green);
		vcFunction.push_back(cyan);
		vcFunction.push_back(blue);
		vcFunction.push_back(magenta);
	}
	else
	{
		// read the color map
		fopen_s(&fpColor, szColorMapPathFilename, "r+t");
		if( !fpColor )
			return false;

		// read the alpha map
		fopen_s(&fpOpacity, szOpacityMapPathFilename, "r+t");

		if( !fpOpacity )
			return false;

		fgets(szBuf, sizeof(szBuf)-1, fpColor);
		while( true ) 
		{
			float pf[4];
			if( 4 != fscanf_s(fpColor, "%f %f %f %f\n", &pf[0], &pf[1], &pf[2], &pf[3]) )
				break;
			
			CColor3D cColor;
			cColor.dScalar		= (double)pf[0];
			cColor.pdColor[0]	= (double)pf[1];
			cColor.pdColor[1]	= (double)pf[2];
			cColor.pdColor[2]	= (double)pf[3];
			vcColors.push_back(cColor);
		}
		fclose(fpColor);

		fgets(szBuf, sizeof(szBuf)-1, fpOpacity);
		while( true ) 
		{
			float pf[2];
			if( 2 != fscanf_s(fpOpacity, "%f %f\n", &pf[0], &pf[1]) )
				break;
			CAlpha cAlpha;
			cAlpha.dScalar	= (double)pf[0];
			cAlpha.dAlpha	= (double)pf[1];
			vcOpacities.push_back(cAlpha);
		}
		fclose(fpOpacity);

		// construct the color/alpha lookup table 
		vector<CColor3D>::iterator	pvcColor;
		vector<CAlpha>::iterator	pvcAlpha;

		pvcColor = vcColors.begin();
		pvcAlpha = vcOpacities.begin();

		CColorAlphaEntry cEntry;
		if( pvcColor->dScalar < pvcAlpha->dScalar ) 
		{
			cEntry.dScalar = pvcColor->dScalar;
			memcpy(cEntry.cColorAlpha.pdColor, pvcColor->pdColor, sizeof(cEntry.cColorAlpha.pdColor));
			pvcColor++;
		} 
		else
		if( pvcColor->dScalar > pvcAlpha->dScalar ) 
		{
			cEntry.dScalar = pvcAlpha->dScalar;
			cEntry.cColorAlpha.dAlpha = pvcAlpha->dAlpha;
			pvcAlpha++;
		}
		else
		{
			cEntry.dScalar = pvcAlpha->dScalar;
			memcpy(cEntry.cColorAlpha.pdColor, pvcColor->pdColor, sizeof(cEntry.cColorAlpha.pdColor));
			cEntry.cColorAlpha.dAlpha = pvcAlpha->dAlpha;
			pvcColor++;
			pvcAlpha++;
		}

		vcFunction.push_back(cEntry);
		dScalarRangeMin = cEntry.dScalar;

		while( pvcAlpha != vcOpacities.end() || pvcColor != vcColors.end() )
		{
			if( pvcColor->dScalar < pvcAlpha->dScalar ) 
			{
				// use the color
				memcpy(cEntry.cColorAlpha.pdColor, pvcColor->pdColor, sizeof(cEntry.cColorAlpha.pdColor));

				// interpolate the alpha based on scalar field
				cEntry.cColorAlpha.dAlpha = INTERP(
					pvcColor->dScalar, cEntry.dScalar, pvcAlpha->dScalar, 
					cEntry.cColorAlpha.dAlpha, pvcAlpha->dAlpha);

				// update the scalar to the new one
				cEntry.dScalar = pvcColor->dScalar;

				pvcColor++;
			} 
			else
			if( pvcColor->dScalar > pvcAlpha->dScalar ) 
			{
				// use the alpha
				cEntry.cColorAlpha.dAlpha = pvcAlpha->dAlpha;

				// interpolate the alpha based on scalar field
				for(size_t c=0; c<3; c++) 
					cEntry.cColorAlpha.pdColor[c] = INTERP(
						pvcAlpha->dScalar, cEntry.dScalar, pvcColor->dScalar, 
						cEntry.cColorAlpha.pdColor[c], pvcColor->pdColor[c]);

				// update the scalar 
				cEntry.dScalar = pvcAlpha->dScalar;

				pvcAlpha++;
			}
			else
			{
				// use the alpha
				cEntry.cColorAlpha.dAlpha = pvcAlpha->dAlpha;

				// use the color
				memcpy(cEntry.cColorAlpha.pdColor, pvcColor->pdColor, sizeof(cEntry.cColorAlpha.pdColor));

				// update the scalar 
				cEntry.dScalar = pvcAlpha->dScalar;

				pvcColor++;
				pvcAlpha++;
			}
			// insert this entry to the function
			vcFunction.push_back(cEntry);
		} // while

		if( pvcAlpha != vcOpacities.end() ) 
		{
			cEntry.dScalar = pvcAlpha->dScalar;
			cEntry.cColorAlpha.dAlpha = pvcAlpha->dAlpha;
		}
		else
		if( pvcColor != vcColors.end() )
		{
			cEntry.dScalar = pvcColor->dScalar;
			memcpy(cEntry.cColorAlpha.pdColor, pvcColor->pdColor, sizeof(cEntry.cColorAlpha.pdColor));
		}

		dScalarRangeMax = cEntry.dScalar;

		vector<CColorAlphaEntry>::iterator	viEntry;
		for(viEntry = vcFunction.begin(); viEntry != vcFunction.end(); viEntry++ )
			fprintf(stderr, "%f,\t(%f,\t%f,\t%f,\t%f)\n", 
				viEntry->dScalar,
				viEntry->cColorAlpha.pdColor[0], 
				viEntry->cColorAlpha.pdColor[1], 
				viEntry->cColorAlpha.pdColor[2], 
				viEntry->cColorAlpha.dAlpha); 
	}

	return true;
}

bool 
CTransferFunction::BLoadCmap(
	const char *szCmapPathFilename, 
	double dMinScalar, 
	double dMaxScalar)
{
	FILE* fpCmap;
	fopen_s(&fpCmap, szCmapPathFilename, "r+t");
	if( !fpCmap ) {
		perror(szCmapPathFilename);
		return false;
	}

	dScalarRangeMin = dMinScalar;
	dScalarRangeMax = dMaxScalar;

	size_t uNrOfEntries;
	fscanf_s(fpCmap, "%d", &uNrOfEntries);

	for(int iI=0; iI<(int)uNrOfEntries; iI++) 
	{
		CColorAlphaEntry cEntry;

		float pfBuffer[4];
		if( 4 != fscanf_s(fpCmap, "%f %f %f %f", 
			&pfBuffer[0],
			&pfBuffer[1],
			&pfBuffer[2],
			&pfBuffer[3]) )
			perror("");

		cEntry.cColorAlpha.pdColor[0]	= (double)pfBuffer[0];
		cEntry.cColorAlpha.pdColor[1]	= (double)pfBuffer[1];
		cEntry.cColorAlpha.pdColor[2]	= (double)pfBuffer[2];
		cEntry.cColorAlpha.dAlpha		= (double)pfBuffer[3];

		cEntry.dScalar = INTERP((double)iI, (double)0, (double)uNrOfEntries-1, dMinScalar, dMaxScalar);

		vcFunction.push_back(cEntry);
	}
	fclose(fpCmap);

	return true;
}

CColorAlpha
CTransferFunction::CIntegrate(double dFrontScalar, double dBackScalar, double dIntervalDepth)
{
	//	fprintf(stderr, "CTransferFunction::CIntegrate: Depth=%f\n", dIntervalDepth);

	CColorAlpha cSum;

	if( dFrontScalar < dScalarRangeMin	|| dScalarRangeMax < dFrontScalar || // out of range
		dBackScalar  < dScalarRangeMin	|| dScalarRangeMax < dBackScalar )
		return cSum;

	if( dFrontScalar == dBackScalar ) 
	{
		double dScalar = dBackScalar;

		vcaIterator vi = this->CFindEntry(dFrontScalar);
		if( vi == vcFunction.end() )
			return cSum;

		cSum = CInterpolate(
			dScalar, 
			vi->dScalar, (vi+1)->dScalar, 
			vi->cColorAlpha, (vi+1)->cColorAlpha);

		double dAlpha = cSum.dAlpha;

		// adjust alpha by depth
		dAlpha = 1.0 - pow(1.0 - dAlpha, dIntervalDepth);

		cSum.dAlpha = dAlpha;
		// MOD-BY-LEETEN 04/03/2007-BEGIN
		/* FROM:
		for(size_t c=0; c<3; c++)
			cSum.pdColor[c] *= dAlpha;
		TO: */
		cSum = CAssociateColor(cSum);
		// MOD-BY-LEETEN 04/03/2007-END
	}
	else
	{
		CColorAlpha cCA;
		if( dFrontScalar < dBackScalar ) 
		{
			vcaIterator vi, viFront, viBack, viEntry;
			double dPrevDepth, dDepth;

			viFront = this->CFindEntry(dFrontScalar);
			viBack = this->CFindEntry(dBackScalar);

			// build a entry for scalar equal to dFrontScalar
			CColorAlphaEntry cPrev, cEnd;

			cPrev.dScalar = dFrontScalar;
			cPrev.cColorAlpha = CInterpolate(
				dFrontScalar, viFront->dScalar, (viFront+1)->dScalar, 
				viFront->cColorAlpha, (viFront+1)->cColorAlpha);

			dPrevDepth = 0.0;
#if 01// 04/02/TEST
			for(vi = viFront+1; vi != viBack + 1; vi++)
			{
				dDepth = INTERP(
					vi->dScalar, dFrontScalar, dBackScalar, 0.0, dIntervalDepth);

				// compute the integral between dPrevDepth and dDepth
				cCA = this->CIntegrate(cPrev.dScalar, vi->dScalar, dDepth - dPrevDepth);
				cSum = CFrontToBackComposition(cSum, cCA);

				// update entry and depth
				dPrevDepth = dDepth;
				cPrev = *vi;
			}
#endif	// #if 0	// 04/02/TEST

			dDepth = dIntervalDepth;

			cEnd.dScalar = dBackScalar;
			cEnd.cColorAlpha = CInterpolate(
				dBackScalar, viBack->dScalar, (viBack+1)->dScalar, 
				viBack->cColorAlpha, (viBack+1)->cColorAlpha);

			// apply the integral
			double dD = (dDepth - dPrevDepth)/(double)SUPER_SAMPLING_FACTOR;
			for(size_t s=0; s<SUPER_SAMPLING_FACTOR; s++)
			{
				cCA = CInterpolate(
					// 04/02/TEST	(double)(s+1), (double)0, (double)SUPER_SAMPLING_FACTOR, 
					0.5 + (double)s, (double)0, (double)SUPER_SAMPLING_FACTOR, // 04/02/TEST
					cPrev.cColorAlpha, cEnd.cColorAlpha);

				// adjust alpha by depth
				cCA.dAlpha = 1.0 - pow(1.0 - cCA.dAlpha, dD);

				// ADD-BY-LEETEN 04/03/2007-BEGIN
				cCA = CAssociateColor(cCA);
				// ADD-BY-LEETEN 04/03/2007-END

				// apply front-to-back composistion
				cSum = CFrontToBackComposition(cSum, cCA);
			}
		}
		else
		// if( dFrontScalar > dBackScalar ) 
		{
			vcaRiterator vi, viFront, viBack, viEntry;
			double dPrevDepth, dDepth;

			viFront = this->CFindEntryReversely(dFrontScalar);
			viBack = this->CFindEntryReversely(dBackScalar);

			// build a entry for scalar equal to dFrontScalar
			CColorAlphaEntry cPrev, cEnd;

			cPrev.dScalar = dFrontScalar;
			cPrev.cColorAlpha = CInterpolate(
				dFrontScalar, viFront->dScalar, (viFront+1)->dScalar, 
				viFront->cColorAlpha, (viFront+1)->cColorAlpha);

			dPrevDepth = 0.0;
#if 1	// 04/02/TEST
			for(vi = viFront+1; vi != viBack+1; vi++)
			{
				double dScalar = vi->dScalar;
				dDepth = INTERP(
					dScalar, dFrontScalar, dBackScalar, 0.0f, dIntervalDepth);

				// compute the integral between dPrevDepth and dDepth
				cCA = this->CIntegrate(cPrev.dScalar, dScalar, dDepth - dPrevDepth);
				cSum = CFrontToBackComposition(cSum, cCA);

				// update entry and depth
				dPrevDepth = dDepth;
				cPrev = *vi;
			}
#endif	//	#if 0	// 04/02/TEST

			dDepth = dIntervalDepth;

			cEnd.dScalar = dBackScalar;
			cEnd.cColorAlpha = CInterpolate(
				dBackScalar, viBack->dScalar, (viBack+1)->dScalar, 
				viBack->cColorAlpha, (viBack+1)->cColorAlpha);

			// apply the integral
			size_t uNrOfSamples;
			if( dSampleDistnace <= 0.0 )
				uNrOfSamples = SUPER_SAMPLING_FACTOR;
			else
				uNrOfSamples = (size_t)ceil(fabs(cPrev.dScalar - cEnd.dScalar) / dSampleDistnace);
		
			double dD = (dDepth - dPrevDepth)/(double)uNrOfSamples;
			
			for(size_t s=0; s<uNrOfSamples; s++)
			{
				cCA = CInterpolate(
					// 04/02/TEST	(double)(s+1), (double)0, (double)SUPER_SAMPLING_FACTOR, 
					0.5 + (double)s, (double)0, (double)SUPER_SAMPLING_FACTOR,// 04/02
					cPrev.cColorAlpha, cEnd.cColorAlpha);

				// adjust alpha by depth
				cCA.dAlpha = 1.0 - pow(1.0 - cCA.dAlpha, dD);

				// ADD-BY-LEETEN 04/03/2007-BEGIN
				cCA = CAssociateColor(cCA);
				// ADD-BY-LEETEN 04/03/2007-END

				// apply front-to-back composistion
				cSum = CFrontToBackComposition(cSum, cCA);
			}
		}
	}

	return cSum;
}

// find the entry e s.t e.s < dScalar < (e+1).s
vcaIterator
CTransferFunction::CFindEntry(double dScalar)
{
	vcaIterator vi;
	for(vi=vcFunction.begin(); vi != vcFunction.end(); vi++)
		if( dScalar <= vi->dScalar ) 
		{
			vi--;
			break;
		}

	return vi;
}

// find the entry e s.t (e-1).s < dScalar < e.s
vcaRiterator
CTransferFunction::CFindEntryReversely(double dScalar)
{
	vcaRiterator vri;
	for(vri=vcFunction.rbegin(); vri != vcFunction.rend(); vri++)
	{
		double fS = vri->dScalar;
		if( fS <= dScalar ) 
		{
			vri--;
			break;
		}
	}

	return vri;
}

// sample the function to build a 1D lookup table
const float *
CTransferFunction::PFGet1DLookupTable(size_t uNrOfEntries, double dScalarMin, double dScalarMax)
{
	if( pfLookupTable )
		delete [] pfLookupTable;
	pfLookupTable = new float[4 * uNrOfEntries];

	// reserve the 1st entry as zero
	for(size_t c=0; c<4; c++)
		pfLookupTable[c] = 0.0f;
	for(size_t uE=1; uE<uNrOfEntries; uE++) 
	{
		double dE = (double)uE + 0.5;
		double dScalar = INTERP(dE, (double)1, (double)uNrOfEntries-1, dScalarMin, dScalarMax);
		vcaIterator cEntry = CFindEntry(dScalar);
		CColorAlpha cCa = CInterpolate(
			dScalar, 
			cEntry->dScalar,	 (cEntry+1)->dScalar, 
			cEntry->cColorAlpha, (cEntry+1)->cColorAlpha);

		cCa._AssignToFloatArray(&pfLookupTable[4*uE]);
	}

	return pfLookupTable;
}

const float *
CTransferFunction::PFGet3DPreIntegratedTable(
	size_t uNrOfFrontValues, size_t uNrOfBackValues, size_t uNrOfDepthIntervals, 
	double dScalarMin, double dScalarMax, double dMaxDepth)
{
	fprintf(stderr, "Pre-integrate lookup table..");

	if( pfPreIntegratedTable )
		delete [] pfPreIntegratedTable;
	pfPreIntegratedTable = new float[4 * uNrOfFrontValues * uNrOfBackValues * uNrOfDepthIntervals];
	memset(pfPreIntegratedTable, 0, 4 * sizeof(pfPreIntegratedTable) * uNrOfFrontValues * uNrOfBackValues * uNrOfDepthIntervals);

	dSampleDistnace = (dScalarMax - dScalarMin) / (double)SAMPLING_FREQUENCIES;

	double dDepthUnit = dMaxDepth/(double)(uNrOfDepthIntervals - 1);

	CColorAlpha *pcCAOverUnitDepth;
	CColorAlpha *pcCAOverLastDepth, *pcCAOverPrevDepth;

	pcCAOverUnitDepth = (CColorAlpha*)calloc(sizeof(CColorAlpha), uNrOfFrontValues * uNrOfBackValues);
	pcCAOverLastDepth = (CColorAlpha*)calloc(sizeof(CColorAlpha), uNrOfFrontValues * uNrOfBackValues);
	pcCAOverPrevDepth = (CColorAlpha*)calloc(sizeof(CColorAlpha), uNrOfFrontValues * uNrOfBackValues);

	for(size_t p=0, d=0; d<uNrOfDepthIntervals; d++)
	{
		for(size_t p2=0, f=0; f<uNrOfFrontValues; f++)
			for(size_t b=0; b<uNrOfBackValues; b++, p++, p2++)
			{
				if( !d  || !f || !b )
					continue;

				double dFront	= INTERP((double)f, (double)1, (double)uNrOfFrontValues-1,	dScalarMin, dScalarMax);
				double dBack	= INTERP((double)b, (double)1, (double)uNrOfBackValues-1,	dScalarMin, dScalarMax);
				double dIndex;
				size_t i;		// index to the interpolated value
				CColorAlpha cCA;		// integral of color/alpha in the 1st part
				CColorAlpha pcCAs[2];	// integral of color/alpha in the later part

				if( f == b )
					cCA = pcCAOverLastDepth[p2] = CIntegrate(dFront, dBack, dDepthUnit * (double)d);
				else
					// if depth is zero, there is no color/alpha
					switch(d)
					{
					#if 1

					case 1:	// exactly compute the integral from front to back over the unit depth d
						cCA = pcCAOverUnitDepth[p2] = CIntegrate(dFront, dBack, dDepthUnit);
						break;

					case 2:	// compute the integral from front to back over 2d
						dIndex = ((double)f + (double)b)/2.0;

						i = (size_t)floor(dIndex);
						for(size_t j=0; j<2; j++)
							pcCAs[j] = CFrontToBackComposition(
								pcCAOverUnitDepth[ f	 * uNrOfBackValues + i + j], 
								pcCAOverUnitDepth[(i + j)* uNrOfBackValues + b    ]);

						cCA = pcCAOverLastDepth[p2] = CInterpolate(dIndex, (double)i, (double)i+1, pcCAs[0], pcCAs[1]);

						break;

					default: // compute the integral from front to back over nd
						dIndex = INTERP((double)d-1, (double)0, (double)d, (double)f, (double)b);

						i = (size_t)floor(dIndex);
						for(size_t j=0; j<2; j++)
							pcCAs[j] = CFrontToBackComposition(
								pcCAOverPrevDepth[ f	   * uNrOfBackValues + i + j],
								pcCAOverUnitDepth[ (i + j) * uNrOfBackValues + b    ]);
						cCA = pcCAOverLastDepth[p2] = CInterpolate(dIndex, (double)i, (double)i+1, pcCAs[0], pcCAs[1]);

					#else
					default: // exactly compute the integral from front to back over nd
						cCA = CIntegrate(dFront, dBack, dDepthUnit * (double)d);
					#endif
				} // switch

				// assign the integrated result to the pre-integrated table
				cCA._AssignToFloatArray(&pfPreIntegratedTable[4*p]);
			}

		// update the latest integral result from pcCAOverLastDepth to pcCAOverPrevDepth
		if( d > 2 )
			swap(pcCAOverPrevDepth, pcCAOverLastDepth);
	} 

	free(pcCAOverUnitDepth);
	free(pcCAOverLastDepth);
	free(pcCAOverPrevDepth);

	fprintf(stderr, "..done\n");

	return pfPreIntegratedTable;
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2007/03/12 23:44:40  leeten

[03/12/2007]
1. First time checkin. This is a new version of transfer function library.


*/
