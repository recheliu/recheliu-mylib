#include <stdio.h>
#include <string.h>
// ADD-BY-LEETEN 11/30/2006-BEGIN
#include <assert.h>
// ADD-BY-LEETEN 11/30/2006-END

#include "libppm.h"

bool CImage::BWrite (const char szFileName[])
{
	FILE* fpImage = fopen(szFileName, "w+b");

	fprintf(fpImage, "P6\n");
	fprintf(fpImage, "%d %d\n", uWidth, uHeight);	// output the window size of the image
	fprintf(fpImage, "%d\n", 255);	// output the range of the color

	if ( !uHeight || !uWidth || !pfBuffer )
		return false;

	size_t uIndex, uH, uW, uC;

	for(uIndex=0, uH=0; uH<uHeight; uH++)
		for(uW=0; uW<uWidth; uW++, uIndex++) 
			for(uC=0; uC<3; uC++) 
			{
				float fColor = (pfBuffer[uIndex][uC] * 255.0f);
				unsigned char ubColor = (fColor<=255.0f)?(unsigned char)fColor:255;
				fwrite(&ubColor, 1, 1, fpImage);
			}

	fclose(fpImage);
	return true;
}

bool CImage::BRead(const char szFileName[])
{
	bool bBin = false;
	FILE* fpImage = fopen(szFileName, "r+b");

	if( !fpImage )
		return false;

	static char szBuf[128+1];
	int iRange;

	fscanf(fpImage, "%s", szBuf);	  // output the "magic number"
	if( strcmp(szBuf, "P6") && strcmp(szBuf, "P3") )
		return false;

	bBin = (!strcmp(szBuf, "P6"))?true:false;
	fscanf(fpImage, "%d %d", &uWidth, &uHeight);	// output the window size of the image
//	if( !bBin )
//08/17		fscanf(fpImage, "%d\n", &iRange);	// output the range of the color
		fscanf(fpImage, "%d", &iRange);	// output the range of the color
		//	else
//		iRange = 255;

	BAlloc();

	if ( !uHeight || !uWidth || !pfBuffer )
		return false;

	// 08/05
	fseek(fpImage, 1, SEEK_CUR);

	size_t uIndex, uH, uW, uC;

	for(uIndex=0, uH=0; uH<uHeight; uH++)
		for(uW=0; uW<uWidth; uW++, uIndex++) 
		{
			for(uC=0; uC<3; uC++) 
			{
				int iColor = 0;
				if( bBin )
				{
					// ADD-BY-LEETEN 11/30/2006-BEGIN
					if( iRange == 65535 )
					{
						unsigned short usColor;
						fread(&usColor, sizeof(usColor), 1, fpImage);
						iColor = usColor;
					}
					else if( iRange==255 )
					{
					// ADD-BY-LEETEN 11/30/2006-END
					unsigned char ubColor;
					fread(&ubColor, sizeof(ubColor), 1, fpImage);
					iColor = ubColor;
					// ADD-BY-LEETEN 11/30/2006-BEGIN
					}
					// ADD-BY-LEETEN 11/30/2006-END
				}
				else 
				{
					fscanf(fpImage, "%d ", &iColor);
				}
				if( iColor )
					pfBuffer[uIndex][uC] = (float)iColor/(float)iRange;
			}
		}

	fclose(fpImage);
	
	return true;
}

// ADD-BY-LEETEN 11/30/2006-BEGIN
// convert to a buffer in floating point format in RGB format
float* 
CImage::PFConvertToRgb()
{
	float *pfRgb;
	if(!uWidth || !uHeight || !pfBuffer )
		return NULL;

	size_t uP, uH, uW, uC;
	assert( pfRgb = (float*)calloc(sizeof(float)*3, uWidth * uHeight) );
	for(uP=0, uH=0; uH<uHeight; uH++)
		for(uW=0; uW<uWidth; uW++, uP++)
			for(uC=0; uC<3; uC++)
				pfRgb[3*uP+uC] = pfBuffer[uP][uC];

	return pfRgb;
}
// ADD-BY-LEETEN 11/30/2006-END

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2006/09/07 18:51:44  leeten

[09/07/2006]
1. First time checkin. 


*/
