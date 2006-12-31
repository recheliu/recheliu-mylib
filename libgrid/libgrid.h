#ifndef __LIB__GRID__H__
#define __LIB__GRID__H__

#include <math.h>
#include <stdlib.h>

#include "triangle.h"

// ADD-BY-LEETEN 12/30/2006-BEGIN
// define the interesting scalar field
typedef enum {
	GRID_SCALAR_NONE,	// if this flag is given, the .soln file will be ignored
	GRID_SCALAR_DENSITY, 
	GRID_SCALAR_ENERGY, 
	GRID_SCALAR_PRESSURE, 
	GRID_SCALAR_VELOCITY_MAGNITUDE
} EGridScalar;
// ADD-BY-LEETEN 12/30/2006-END

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

	// MOD-BY-LEETEN 12/30/2006-BEGIN
	/* FROM: bool BLoad(char* szGridName, char* szSolnName); TO: */
	bool BLoad(char* szGridName, char* szSolnName, EGridScalar=GRID_SCALAR_NONE); 
	// MOD-BY-LEETEN 12/30/2006-END
	void _FindBBox(int iTriangleIndex = -1);
	bool BAllocPlot3D();
	
	// ADD-By-LEETEN 12/25/2006-BEGIN
	float FNormalizeScalar(const float& fValue)			{	return 1.0f/255.0f + (fValue - fMinV)/(fMaxV - fMinV) * 254.0f / 255.0f; }
	float FGetNormalizedScalar(const size_t& uIndex)	{	return FNormalizeScalar(pfVerticesV[uIndex]);	}
	// ADD-By-LEETEN 12/25/2006-END

} CGrid;

#endif // __LIB__GRID__H__
