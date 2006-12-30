/*

Name: 
	LIBTF.h

Description:
	Define functions to load transfer function. The transfer function includes two files. 
One is for the color and the other is for the opacity. 

	The data strcture of the transfer function is an array of 4 tuple as (r, g, b, a). 
The fist entry is always (0, 0, 0, 0) so it could be used to define empty region. In 
order to be used in OpenGL, the range of the original scalar, [min, max] will be normalized to 
[1/N, N/N] while N is the number of entries. 
*/

#include <vector>

#include <stdio.h>
#include <assert.h>

#include "libtf.h"

#include "libopengl.h"

using namespace std;

typedef struct CColor3D {
	float fScalar;
	float pfColor[3];
}CColor3D;

typedef struct CAlpha {
	float fScalar;
	float fAlpha;
}CAlpha ;

bool
CTransferFunction1D::BLoad(const char* szColorMap, const char* szOpacityMap)
{
	FILE *fpColor, *fpOpacity;

	if( fScalarMin > fScalarMax || !pfEntries ) 
	{
		fprintf(stderr, "Warning! _Init() should be called before BLoad()\n");
		return false;
	}

	if( !(fpColor = fopen(szColorMap, "r+t")) )
		return false;

	if( !(fpOpacity = fopen(szOpacityMap, "r+t")) )
		return false;

	vector<CColor3D> vcColors;
	vector<CAlpha> vcOpacities;

	static char szBuf[1024+1];
	// read the color map
	fgets(szBuf, sizeof(szBuf)-1, fpColor);
	while( true ) 
	{
		CColor3D cColor;
		if( 4 != fscanf(fpColor, "%f %f %f %f\n", 
			&cColor.fScalar,
			&cColor.pfColor[0],
			&cColor.pfColor[1],
			&cColor.pfColor[2]) )
			break;
		vcColors.push_back(cColor);
	}
	fclose(fpColor);

	// read the opacity map
	fgets(szBuf, sizeof(szBuf)-1, fpOpacity);
	while( true ) 
	{
		CAlpha cAlpha;
		if( 2 != fscanf(fpOpacity, "%f %f\n", &cAlpha.fScalar, &cAlpha.fAlpha) )
			break;
		vcOpacities.push_back(cAlpha);
	}
	fclose(fpOpacity);

	int i, c;
	vector<CColor3D>::iterator pvcColor, pvcPrevColor;
	vector<CAlpha>::iterator pvcAlpha, pvcPrevAlpha;

	pvcColor = vcColors.begin();
	pvcAlpha = vcOpacities.begin();
	for(i=1; i<(int)uNrOfEntries; i++)
	{
		float fValue = fScalarMin + (float)(i-1)*(fScalarMax - fScalarMin)/(float)(uNrOfEntries-1);

		for(; pvcColor!=vcColors.end(); pvcColor++) 
		{
			if( fValue < pvcColor->fScalar )
				break;
			pvcPrevColor = pvcColor;
		}

		for(; pvcAlpha!=vcOpacities.end(); pvcAlpha++) 
		{
			if( fValue < pvcAlpha->fScalar )
				break;
			pvcPrevAlpha = pvcAlpha;
		}

		if( pvcColor == vcColors.end() )
			for(c=0; c<3; c++)
				pfEntries[4 * i + c] = 0.0f;
		else
			for(c=0; c<3; c++)
				pfEntries[4 * i + c] = 
					pvcPrevColor->pfColor[c] + 
					(pvcColor->pfColor[c] - pvcPrevColor->pfColor[c])*
					(fValue - pvcPrevColor->fScalar)/(pvcColor->fScalar - pvcPrevColor->fScalar);

		c = 3;
		if( pvcAlpha == vcOpacities.end() )
			pfEntries[4 * i + c] = 0.0f;
		else
			pfEntries[4 * i + c] = 
				pvcPrevAlpha->fAlpha + 
				(pvcAlpha->fAlpha - pvcPrevAlpha->fAlpha)*
				(fValue - pvcPrevAlpha->fScalar)/(pvcAlpha->fScalar - pvcPrevAlpha->fScalar);
	}

	return true;
}

