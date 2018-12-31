#pragma once

#if defined(GOOGLE_LOG_INFO)
	#define	LOG_VAR(var)					LOG(INFO)	<< #var <<": " << (var)

#else
	#define	LOG_VAR(var)					\
		{									\
			PRINT_HEADER;					\
			cout<<#var<<": "<<(var)<<endl;	\
		}									\

#endif
