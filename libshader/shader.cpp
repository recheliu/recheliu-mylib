#include <gl/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

// ADD-BY-LEETEN 08/14/2008-BEGIN
#include "shader.h"

// ADD-BY-LEETEN 01/10/2010-BEGIN
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
	assert( true == BCheckObject(hShader) );
	glAttachObjectARB(hProgramHandle, hShader);
}

void
_LinkPrograms
(
	GLhandleARB hProgramHandle
)
{
	glLinkProgramARB(hProgramHandle);
	assert( true == BCheckObject(hProgramHandle) );
}
// ADD-BY-LEETEN 01/10/2010-END

#if	0	// MOD-BY-LEETEN 01/10/2010-FROM:
	GLhandleARB 
	CSetShadersByString(
		const char* szVertexProg, 
		const char* szFragmentProg)
	{
		GLhandleARB hFragmentShader, hVertexShader;

		GLhandleARB 
			hProgramHandle = glCreateProgramObjectARB();

		if( szVertexProg ) 
		{
			hVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
			glShaderSourceARB(hVertexShader, 1, &szVertexProg, NULL);
			glCompileShaderARB(hVertexShader);
			if( !BCheckObject(hVertexShader) ) 
			{
				fprintf(stderr, "Error during parsing the vertex program.\n");
				return false;
			}
		}

		if( szFragmentProg ) 
		{
			hFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
			glShaderSourceARB(hFragmentShader, 1, &szFragmentProg, NULL);
			glCompileShaderARB(hFragmentShader);
			if( !BCheckObject(hFragmentShader) ) 
			{
				fprintf(stderr, "Error during compiling the fragment program.\n");
				return false;
			}
		}

		if( szVertexProg )
			glAttachObjectARB(hProgramHandle, hVertexShader);

		if( szFragmentProg )
			glAttachObjectARB(hProgramHandle, hFragmentShader);

		glLinkProgramARB(hProgramHandle);

		if( !BCheckObject(hProgramHandle) )
		{
			fprintf(stderr, "Error during linking shaders\n");
			return false;
		}

		return hProgramHandle;
	}

#else	// MOD-BY-LEETEN 01/10/2010-TO:

GLhandleARB 
CSetShadersByString(
	const char* szVertexProg, 
	const char* szFragmentProg,
	const char* szGeometryProg)
{
	GLhandleARB hProgramHandle = HCreateProgramHandle();
	if( szVertexProg )		_AddShaderProgram(hProgramHandle, GL_VERTEX_SHADER_ARB,		szVertexProg);
	if( szFragmentProg )	_AddShaderProgram(hProgramHandle, GL_FRAGMENT_SHADER_ARB,	szFragmentProg);
	if( szGeometryProg )	_AddShaderProgram(hProgramHandle, GL_GEOMETRY_SHADER_EXT,	szGeometryProg);
	_LinkPrograms(hProgramHandle);
	return hProgramHandle;
}
#endif	// MOD-BY-LEETEN 01/10/2010-END
// ADD-BY-LEETEN 08/14/2008-END

char *SzTextFileRead(char *fn) 
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
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

/*

  $Log: not supported by cvs2svn $
  Revision 1.6  2010/01/11 18:54:25  leeten

  [01/10/2010]
  1. [ADD] Define a new function HCreateProgramHandle to obtain the program handle.
  2. [ADD] Defien a new function _AddShaderProgram to add a shader program to the program handle.
  3. [ADD] Defien a new function _LinkPrograms to link and create the final program handle.
  4. [MOD] Change the content of CSetShadersByString by using the new defined functions.

  Revision 1.5  2008/08/15 02:17:50  leeten

  [2008/08/14]
  1. [ADD] Define a new function CSetShadersByString to compile shader progoams as long strings.

  Revision 1.4  2007/09/18 16:17:38  leeten

  [09/18/2007]
  1. Changed the code to free the buffer for the shader programs.

  Revision 1.3  2007/03/12 23:40:37  leeten

  [03/12/2007]
  1. Add macro SET_3FV_VALUE_BY_NAME.

  Revision 1.2  2007/02/20 18:04:36  leeten

  [02/20/2007]
  1. Output the path of the shader program during parsing.

  Revision 1.1.1.1  2006/11/09 19:20:04  leeten

  [11/09/2006]
  1. First time checkin.

  Revision 1.1.1.1  2006/09/13 19:49:26  leeten

  [09/13/2006]
  1. First time checkin. It is a new version renderere which simulate 4 slices at the same time.


*/
