#pragma once

#ifndef MAX_NR_OF_TIMERS
	#define MAX_NR_OF_TIMERS	64
#endif

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment (lib, "winmm.lib")      // link with my own library libfps
	
	#define LIBCLOCK_INIT(flag, header)						\
		{															\
			const int _iMaxNrOfTimers = MAX_NR_OF_TIMERS;			\
			DWORD _pdBeginTimers[_iMaxNrOfTimers];					\
			DWORD _pdEndTimers[_iMaxNrOfTimers];					\
			const char* szHeader = header;								\
			size_t _t = 0;											


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

#else	// #ifdef WIN32
	#include <time.h>
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

	#pragma message("Remerber to add the argument '-l rt' to the linker options.")
	#endif	// #if defined(__APPLE__) && defined(__MACH__)		

	#define LIBCLOCK_INIT(flag, header)							\
		{																\
			static struct timespec _pcBeginTimers[MAX_NR_OF_TIMERS];	\
			static struct timespec _pcEndTimers[MAX_NR_OF_TIMERS];		\
			size_t _t = 0;												\
			const char* szHeader = header;									


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

#endif	// #ifdef WIN32


