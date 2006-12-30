#ifndef __LIB__GRID__H__
#define __LIB__GRID__H__

#include <math.h>
#include <stdlib.h>

#include "triangle.h"

typedef struct CGrid
{
	size_t uNrOfNodes, uNrOfTriangles, uNrOfTets;

	float* pfVerticesX;
	float* pfVerticesY;
	float* pfVerticesZ;
	float* pfVerticesV;
	float fMaxX, fMaxY, fMaxZ, fMaxV;
	float fMinX, fMinY, fMinZ, fMinV;

	CTriangle* pcTriangles;
	size_t* puTIndices;

	typedef struct {
		size_t puIndex[4];
	} CVolume;

	CVolume* pcVolumes;
	bool* pbVolumes;

	CGrid() 
	{
		fMinX = fMinY = fMinZ = fMinV = (float)-HUGE_VAL;
		fMaxX = fMaxY = fMaxZ = fMaxV = (float)HUGE_VAL;
		uNrOfNodes = uNrOfTriangles = uNrOfTets = 0;
		pcVolumes = NULL;
		pbVolumes = NULL;
		pcTriangles = NULL;
		puTIndices = NULL;
	} 

	~CGrid() 
	{
		if( pcVolumes )
			free(pcVolumes);
		if( pbVolumes )
			free(pbVolumes);
		if( puTIndices )
			free(puTIndices);
		if( pcTriangles )
			free(pcTriangles);
	} 

	bool BLoad(char* szGridName, char* szSolnName);
	void _FindBBox(int iTriangleIndex = -1);
	bool BAllocPlot3D();
	
	// ADD-By-LEETEN 12/25/2006-BEGIN
	float FNormalizeScalar(const float& fValue)			{	return 1.0f/255.0f + (fValue - fMinV)/(fMaxV - fMinV) * 254.0f / 255.0f; }
	float FGetNormalizedScalar(const size_t& uIndex)	{	return FNormalizeScalar(pfVerticesV[uIndex]);	}
	// ADD-By-LEETEN 12/25/2006-END

} CGrid;

#endif // __LIB__GRID__H__
