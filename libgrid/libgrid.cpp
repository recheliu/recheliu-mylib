#include <stdio.h>
#include <assert.h>

#include "libgrid.h"

bool 
CGrid::BLoad(char* szGridName, char* szSolnName)
{
	FILE *fpGrid;
	FILE *fpSoln;
	size_t uI;

	fpGrid = fopen(szGridName, "r+b");
	if( !fpGrid ) {
		perror(szGridName);
		return false;
	}

	fpSoln = fopen(szSolnName, "r+b");
	if( !fpSoln) {
		perror(szSolnName);
		return false;
	}

	printf("Reading %s\n", szGridName);

	// reading the header
	fread(&uNrOfNodes, sizeof(uNrOfNodes), 1, fpGrid);
	fread(&uNrOfTriangles, sizeof(uNrOfTriangles), 1, fpGrid);
	fread(&uNrOfTets, sizeof(uNrOfTets), 1, fpGrid);

	printf("#Nodes: %d; #Tri: %d; #Vol: %d\n", uNrOfNodes, uNrOfTriangles, uNrOfTets);

	if( !BAllocPlot3D() )
		return false;

	// DEL-BY-leety-10/31/2006-BEGIN
	// del the old code that allocate space for all arrays
	// DEL-BY-leety-10/31/2006-END

	for( uI=0; uI<uNrOfNodes; uI++) {
		fread(&pfVerticesX[uI], sizeof(*pfVerticesX), 1, fpGrid);
		fread(&pfVerticesY[uI], sizeof(*pfVerticesY), 1, fpGrid);
		fread(&pfVerticesZ[uI], sizeof(*pfVerticesZ), 1, fpGrid);
	}
	fread(pcTriangles, sizeof(*pcTriangles), uNrOfTriangles, fpGrid);
	fread(puTIndices, sizeof(*puTIndices), uNrOfTriangles, fpGrid);
	fread(pcVolumes, sizeof(*pcVolumes), uNrOfTets, fpGrid);

	fclose(fpGrid);

	printf("Reading %s\n", szSolnName);

	fseek(fpSoln, sizeof(size_t), SEEK_CUR);
	fseek(fpSoln, sizeof(float) * uNrOfNodes, SEEK_CUR);
	fread(pfVerticesV, sizeof(*pfVerticesV), uNrOfNodes, fpSoln);

	fclose(fpSoln);

	_FindBBox();	
	return true;
}

void 
CGrid::_FindBBox(int iTriangleIndex)
{
	size_t uI, uP;

	// find the bouding box formed by the triangles of the specified index
	fMaxX = fMaxY = fMaxZ = fMaxV = (float)-HUGE_VAL;
	fMinX = fMinY = fMinZ = fMinV = (float)HUGE_VAL;

	// ADD-BY-LEETY 04/06-BEGIN
	if( iTriangleIndex < 0 ) 
		for(uI=0; uI<uNrOfNodes; uI++) 
		{
			if ( pfVerticesX[uI] > fMaxX )
				fMaxX = pfVerticesX[uI];
			if ( pfVerticesX[uI] < fMinX )
				fMinX = pfVerticesX[uI];

			if ( pfVerticesY[uI] > fMaxY )
				fMaxY = pfVerticesY[uI];
			if ( pfVerticesY[uI] < fMinY )
				fMinY = pfVerticesY[uI];

			if ( pfVerticesZ[uI] > fMaxZ )
				fMaxZ = pfVerticesZ[uI];
			if ( pfVerticesZ[uI] < fMinZ )
				fMinZ = pfVerticesZ[uI];

			if ( pfVerticesV[uI] > fMaxV )
				fMaxV = pfVerticesV[uI];
			if ( pfVerticesV[uI] < fMinV )
				fMinV = pfVerticesV[uI];
		}
	else
	// ADD-BY-LEETY 04/06-END

	for(uI=0; uI<uNrOfTriangles; uI++) 
	{
		size_t *puNodeIndex = pcTriangles[uI].puIndex;
		// mod-by-leety-01/14/2006-BEGIN
		// FROM: if( 2 != puTIndices[uI] )
		// TO:
		if( iTriangleIndex >= 0 && (
			(puTIndices[uI]>=255 && iTriangleIndex != (int)puTIndices[uI]-255) ||
			(iTriangleIndex != (int)puTIndices[uI]) )
			)
		// add-by-leety-01/14/2006-END
			continue;

		for(uP=0; uP<3; uP++) 
		{
			if ( pfVerticesX[puNodeIndex[uP]] > fMaxX )
				fMaxX = pfVerticesX[puNodeIndex[uP]];
			if ( pfVerticesX[puNodeIndex[uP]] < fMinX )
				fMinX = pfVerticesX[puNodeIndex[uP]];

			if ( pfVerticesY[puNodeIndex[uP]] > fMaxY )
				fMaxY = pfVerticesY[puNodeIndex[uP]];
			if ( pfVerticesY[puNodeIndex[uP]] < fMinY )
				fMinY = pfVerticesY[puNodeIndex[uP]];

			if ( pfVerticesZ[puNodeIndex[uP]] > fMaxZ )
				fMaxZ = pfVerticesZ[puNodeIndex[uP]];
			if ( pfVerticesZ[puNodeIndex[uP]] < fMinZ )
				fMinZ = pfVerticesZ[puNodeIndex[uP]];

			if ( pfVerticesV[puNodeIndex[uP]] > fMaxV )
				fMaxV = pfVerticesV[puNodeIndex[uP]];
			if ( pfVerticesV[puNodeIndex[uP]] < fMinV )
				fMinV = pfVerticesV[puNodeIndex[uP]];
		}
	}

	printf("X: [%f %f]\n", fMinX, fMaxX);
	printf("Y: [%f %f]\n", fMinY, fMaxY);
	printf("Z: [%f %f]\n", fMinZ, fMaxZ);
	printf("V: [%f %f]\n", fMinV, fMaxV);
}

bool 
CGrid::BAllocPlot3D()
{
	if( !(pfVerticesX = (float*)calloc(sizeof(*pfVerticesX), uNrOfNodes) ) ) {
		perror("pfVerticesX");
		return false;
	}
	if( !(pfVerticesY = (float*)calloc(sizeof(*pfVerticesY), uNrOfNodes) ) ) {
		perror("pfVerticesY");
		return false;
	}
	if( !(pfVerticesZ = (float*)calloc(sizeof(*pfVerticesZ), uNrOfNodes) ) ) {
		perror("pfVerticesZ");
		return false;
	}
	if( !(pcTriangles = (CTriangle*)calloc(sizeof(*pcTriangles), uNrOfTriangles) ) ) {
		perror("pcTriangles");
		return false;
	}
	if( !(puTIndices= (size_t*)calloc(sizeof(*puTIndices), uNrOfTriangles) ) ) {
		perror("puTIndices");
		return false;
	}
	if( !(pcVolumes = (CVolume*)calloc(sizeof(*pcVolumes), uNrOfTets) ) ) {
		perror("pcVolumes");
		return false;
	}
	if( !(pbVolumes = (bool*)calloc(sizeof(*pbVolumes), uNrOfTets) ) ) {
		perror("pbVolumes");
		return false;
	}
	if( !(pfVerticesV = (float*)calloc(sizeof(*pfVerticesV), uNrOfNodes) ) ) {
		perror("pfVerticesV");
		return false;
	}
	return true;
}


