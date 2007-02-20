#ifndef __SHADER__H__
#define __SHADER__H__

#include <GL/glew.h>
#include <GL/glut.h>

bool BCheckObject(GLhandleARB obj);
GLhandleARB CSetShaders(const char* szVertex, const char* szFragment);

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

// ADD-BY-LEETY 09/15/2006-END

#endif	// __SHADER__H__

/*

  $Log: not supported by cvs2svn $
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