void
CTransferFunction1D::_ConvertToAssociateColor()
{
	size_t uE, uC;
	for(uE=0; uE<uNrOfEntries; uE++)
		for(uC=0; uC<3; uC++) 
			pfEntries[4 * uE + uC] *= pfEntries[4 * uE + 3];
}

bool 
CTransferFunction1D::BPreIntegrate(size_t uDepthInterval)
{
	if( fScalarMin > fScalarMax || !pfEntries ) 
	{
		fprintf(stderr, "Warning! _Init() and BLoad() should be called before BPreIntegrate()\n");
		return false;
	}

	if( !uDepthInterval )
	{
		fprintf(stderr, "Warning! %s(%d) Invalid uNrOfDepthIntervals\n", __FILE__, __LINE__);
		return false;
	}

	uNrOfDepthIntervals = uDepthInterval;

	// allocate the space
	assert( pfPreIntegratedTable3D = (float*)calloc(4 * sizeof(float), uNrOfEntries * uNrOfEntries * uNrOfDepthIntervals) );

	size_t uP;
	int iF, iB, iD, iC, iI;	// index to front, back, detph, color and interval
	double dDepthInterval;

	fprintf(stderr, "Generating Pre. Integrated TF\n");

	double dDepth;
	double pdOut[4];
	double dAlpha;
	for( uP=0, iD=0; iD<(int)uNrOfDepthIntervals; iD++) 
	{
		dDepthInterval = (double)iD / (double)(uNrOfDepthIntervals-1);
		
		for(iF=0; iF<(int)uNrOfEntries; (int)iF++)
		{
			for(iB=0; iB<(int)uNrOfEntries; (int)iB++, uP++)
			{
				fprintf(stderr, "(%4d, %4d, %4d)\r", iD, iF, iB);

				if ( !iD || (!iF && !iB) )
					continue;

				memset(pdOut, 0, sizeof(pdOut));

				if( iF != iB )
					dDepth = dDepthInterval / (double)abs(iF - iB);

				if( iF < iB )
				{
					for(iI=iF+1; iI<=iB; iI++) 
					{
						double pdIn[4];

						// initial the alpha of the coming fragment
						pdIn[3] = 1.0 - pow(1.0 - (double)pfEntries[4 * iI + 3], dDepth);

						// initial the assocaited color of the coming fragment
						for(iC=0; iC<3; iC++)
							pdIn[iC] = pdIn[3] * (double)pfEntries[4 * iI + iC];

						// add the fragment to the framebuffer
						dAlpha = pdOut[3];
						for(iC=0; iC<4; iC++)
							pdOut[iC] += pdIn[iC] * (1.0 - dAlpha);
					} // for iI
				} 

				if( iF > iB )
				{
					for(iI=iF-1; iI>=iB; iI--) 
					{
						double pdIn[4];

						// initialize the alpha of the coming fragment
						pdIn[3] = 1.0 - pow(1.0 - (double)pfEntries[4 * iI + 3], dDepth);

						// initialize the assocaited color of the coming fragment
						for(iC=0; iC<3; iC++)
							pdIn[iC] = pdIn[3] * (double)pfEntries[4 * iI + iC];

						// add the fragment to the framebuffer
						dAlpha = pdOut[3];
						for(iC=0; iC<4; iC++)
							pdOut[iC] += pdIn[iC] * (1.0 - dAlpha);
					} // for iI
				}

				if( iF == iB )
				{
					pdOut[3] = 1.0 - pow(1.0 - (double)pfEntries[4 * iB + 3], dDepthInterval);
					for(iC=0; iC<3; iC++)
						pdOut[iC] = pdOut[3] * (double)pfEntries[4 * iB + iC];
				}

				// copy the integration result to the trasnfer function
				for(iC=0; iC<4;iC++)
					pfPreIntegratedTable3D[uP*4+iC] = (float)pdOut[iC];
			} // for iB
		} // for iF
	} // for iD
	fprintf(stderr, "\nDone\n");

	return true;
}

