#include <stdio.h>
#include <string.h>

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
					unsigned char ubColor;
					fread(&ubColor, sizeof(ubColor), 1, fpImage);
					iColor = ubColor;
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

/*

$Log: not supported by cvs2svn $

*/
