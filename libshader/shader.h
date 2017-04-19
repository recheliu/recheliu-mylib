#ifndef __SHADER__H__
#define __SHADER__H__

#include <GL/glew.h>

#include <string>
#include <stdlib.h>
#include <assert.h>

GLhandleARB CSetShadersByString(
	const char* szVertexProg, 
	const char* szFragmentProg);

bool BCheckObject(GLhandleARB obj);
GLhandleARB CSetShaders(const char* szVertex, const char* szFragment);

GLhandleARB HCreateProgramHandle();
void _AddShaderProgram(GLhandleARB hProgramHandle, int iShader, const char* szProgSrc);
void _LinkPrograms(GLhandleARB hProgramHandle);

void _NameStringAsInclude(const std::string& strName, const std::string& strContent);

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

#define SET_4FV_VALUE_BY_NAME(SHADER, NAME, LEN, PVALUES)	\
	{	\
		int iLoc;	\
		GET_LOC(SHADER, NAME, iLoc);	\
		SET_4FV_VALUE_BY_LOC(iLoc, LEN, PVALUES);	\
	}

#define SET_MAT4FV_VALUE_BY_LOC(LOC, LEN, TRANSPOSE, PVALUES)	\
	{	\
		if( (LOC) < 0 )	\
			PRINT_WARNING_INVALID_LOC(LOC)	\
		else	\
			glUniformMatrix4fvARB(LOC, LEN, TRANSPOSE, PVALUES);	\
	}
	
#define SET_MAT4FV_VALUE_BY_NAME(SHADER, NAME, LEN, TRANSPOSE, PVALUES)	\
	{	\
		int iLoc;	\
		GET_LOC(SHADER, NAME, iLoc);	\
		SET_MAT4FV_VALUE_BY_LOC(iLoc, LEN, TRANSPOSE, PVALUES);	\
	}

#endif	// __SHADER__H__