// ADD-BY-LEETEN 11/05/2006-BEGIN
// return the id of the allocated 3D texture
GLuint
CTransferFunction1D::UPreIntegrateOnGPU(size_t uDepthInterval)
{
	if( fScalarMin > fScalarMax || !pfEntries ) 
	{
		fprintf(stderr, "Warning! _Init() and BLoad() should be called before BPreIntegrate()\n");
		return false;
	}

	if( !uDepthInterval )
	{
		fprintf(stderr, "Warning! %s(%d) Invalid uNrOfDepthIntervals\n", __FILE__, __LINE__);
		return false;
	}

	uNrOfDepthIntervals = uDepthInterval;

	// check opengl extension
	if( !GLEW_EXT_framebuffer_object || 
		!GLEW_ARB_vertex_shader || 
		!GLEW_ARB_fragment_shader || 
		!GLEW_ARB_texture_non_power_of_two || 
		!GL_EXT_framebuffer_blit ||
		!GL_EXT_texture3D )
		return 0;

	GLuint uTid3D = 0;
	GLuint uFid = 0;	// id's of fbo's
	GLuint uRid2D;
	int iD;

	// save current viewport and setup viewport as the size for TF
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, uNrOfEntries, uNrOfEntries);

	// allocate the 3D texture
	glGenTextures(1, &uTid3D);
	glBindTexture(GL_TEXTURE_3D, uTid3D);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F_ARB,	
		uNrOfEntries, uNrOfEntries, uNrOfDepthIntervals, 0, GL_RGBA, GL_FLOAT, NULL);	

	// allocate the FBO to store the 2D texture
	glGenFramebuffersEXT(1, &uFid);	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, uFid); 

	// allocate the 2D render buffer
	glGenRenderbuffersEXT(1, &uRid2D);	// allocate the renderbuffer for the color component of the LDI's FBO
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, uRid2D);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA16F_ARB, uNrOfEntries, uNrOfEntries);

	// bind the render buffer to the fbo
	glFramebufferRenderbufferEXT(
		GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, 
		GL_RENDERBUFFER_EXT, uRid2D);

	/*
	glFramebufferTexture3DEXT(
		GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_3D, uTid3D, 0, 0);
	*/

	assert( GL_FRAMEBUFFER_COMPLETE_EXT == EGetFboStatus(false) );

	// since the value is always zero when depth is zero, case iD = 0 could be skipped
	for(iD=1; iD<uNrOfDepthIntervals; iD++)
	{
		// render a quad to trigger the fragment shader to compute the pre-integrated TF

		// copy the result to the slice of 3D texture
	}

	// deactivate current fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// restore the viewport
	glPopAttrib();	// glPushAttrib(GL_VIEWPORT_BIT);

	CHECK_OPENGL_ERROR("CTransferFunction1D::UPreIntegrateOnGPU():", false);

	return uTid3D;
}

