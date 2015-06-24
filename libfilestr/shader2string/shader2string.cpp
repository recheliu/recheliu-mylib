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

	fputc('\n', fpShaderHeader);

	fclose(fpShaderHeader);
	free(szShaderContext);

	fprintf(stderr, "%s is converted into %s\n", argv[1], argv[2]);

	return 0;

}

