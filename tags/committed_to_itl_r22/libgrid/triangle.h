#ifndef __TRIANGLE__H__
#define __TRIANGLE__H__

#include <memory.h>

typedef struct CTriangle {
	size_t puIndex[3];
	CTriangle() { puIndex[0] = puIndex[1] = puIndex[2] = 0;	}
	CTriangle(const CTriangle& cT) { 
		puIndex[0] = cT.puIndex[0];
		puIndex[1] = cT.puIndex[1];
		puIndex[2] = cT.puIndex[2];
	}
	CTriangle(size_t uIndex0, size_t uIndex1, size_t uIndex2) 
	{ 
		puIndex[0] = uIndex0;
		puIndex[1] = uIndex1;
		puIndex[2] = uIndex2;
	}
}CTriangle;

inline bool operator<(const CTriangle& cT1, const CTriangle& cT2)
{
	size_t puIndex1[3], puIndex2[3];
	size_t uMax, uTemp;

	memcpy(puIndex1, puIndex1, sizeof(puIndex1));
	memcpy(puIndex2, puIndex2, sizeof(puIndex2));

	uMax = 0;
	if(puIndex1[1] > puIndex1[0]) 
		uMax = 1;
	if(puIndex1[2]> puIndex1[uMax]) 
		uMax = 2;

	// swap the one with the maximum one
	if( uMax < 2 ) {
		uTemp = puIndex1[2];
		puIndex1[2] = puIndex1[uMax];
		puIndex1[uMax] = uTemp;
	}

	if( puIndex1[0] > puIndex1[1] ) {
		uTemp = puIndex1[0];
		puIndex1[0] = puIndex1[1];
		puIndex1[1] = uTemp;
	}

	uMax = 0;
	if(puIndex2[1] > puIndex2[0]) 
		uMax = 1;
	if(puIndex2[2]> puIndex2[uMax]) 
		uMax = 2;

	// swap the one with the maximum one
	if( uMax < 2 ) {
		uTemp = puIndex2[2];
		puIndex2[2] = puIndex2[uMax];
		puIndex2[uMax] = uTemp;
	}

	if( puIndex2[0] > puIndex2[1] ) {
		uTemp = puIndex2[0];
		puIndex2[0] = puIndex2[1];
		puIndex2[1] = uTemp;
	}

	if(puIndex1[0]<puIndex2[0])
		return true;
	else if(puIndex1[0]==puIndex2[0])
	{
		if(puIndex1[1]<puIndex2[1])
			return true;
		else if(puIndex1[1]==puIndex2[1])
		{
			if(puIndex1[2]<puIndex2[2])
				return true;
			else 
				return false;
		}
		else
			return false;
	}
	else
		return false;

}

inline bool operator==(const CTriangle& cT1, const CTriangle& cT2)
{
	size_t puIndex1[3], puIndex2[3];
	size_t uMax, uTemp;

	memcpy(puIndex1, puIndex1, sizeof(puIndex1));
	memcpy(puIndex2, puIndex2, sizeof(puIndex2));

	uMax = 0;
	if(puIndex1[1] > puIndex1[0]) 
		uMax = 1;
	if(puIndex1[2]> puIndex1[uMax]) 
		uMax = 2;

	// swap the one with the maximum one
	if( uMax < 2 ) {
		uTemp = puIndex1[2];
		puIndex1[2] = puIndex1[uMax];
		puIndex1[uMax] = uTemp;
	}

	if( puIndex1[0] > puIndex1[1] ) {
		uTemp = puIndex1[0];
		puIndex1[0] = puIndex1[1];
		puIndex1[1] = uTemp;
	}

	uMax = 0;
	if(puIndex2[1] > puIndex2[0]) 
		uMax = 1;
	if(puIndex2[2]> puIndex2[uMax]) 
		uMax = 2;

	// swap the one with the maximum one
	if( uMax < 2 ) {
		uTemp = puIndex2[2];
		puIndex2[2] = puIndex2[uMax];
		puIndex2[uMax] = uTemp;
	}

	if( puIndex2[0] > puIndex2[1] ) {
		uTemp = puIndex2[0];
		puIndex2[0] = puIndex2[1];
		puIndex2[1] = uTemp;
	}

	if( puIndex1[0]==puIndex2[0] &&
		puIndex1[1]==puIndex2[1] &&
		puIndex1[2]==puIndex2[2] )
		return true;
	else
		return false;
}

#endif
