#pragma once

#ifndef MAX_NR_OF_TIMERS
	#define MAX_NR_OF_TIMERS	64
#endif

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment (lib, "winmm.lib")      // link with my own library libfps
	
	#if	0	// MOD-BY-LEETEN 02/24/2011-FROM:
		#define LIBCLOCK_INIT(flag, header)						\
			{															\
				const int _iMaxNrOfTimers = MAX_NR_OF_TIMERS;			\
				DWORD _pdBeginTimers[_iMaxNrOfTimers];					\
				DWORD _pdEndTimers[_iMaxNrOfTimers];					\
				size_t _t = 0;											\
				if( (flag)	)											\
				{														\
					printf("%s", header);								\
				}

	#else	// MOD-BY-LEETEN 02/24/2011-TO:
	#define LIBCLOCK_INIT(flag, header)						\
		{															\
			const int _iMaxNrOfTimers = MAX_NR_OF_TIMERS;			\
			DWORD _pdBeginTimers[_iMaxNrOfTimers];					\
			DWORD _pdEndTimers[_iMaxNrOfTimers];					\
			char* szHeader = header;								\
			size_t _t = 0;											

	#endif	// MOD-BY-LEETEN 02/24/2011-END

	#define LIBCLOCK_BEGIN(flag)								\
			if( (flag)	)											\
			{														\
				_pdBeginTimers[_t] = timeGetTime();					\
			}

	#define LIBCLOCK_END(flag)									\
			if( (flag)	)											\
			{														\
				_pdEndTimers[_t++] = timeGetTime();					\
				assert(_t < _iMaxNrOfTimers );						\
			}

	#if	0	// MOD-BY-LEETEN 02/24/2011-FROM:
		#define LIBCLOCK_PRINT(flag)									\
				if( (flag)	)										\
				{													\
					double dTotalTime = 0.0;							\
					for(size_t i=0; i<_t; i++)						\
					{												\
						double dTime = (double)(_pdEndTimers[i] - _pdBeginTimers[i]);	\
						printf("%.2f,", dTime);						\
						dTotalTime += dTime;						\
					}												\
					printf("%.2f\n", dTotalTime);					\
				}													\
			}

	#else	// MOD-BY-LEETEN 02/24/2011-TO:
	#define LIBCLOCK_PRINT(flag)									\
			if( (flag)	)											\
			{														\
				printf("%s: ", szHeader);								\
				double dTotalTime = 0.0;							\
				for(size_t i=0; i<_t; i++)							\
				{													\
					double dTime = (double)(_pdEndTimers[i] - _pdBeginTimers[i]);	\
					printf("%.2f,", dTime);							\
					dTotalTime += dTime;							\
				}													\
				printf("%.2f\n", dTotalTime);						\
			}														\
		}

	#endif	// MOD-BY-LEETEN 02/24/2011-END

#else	// #ifdef WIN32
	#include <time.h>
	// ADD-BY-LEETEN 09/13/2011-BEGIN
	#if defined(__APPLE__) && defined(__MACH__)
	#include <sys/time.h>
	#include <mach/mach.h>
	#include <mach/clock.h>

	#define CLOCK_REALTIME	0
	inline void clock_gettime(int unused, struct timespec* ts)
	{
		clock_serv_t cclock;
		mach_timespec_t mts;
		host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
		clock_get_time(cclock, &mts);
		mach_port_deallocate(mach_task_self(), cclock);
		ts->tv_sec = mts.tv_sec;
		ts->tv_nsec = mts.tv_nsec;
	}
	#else	// #if defined(__APPLE__) && defined(__MACH__)
	// ADD-BY-LEETEN 09/13/2011-END

	#pragma message("Remerber to add the argument '-l rt' to the linker options.")
	#endif	// #if defined(__APPLE__) && defined(__MACH__)		// ADD-BY-LEETEN 09/13/2011

	#if	0	// MOD-BY-LEETEN 02/24/2011-FROM:
		#define LIBCLOCK_INIT(flag, header)							\
			{																\
				static struct timespec _pcBeginTimers[MAX_NR_OF_TIMERS];	\
				static struct timespec _pcEndTimers[MAX_NR_OF_TIMERS];		\
				size_t _t = 0;												\
				if( (flag)	)												\
				{															\
					printf("%s", header);									\
				}

	#else
	#define LIBCLOCK_INIT(flag, header)							\
		{																\
			static struct timespec _pcBeginTimers[MAX_NR_OF_TIMERS];	\
			static struct timespec _pcEndTimers[MAX_NR_OF_TIMERS];		\
			size_t _t = 0;												\
			char* szHeader = header;									

	#endif

	#define LIBCLOCK_BEGIN(flag)								\
			if( (flag)	)											\
			{														\
				clock_gettime(CLOCK_REALTIME, &_pcBeginTimers[_t]);	\
			}

	#define LIBCLOCK_END(flag)									\
			if( (flag)	)											\
			{														\
				clock_gettime(CLOCK_REALTIME, &_pcEndTimers[_t++]);	\
				assert(_t < MAX_NR_OF_TIMERS);						\
			}

	#if	0	// MOD-BY-LEETEN 02/24/2011-FROM:
		#define LIBCLOCK_PRINT(flag)							\
				if( (flag)	)										\
				{													\
					double dTotalTime = 0.0;						\
					for(size_t i=0; i<_t; i++)						\
					{												\
						double dBeginTime = (double)_pcBeginTimers[i].tv_sec * 1.0e+3 + (double)_pcBeginTimers[i].tv_nsec/1.0e+6;	\
						double dEndTime = (double)_pcEndTimers[i].tv_sec * 1.0e+3 + (double)_pcEndTimers[i].tv_nsec/1.0e+6;			\
						double dTime = dEndTime - dBeginTime;		\
						printf("%.2f,", dTime);						\
						dTotalTime  += dTime;						\
					}												\
					printf("%.2f\n", dTotalTime);					\
				}													\
			}
	#else	// MOD-BY-LEETEN 02/24/2011-TO:
	#define LIBCLOCK_PRINT(flag)							\
			if( (flag)	)										\
			{													\
				printf("%s: ", szHeader);						\
				double dTotalTime = 0.0;						\
				for(size_t i=0; i<_t; i++)						\
				{												\
					double dBeginTime = (double)_pcBeginTimers[i].tv_sec * 1.0e+3 + (double)_pcBeginTimers[i].tv_nsec/1.0e+6;	\
					double dEndTime = (double)_pcEndTimers[i].tv_sec * 1.0e+3 + (double)_pcEndTimers[i].tv_nsec/1.0e+6;			\
					double dTime = dEndTime - dBeginTime;		\
					printf("%.2f,", dTime);						\
					dTotalTime  += dTime;						\
				}												\
				printf("%.2f\n", dTotalTime);					\
			}													\
		}
	#endif	// MOD-BY-LEETEN 02/24/2011-END

#endif	// #ifdef WIN32


/*

$Log: not supported by cvs2svn $
Revision 1.2  2011-02-25 02:30:08  leeten

[02/20/2011]
1. [MOD] Delay the printing of header until LIBCLOCK_PRINT is called.

Revision 1.1  2011/01/10 19:35:45  leeten

[01/09/2010]
1. [1ST] First time checkin.


*/
