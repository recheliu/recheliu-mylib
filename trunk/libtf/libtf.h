/*

Name: 
	LIBTF.cpp

Description
	Declare functions and structures to load transfer function.
*/

#ifndef __LIB_TRANSFER_FUNCTION_H__
#define __LIB_TRANSFER_FUNCTION_H__

#include <GL/glew.h>

#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef struct CTransferFunction1D 
{
	size_t uNrOfEntries;
	size_t uNrOfDepthIntervals;
	float fScalarMin, fScalarMax; 
	float *pfEntries;
	float *pfPreIntegratedTable3D;

	CTransferFunction1D() 
	{
		uNrOfEntries = 0;
		uNrOfDepthIntervals = 0;
		pfEntries = NULL;
		pfPreIntegratedTable3D = NULL;
		fScalarMin = (float)HUGE_VAL;
		fScalarMax = -(float)HUGE_VAL;
	}

	~CTransferFunction1D() 
	{
		_Free();
	}

	void _Free() 
	{
		if( pfEntries )
			free(pfEntries);
		pfEntries = NULL;

		if( pfPreIntegratedTable3D )
			free(pfPreIntegratedTable3D);
		pfPreIntegratedTable3D = NULL;
	}

	void _Alloc()
	{
		_Free();
		if( uNrOfEntries )
			assert( pfEntries = (float*)calloc(4 * sizeof(pfEntries), uNrOfEntries) );
	}

	void _Init(size_t uEntries, float fMin, float fMax)
	{
		uNrOfEntries = uEntries;
		fScalarMin = fMin;
		fScalarMax = fMax;
		_Alloc();
	}

	// ADD-BY-LEETEN 01/03/2007-BEGIN
	// create a default traasfer function
	void _Default();

	// load the 1d lookup table to OpenGL
	GLuint ULoad1D()
	{
		GLuint uTidTf = 0;
		glGenTextures(1, &uTidTf);
		glBindTexture(GL_TEXTURE_1D, uTidTf);
		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, uNrOfEntries, 0, GL_RGBA, GL_FLOAT, pfEntries);

		GLint iError;
		if( (iError = glGetError()) )
		{			
			fprintf(stderr, "Error in CTransferFunction1D::ULoad1D(): %s\n", gluErrorString(iError));
			return 0;
		}
		return uTidTf;
	}

	// load the transfer function in camp format
	// the format is:
	// int_number_of_entries
	// float_r float_g float_b float_a
	bool BLoadCMap(const char* szCMapPathFilename);
	// ADD-BY-LEETEN 01/03/2007-BEGIN

	bool BLoad(const char* szColorMap, const char* szOpacityMap);
	void _ConvertToAssociateColor();
	bool BPreIntegrate(size_t uDepthIntervals);

	// ADD-BY-LEETEN 11/05/2006-BEGIN
	bool BPreIntegrateIncrementally(size_t uDepthIntervals);

	GLuint UPreIntegrateOnGPU(size_t uDepthIntervals);
	// ADD-BY-LEETEN 11/05/2006-END
} CTransferFunction1D;
#endif // __LIB_TRANSFER_FUNCTION_H__

/*

$Log: not supported by cvs2svn $

*/
