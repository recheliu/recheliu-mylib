#ifndef __SHADER__H__
#define __SHADER__H__

#include <GL/glew.h>

// ADD-BY-LEETEN 01/10/2010-BEGIN
#include <stdlib.h>
#include <assert.h>
// ADD-BY-LEETEN 01/10/2010-END

// ADD-BY-LEETEN 08/14/2008-BEGIN
char *SzTextFileRead(char *fn);

// MOD-BY-LEETEN 01/10/2010-FROM:
	// GLhandleARB CSetShadersByString(const char* szVertexProg, const char* szFragmentProg);
// TO:
#if	0	// MOD-BY-LEETEN 01/12/2010-FROM:
	GLhandleARB CSetShadersByString(
		const char* szVertexProg, 
		const char* szFragmentProg, 
		const char* szGeometryProg = NULL);
#else	// MOD-BY-LEETEN 01/12/2010-TO:
GLhandleARB CSetShadersByString(
	const char* szVertexProg, 
	const char* szFragmentProg);
#endif	// MOD-BY-LEETEN 01/12/2010-END
// MOD-BY-LEETEN 01/10/2010-END
// ADD-BY-LEETEN 08/14/2008-END

bool BCheckObject(GLhandleARB obj);
GLhandleARB CSetShaders(const char* szVertex, const char* szFragment);

// ADD-BY-LEETEN 01/10/2010-BEGIN
GLhandleARB HCreateProgramHandle();
void _AddShaderProgram(GLhandleARB hProgramHandle, int iShader, const char* szProgSrc);
void _LinkPrograms(GLhandleARB hProgramHandle);
// ADD-BY-LEETEN 01/10/2010-END

