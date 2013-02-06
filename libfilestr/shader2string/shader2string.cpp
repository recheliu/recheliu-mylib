/*

Shader2String.cpp

This application is used to convert a shader program into a .h that contains its context as a multiple line string


*/

#include <stdio.h>
#include <stdlib.h>

#include "libfilestr.h"

int
main(int argn, char * argv[])
{
	if( argn < 3 )
	{
		fprintf(stderr, "Usage: Shader2String <shader_prog> <shader_prog.h>\n");
		return -1;
	}

	char *szShaderContext = SzTextFileRead(argv[1]);
	if( !szShaderContext )
	{
		fprintf(stderr, "Error: %s cannot be opened\n", argv[1]);
		return -1;
	}

	FILE *fpShaderHeader = fopen(argv[2], "wt");
	if( !fpShaderHeader )
	{
		fprintf(stderr, "Error: %s cannot be opened\n", argv[2]);
		return -1;
	}

	fputc('"', fpShaderHeader);

	char *pc; 
	for(pc = szShaderContext; *pc; pc++)
	{
		switch( *pc )
		{
		case '"':
			fputc('\\', fpShaderHeader);
			fputc(*pc, fpShaderHeader);
			break;

		case '\r':
			continue;

		case '\\':
			fputc('\\', fpShaderHeader);
			fputc('\\', fpShaderHeader);
			break;

		case '\n':
			fputc('\\',	fpShaderHeader);
			fputc('n',	fpShaderHeader);
			fputc('"',	fpShaderHeader);
			fputc('\n', fpShaderHeader);
			fputc('"',	fpShaderHeader);
			break;

		default:
			fputc(*pc, fpShaderHeader);
		}
	}
					// if the last char is a newline, a quote has been wrote, and another quote is wrote too.
	pc--;
	if( '\n' == *pc )
		fputc('"', fpShaderHeader);

	// ADD-BY-LEETEN 2009/06/01-BEGIN
	fputc('\n', fpShaderHeader);
	// ADD-BY-LEETEN 2009/06/01-END


	fclose(fpShaderHeader);
	free(szShaderContext);

	fprintf(stderr, "%s is converted into %s\n", argv[1], argv[2]);

	return 0;

}

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/15 02:16:03  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN] This application is used to convert a ASCII file into a C/C++ multiple line string. The main purpos is used to embed a shader program into the source code.


*/
