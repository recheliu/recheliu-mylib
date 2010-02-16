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
	// MOD-BY-LEETEN 02/16/2010-FROM:
		// #define PRINT_HEADER	printf("[%s() @ %s(%d)]: ", __FUNCTION__, strrchr(__FILE__, '\\') + 1, __LINE__)	
	// TO:
	#define PRINT_HEADER	cout<<"["<<__FUNCTION__<<"() @ "<<strrchr(__FILE__, '\\') + 1<<"("<<__LINE__<<")]: ";
	// MOD-BY-LEETEN 02/16/2010-END
#else
	// MOD-BY-LEETEN 02/16/2010-FROM:
		// #define PRINT_HEADER	printf("[%s()@%s(%d)]: ", __FUNCTION__, __FILE__, __LINE__)	
	// TO:
	#define PRINT_HEADER	cout<<"["<<__FUNCTION__<<"() @ "<<__FILE__<<"("<<__LINE__<<")]: ";
	// MOD-BY-LEETEN 02/16/2010-END
#endif

#define	LOG_VAR(var)					\
	{									\
		PRINT_HEADER;					\
		cout<<#var<<" = "<<(var)<<endl;	\
	}									\

#if	0	// MOD-BY-LEETEN 02/16/2010-FROM:
	#define LOG(printf_stmt)	\
		{						\
			PRINT_HEADER;		\
			(printf_stmt);		\
			printf("\n");		\
		}						\

#else	// MOD-BY-LEETEN 02/16/2010-TO:

#define LOG(printf_stmt)	\
	{						\
		PRINT_HEADER;		\
		(printf_stmt);		\
		cout<<endl;			\
	}						

#endif	// MOD-BY-LEETEN 02/16/2010-END

#endif	// __LIB_LOG_H__

/*

$Log: not supported by cvs2svn $
Revision 1.2  2009/12/31 01:45:35  leeten

[12/30/2009]
1. [ADD] Include the header iostream and use the namespace std.

Revision 1.1  2009/06/03 21:56:54  leeten

[2009/06/03]
1. [1ST] This project define a library to log out statement and variables.


*/