// ADD-BY-LEETY 09/15/2006-BEGIN
#define PRINT_WARNING_INVALID_LOC(loc)	\
	{	\
		fprintf(stderr, "%s(%d) Warning: location %s is invalid.\n", __FILE__, __LINE__, #loc);	\
	}	

#define GET_LOC(SHADER, NAME, VAR)	\
	{	\
		if( (VAR = glGetUniformLocationARB(SHADER, NAME))<0 ) \
			fprintf(stderr, "%s(%d) Warning: variable %s is not found.\n", __FILE__, __LINE__, NAME);\
	}	

#define SET_1I_VALUE_BY_LOC(LOC, VALUE)	\
	{	\
		if( (LOC) < 0 )	\
			PRINT_WARNING_INVALID_LOC(LOC)	\
		else	\
			glUniform1iARB(LOC, VALUE);	\
	}

#define SET_1IV_VALUE_BY_LOC(LOC, LEN, PVALUES)	\
	{	\
		if( (LOC) < 0 )	\
			PRINT_WARNING_INVALID_LOC(LOC)	\
		else	\
			glUniform1ivARB(LOC, LEN, PVALUES);	\
	}

#define SET_1F_VALUE_BY_LOC(LOC, VALUE)	\
	{	\
		if( (LOC) < 0 )	\
			PRINT_WARNING_INVALID_LOC(LOC)	\
		else	\
			glUniform1fARB(LOC, VALUE);	\
	}

#define SET_2FV_VALUE_BY_LOC(LOC, LEN, PVALUES)	\
	{	\
		if( (LOC) < 0 )	\
			PRINT_WARNING_INVALID_LOC(LOC)	\
		else	\
			glUniform2fvARB(LOC, LEN, PVALUES);	\
	}

#define SET_3FV_VALUE_BY_LOC(LOC, LEN, PVALUES)	\
	{	\
		if( (LOC) < 0 )	\
			PRINT_WARNING_INVALID_LOC(LOC)	\
		else	\
			glUniform3fvARB(LOC, LEN, PVALUES);	\
	}

#define SET_4FV_VALUE_BY_LOC(LOC, LEN, PVALUES)	\
	{	\
		if( (LOC) < 0 )	\
			PRINT_WARNING_INVALID_LOC(LOC)	\
		else	\
			glUniform4fvARB(LOC, LEN, PVALUES);	\
	}

#define SET_1I_VALUE_BY_NAME(SHADER, NAME, VALUE)	\
	{	\
		int iLoc;	\
		GET_LOC(SHADER, NAME, iLoc);	\
		SET_1I_VALUE_BY_LOC(iLoc, VALUE);	\
	}

#define SET_1F_VALUE_BY_NAME(SHADER, NAME, VALUE)	\
	{	\
		int iLoc;	\
		GET_LOC(SHADER, NAME, iLoc);	\
		SET_1F_VALUE_BY_LOC(iLoc, VALUE);	\
	}

#define SET_1IV_VALUE_BY_NAME(SHADER, NAME, LEN, PVALUES)	\
	{	\
		int iLoc;	\
		GET_LOC(SHADER, NAME, iLoc);	\
		SET_1IV_VALUE_BY_LOC(iLoc, LEN, PVALUES);	\
	}

#define SET_3FV_VALUE_BY_NAME(SHADER, NAME, LEN, PVALUES)	\
	{	\
		int iLoc;	\
		GET_LOC(SHADER, NAME, iLoc);	\
		SET_3FV_VALUE_BY_LOC(iLoc, LEN, PVALUES);	\
	}

// ADD-BY-LEETEN 2009/08/17-BEGIN
#define SET_4FV_VALUE_BY_NAME(SHADER, NAME, LEN, PVALUES)	\
	{	\
		int iLoc;	\
		GET_LOC(SHADER, NAME, iLoc);	\
		SET_4FV_VALUE_BY_LOC(iLoc, LEN, PVALUES);	\
	}
// ADD-BY-LEETEN 2009/08/17-END

// ADD-BY-LEETY 09/15/2006-END

#endif	// __SHADER__H__

/*

  $Log: not supported by cvs2svn $
  Revision 1.7  2010/01/11 18:58:10  leeten

  [01/10/2010]
  1. [ADD] Declare a new function HCreateProgramHandle to obtain the program handle.
  2. [ADD] Declare a new function _AddShaderProgram to add a shader program to the program handle.
  3. [ADD] Declare a new function _LinkPrograms to link and create the final program handle.

  Revision 1.6  2010/01/11 18:55:36  leeten

  [01/10/2010]
  1. [ADD] Declare a new function HCreateProgramHandle to obtain the program handle.
  2. [ADD] Declare a new function _AddShaderProgram to add a shader program to the program handle.
  3. [ADD] Declare a new function _LinkPrograms to link and create the final program handle.

  Revision 1.5  2009/08/17 11:20:12  leeten

  [2009/08/16]
  1. [ADD] Define a new macro SET_4FV_VALUE_BY_NAME.

  Revision 1.4  2008/08/15 02:18:22  leeten

  [2008/08/14]
  1. [ADD] Declare a new function CSetShadersByString to compile shader progoams as long strings.
  2. [ADD] Export the function SzTextFileRead as a function.

  Revision 1.3  2007/03/12 23:40:22  leeten

  [03/12/2007]
  1. Remove included header glut.h.

  Revision 1.2  2007/02/20 18:03:53  leeten

  [02/20/2007]
  1. Output the name of the invalid variable in macro PRINT_WARNING_INVALID_LOC.

  Revision 1.1.1.1  2006/11/09 19:20:04  leeten

  [11/09/2006]
  1. First time checkin.

  Revision 1.3  2006/09/22 13:52:58  leeten

  [09/22/2006]
  1. Define new macro SET_3FV_VALUE_BY_LOC.

  Revision 1.2  2006/09/15 20:12:43  leeten

  [09/15/2006]
  1. Define new macros to simplify the assignment of uniform variables in shader program.

  Revision 1.1.1.1  2006/09/13 19:49:26  leeten

  [09/13/2006]
  1. First time checkin. It is a new version renderere which simulate 4 slices at the same time.


*/
