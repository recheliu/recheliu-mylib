#ifndef __LIB_LOG_H__
#define __LIB_LOG_H__

#include <iostream>
using namespace std;


#include <stdio.h>
#include <stdarg.h>

#ifdef WIN32
	#include <string.h>
#endif
#define PRINT_HEADER	{	\
		const char *szFileSlash = strrchr(__FILE__, '\\');				\
		const char *szFile = (NULL == szFileSlash)?(__FILE__):(szFileSlash + 1);	\
		cout<<"["<<__FUNCTION__<<"()@"<<szFile<<"("<<__LINE__<<")]: ";			\
	}

#if 0 // TEST-DEL
#define	LOG_VAR(var)					\
	{									\
		PRINT_HEADER;					\
		cout<<#var<<": "<<(var)<<endl;	\
	}									\

#endif

#define LOG(printf_stmt)	\
	{						\
		PRINT_HEADER;		\
		(printf_stmt);		\
		cout<<endl;			\
	}						

#ifdef WIN32
	#include <string.h>
#endif

#define PRINT_ERROR_HEADER	{	\
		const char *szFileSlash = strrchr(__FILE__, '\\');				\
		const char *szFile = (NULL == szFileSlash)?(__FILE__):(szFileSlash + 1);	\
		cerr<<"["<<__FUNCTION__<<"()@"<<szFile<<"("<<__LINE__<<")]: ";			\
	}

#define LOG_ERROR(printf_stmt)	\
	{						\
		PRINT_ERROR_HEADER;		\
		(printf_stmt);		\
		cerr<<endl;			\
	}						

#define LOG_VAR_TO_ERROR(var)	\
	{						\
		PRINT_ERROR_HEADER;		\
		cerr<<#var<<": "<<(var)<<endl;	\
	}						

#define ASSERT_OR_LOG(condition, printf_stmt)		\
	{						\
		if( ! (condition) )			\
		{					\
			char *ASSERT = #condition;	\
			LOG_VAR_TO_ERROR(ASSERT);	\
			LOG_ERROR(printf_stmt);		\
			exit(EXIT_FAILURE);		\
		}					\
	}						\

#define LOG_IF(condition, printf_stmt)		\
	{						\
		if( (condition) )			\
		{					\
			char *COND = #condition;	\
			LOG_VAR(COND);		\
			LOG(printf_stmt);		\
		}					\
	}						\

#if 1	// TEST-ADD
#include "logvar.h"
#endif

#endif	// __LIB_LOG_H__

