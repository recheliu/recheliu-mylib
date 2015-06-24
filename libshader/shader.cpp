	#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "libfilestr.h"

#include "shader.h"

GLhandleARB 
HCreateProgramHandle
(
)
{
	GLhandleARB 
		hProgramHandle = glCreateProgramObjectARB();
	return hProgramHandle;
}

void
_AddShaderProgram
(
	GLhandleARB hProgramHandle,
	int iShader,
	const char* szProgSrc
)
{
	GLhandleARB hShader;
	hShader = glCreateShaderObjectARB(iShader);
	glShaderSourceARB(hShader, 1, &szProgSrc, NULL);
	glCompileShaderARB(hShader);
	bool bIsObjectChecked = BCheckObject(hShader);
	assert( true == bIsObjectChecked );

	glAttachObjectARB(hProgramHandle, hShader);
}

void
_LinkPrograms
(
	GLhandleARB hProgramHandle
)
{
	glLinkProgramARB(hProgramHandle);
	bool bIsObjectChecked = BCheckObject(hProgramHandle);
	assert( true == bIsObjectChecked );
}

GLhandleARB 
CSetShadersByString(
	const char* szVertexProg, 
	const char* szFragmentProg)
{
	GLhandleARB hProgramHandle = HCreateProgramHandle();
	if( szVertexProg )		_AddShaderProgram(hProgramHandle, GL_VERTEX_SHADER_ARB,		szVertexProg);
	if( szFragmentProg )	_AddShaderProgram(hProgramHandle, GL_FRAGMENT_SHADER_ARB,	szFragmentProg);
	_LinkPrograms(hProgramHandle);
	return hProgramHandle;
}	

bool
BCheckObject(GLhandleARB obj)
{
	int infologLength = 0;
	int charsWritten  = 0;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
					 &infologLength);

	if (infologLength > 1)
	{
		char *infoLog = (char *)malloc(infologLength);
		glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		fprintf(stderr, "%s\n",infoLog);
		bool bNoError = (strstr(infoLog, "ERROR")||strstr(infoLog, "error"))?false:true;
		free(infoLog);
		return bNoError;
	}
	return true;
}

GLhandleARB 
CSetShaders(const char* szVertex, const char* szFragment) 
{
	char *vs = NULL, *fs = NULL;
	static char szShaderPath[128+1];
//	char* szFileContext;
//	const char* szFileContext2;
	GLhandleARB hFragmentShader, hVertexShader;

	GLhandleARB 
		hProgramHandle = glCreateProgramObjectARB();

	if( szVertex ) 
	{
		char* szFileContext;

		strcpy(szShaderPath, szVertex);
		fprintf(stderr, "Parsing %s\n", szShaderPath);
		if( !(szFileContext = SzTextFileRead(szShaderPath)) )
		{
			fprintf(stderr, "Error: could not find vertex shader %s\n", szShaderPath);
			return false;
		}
		hVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		const char* szFileContext2 = szFileContext;
		glShaderSourceARB(hVertexShader, 1, &szFileContext2, NULL);
		glCompileShaderARB(hVertexShader);
		if( !BCheckObject(hVertexShader) ) 
		{
			fprintf(stderr, "Error during parsing %s\n", szShaderPath);
			return false;
		}
		free(szFileContext);
	}

	if( szFragment ) 
	{
		char* szFileContext;

		strcpy(szShaderPath, szFragment);
		fprintf(stderr, "Parsing %s\n", szShaderPath);
		if( !(szFileContext = SzTextFileRead(szShaderPath)) )
		{
			fprintf(stderr, "Error: could not find fragment shader %s\n", szShaderPath);
			return false;
		}
		hFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		const char* szFileContext2 = szFileContext;
		glShaderSourceARB(hFragmentShader, 1, &szFileContext2, NULL);
		glCompileShaderARB(hFragmentShader);
		if( !BCheckObject(hFragmentShader) ) 
		{
			fprintf(stderr, "Error during compiling %s\n", szShaderPath);
			return false;
		}
		free(szFileContext);
	}

	if( szVertex )
		glAttachObjectARB(hProgramHandle, hVertexShader);

	if( szFragment )
		glAttachObjectARB(hProgramHandle, hFragmentShader);

	glLinkProgramARB(hProgramHandle);

	if( !BCheckObject(hProgramHandle) )
	{
		fprintf(stderr, "Error during linking shaders\n");
		return false;
	}
//	free(szFileContext);

	return hProgramHandle;
}

