#pragma once

#include <stdio.h>

//! read the entire file stream as a long string. 
/*! 
NOTE:  The caller should free the return string
*/
char*
SzTextFileRead
(
	const char *fn
) 
{
	char *content = NULL;
	if (fn != NULL) {
		FILE *fp = fopen(fn,"rt");

		if (fp != NULL) 
		{
			fseek(fp, 0, SEEK_END);
			size_t count = (size_t)ftell(fp);
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
