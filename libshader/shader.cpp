#include <gl/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

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