bool 
CTransferFunction1D::BPreIntegrateIncrementally(size_t uDepthInterval)
{
	if( fScalarMin > fScalarMax || !pfEntries ) 
	{
		fprintf(stderr, "Warning! _Init() and BLoad() should be called before BPreIntegrate()\n");
		return false;
	}

	if( !uDepthInterval )
	{
		fprintf(stderr, "Warning! %s(%d) Invalid uNrOfDepthIntervals\n", __FILE__, __LINE__);
		return false;
	}

	uNrOfDepthIntervals = uDepthInterval;

	// allocate the space
	assert( pfPreIntegratedTable3D = (float*)calloc(4 * sizeof(float), uNrOfEntries * uNrOfEntries * uNrOfDepthIntervals) );

	size_t uP;
	int iF, iB, iD, iC, iI;	// index to front, back, detph, color and interval

	fprintf(stderr, "Generating Pre. Integrated TF\n");

	double dDepth = 1.0 / (double)(uNrOfDepthIntervals-1);
	double pdOut[4], pdIn[4];	// the buffer to hold the associated color and the opacity
	double dAlpha;
	for( uP=0, iD=0; iD<(int)uNrOfDepthIntervals; iD++) 
	{
		fprintf(stderr, "Layer: %4d\r", iD);

		for(iF=0; iF<(int)uNrOfEntries; (int)iF++)
		{
			for(iB=0; iB<(int)uNrOfEntries; (int)iB++, uP++)
			{

				if ( !iD ) 
					continue;

				// ADD-BY-LEETEN 12/10/2006-BEGIN
				// if there is at least one zero scalar of the entry and exit points, set the integration as zero
				if( !iF || !iB )
					continue;
				// ADD-BY-LEETEN 12/10/2006-END

				// TEST if( 1 == iD || iF == iB )
				if( iF == iB )
				{
					pdOut[3] = 1.0 - pow(1.0 - (double)pfEntries[4 * iB + 3], dDepth * (double)iD);
					for(iC=0; iC<3; iC++)
						pdOut[iC] = pdOut[3] * (double)pfEntries[4 * iB + iC];

					for(iC=0; iC<4;iC++)
						pfPreIntegratedTable3D[uP*4+iC] = (float)pdOut[iC];
					continue;
				}

				// find the previous value
				/*
				// ------- F ---------- Prev --------- B -------->
				//				D - 1			 1	
				double dPrev = (double)iB + (double)(iF - iB) / (double)iD;
				int iPrev = (int)floor(dPrev);
				double dFrac = dPrev - (double)iPrev;
				int iIndex;

				iIndex = iPrev + iF * uNrOfEntries + (iD - 1) * uNrOfEntries * uNrOfEntries;	// the index inside the 3d texture
				for(iC=0; iC<4; iC++) 
					pdOut[iC] = pfPreIntegratedTable3D[4*iIndex+iC] * (1.0 - dFrac) + pfPreIntegratedTable3D[4*(iIndex+1)+iC] * dFrac;

				pdIn[3] = 1.0 - pow(1.0 - (double)pfEntries[4 * iB + 3], dDepth);
				for(iC=0; iC<3; iC++)
					pdIn[iC] = pdIn[3] * (double)pfEntries[4 * iB + iC];
				*/
				
				// /* TEST
				// ------- F ---------- Prev --------- B -------->
				//				1				D - 1	
				double dPrev = (double)iF + (double)(iB - iF) / (double)iD;
				int iPrev = (int)floor(dPrev);
				double dFrac = dPrev - (double)iPrev;
				int iIndex;

				iIndex = iB + iPrev * uNrOfEntries + (iD - 1) * uNrOfEntries * uNrOfEntries;	// the index inside the 3d texture
				for(iC=0; iC<4; iC++) 
					pdIn[iC] = pfPreIntegratedTable3D[4*iIndex+iC] * (1.0 - dFrac) + pfPreIntegratedTable3D[4*(iIndex+uNrOfEntries)+iC] * dFrac;

				double pdTemp[4];
				for(iC=0; iC<4; iC++) 
					pdTemp[iC] = pfEntries[4*iPrev+iC] * (1.0 - dFrac) + pfEntries[4*(iPrev+1)+iC] * dFrac;

				pdOut[3] = 1.0 - pow(1.0 - (double)pdTemp[3], dDepth);
				for(iC=0; iC<3; iC++)
					pdOut[iC] = pdOut[3] * (double)pdTemp[iC];
				// */

				// add the fragment to the framebuffer
				dAlpha = pdOut[3];
				for(iC=0; iC<4; iC++)
					pdOut[iC] += pdIn[iC] * (1.0 - dAlpha);

				// copy the integration result to the trasnfer function
				for(iC=0; iC<4;iC++)
					pfPreIntegratedTable3D[uP*4+iC] = (float)pdOut[iC];
			} // for iB
		} // for iF
	} // for iD
	fprintf(stderr, "\nDone\n");

	return true;
}

// ADD-BY-LEETEN 11/05/2006-END

/*

$Log: not supported by cvs2svn $

*/
