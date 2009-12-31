#ifndef __LIB_LOG_H__
#define __LIB_LOG_H__

// ADD-BY-LEETEN 12/30/2009-BEGIN
#include <iostream>
using namespace std;
// ADD-BY-LEETEN 12/30/2009-END


#include <stdio.h>
#include <stdarg.h>

#ifdef WIN32
	#include <string.h>
	#define PRINT_HEADER	printf("[%s() @ %s(%d)]: ", __FUNCTION__, strrchr(__FILE__, '\\') + 1, __LINE__)	
#else
	#define PRINT_HEADER	printf("[%s()@%s(%d)]: ", __FUNCTION__, __FILE__, __LINE__)	
#endif

#define	LOG_VAR(var)					\
	{									\
		PRINT_HEADER;					\
		cout<<#var<<" = "<<(var)<<endl;	\
	}									\


#define LOG(printf_stmt)	\
	{						\
		PRINT_HEADER;		\
		(printf_stmt);		\
		printf("\n");		\
	}						\


#endif	// __LIB_LOG_H__

/*

$Log: not supported by cvs2svn $
Revision 1.1  2009/06/03 21:56:54  leeten

[2009/06/03]
1. [1ST] This project define a library to log out statement and variables.


*/
