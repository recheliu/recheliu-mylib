#ifndef __LIB__PPM__H__
#define __LIB__PPM__H__

#include <stdlib.h>
#include <memory.h>

typedef struct CImage{
	size_t uWidth;
	size_t uHeight;
	typedef float pfPixel[4];
	pfPixel* pfBuffer;
	CImage() {	
		uWidth = uHeight = 0;
		pfBuffer = NULL;
	}
	~CImage() {
		if( pfBuffer )
			delete [] pfBuffer;
	}
	bool BAlloc() {	
		if(uWidth && uHeight)
			pfBuffer = (pfPixel*)calloc(sizeof(pfPixel), uWidth * uHeight);
		else
			return false;
		return (pfBuffer)?true:false;
	}
	bool BFree() {	
		if( pfBuffer ) {
			free(pfBuffer);
			pfBuffer = NULL;
		}
		return true;
	}
	bool BRead(const char szFileName[]);
	bool BWrite(const char szFileName[]);
	bool BSetPixel(size_t uW, size_t uH, float fR, float fG, float fB) 
	{	
		// if(uW>=uWidth || uH>uHeight)
		//	return false;

		size_t uI = uH * uWidth + uW;
		pfBuffer[uI][0] = fR;
		pfBuffer[uI][1] = fG;
		pfBuffer[uI][2] = fB;
		pfBuffer[uI][3] = 1.0f;
		return true;
	}
	// ADD-BY-LEETEN 02/25/2008-BEGIN
	bool BSetPixelAlpha(size_t uW, size_t uH, float fA) 
	{	
		if(uW>=uWidth || uH>uHeight)
			return false;

		size_t uI = uH * uWidth + uW;
		pfBuffer[uI][3] = fA;
		return true;
	}
	// ADD-BY-LEETEN 02/25/2008-END
	bool BGetPixel(size_t uW, size_t uH, float *pfR, float *pfG, float *pfB) 
	{	
		// if(uW>=uWidth || uH>uHeight)
		//	return false;

		size_t uI = uH * uWidth + uW;
		*pfR = pfBuffer[uI][0];
		*pfG = pfBuffer[uI][1];
		*pfB = pfBuffer[uI][2];
		return true;
	}
		
	// ADD-BY-LEETEN 11/30/2006-BEGIN
	float* PFConvertToRgb();	// convert to a buffer in floating point format in RGB format
	// ADD-BY-LEETEN 11/30/2006-END
		
} CImage;

#endif

/*

$Log: not supported by cvs2svn $
Revision 1.2  2006/12/02 01:45:59  leeten

[12/01/2006]
1. Declare new function PFConvertToRgb() to return the pixels as a array, where each element has three componenet as RGB.

Revision 1.1.1.1  2006/09/07 18:51:44  leeten

[09/07/2006]
1. First time checkin. 


*/
