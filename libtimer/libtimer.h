#pragma once

#include <sstream>
#include <vector>
using namespace std;

#if defined(WIN32)
	// DEL-BY-LEETEN 2014/12/25:	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment (lib, "winmm.lib")      
#else	// #if defined(WIN32)
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
	#pragma message("Remerber to add the argument '-l rt' to the linker options.")
	#endif	// #if defined(__APPLE__) && defined(__MACH__)		// ADD-BY-LEETEN 09/13/2011
#endif	// #if defined(WIN32)

#include "liblog.h"

class CTimer
{
// ADD-BY-LEETEN 2014/12/25-BEGIN
public:
// ADD-BY-LEETEN 2014/12/25-END
#if defined(WIN32)
	typedef DWORD CLOCK;

	// ADD-BY-LEETEN 2014/12/25-BEGIN
	static
	// ADD-BY-LEETEN 2014/12/25-END
	void _GetTime(CLOCK *pClock)
	{
		*pClock = timeGetTime();
	}
	#if	0		// MOD-BY-LEETEN 2014-12/20-FROM:
	size_t
	UGetMsec
	(
		const CLOCK& time
	)
	{
		return (size_t)time;
	}
	#else	// MOD-BY-LEETEN 2014-12/20-TO:
	// ADD-BY-LEETEN 2014/12/25-BEGIN
	static
	// ADD-BY-LEETEN 2014/12/25-END
	double
	DGetMsec
	(
		const CLOCK& time
	)
	{
		return (double)time;
	}
	#endif	// MOD-BY-LEETEN 2014-12/20-END
#else	// #if	defined(WIN32)
	typedef timespec CLOCK;
	// ADD-BY-LEETEN 2014/12/25-BEGIN
	static
	// ADD-BY-LEETEN 2014/12/25-END
	void _GetTime(CLOCK *pClock)
	{
		clock_gettime(CLOCK_REALTIME, pClock);
	}

	#if	0	// MOD-BY-LEETEN 2014-12/20-FROM:
	size_t
	UGetMsec
	(
		const CLOCK& clock
	)
	{
		return (size_t)((double)clock.tv_sec * 1.0e+3 + (double)clock.tv_nsec/1.0e+6);
	}
	#else	// MOD-BY-LEETEN 2014-12/20-TO:
	// ADD-BY-LEETEN 2014/12/25-BEGIN
	static
	// ADD-BY-LEETEN 2014/12/25-END
	double
	DGetMsec
	(
		const CLOCK& clock
	)
	{
		return ((double)clock.tv_sec * 1.0e+3 + (double)clock.tv_nsec/1.0e+6);
	}
	#endif	// MOD-BY-LEETEN 2014-12/20-END
#endif	// #if	defined(WIN32)

// ADD-BY-LEETEN 2014/12/25-BEGIN
protected:
// ADD-BY-LEETEN 2014/12/25-END
    vector<pair<string, CLOCK>> vpairTimes;
	string strName;

public:
	CTimer(const string& strName=""):
		strName(strName)
	{
	}
	
	void 
	_Tic(
		const string& strLabel=""
	)
	{
		CLOCK c;
		_GetTime(&c);

		ostringstream os;
		if( !strLabel.empty() ) 
		{
			os<<strLabel;
		}
		else
		{
			os<<"L"<<vpairTimes.size();
		}
		vpairTimes.push_back(make_pair<string, CLOCK>(os.str(), c));
	}
	
	void 
	_Toc
	(
	)
	{
		CLOCK c;
		_GetTime(&c);
		vpairTimes.rbegin()->second = c - vpairTimes.rbegin()->second;
	}

	//! Combine the current value to a string.
	string
	STRToString
	(
	)
	{
		ostringstream osLabels;
		ostringstream osTimes;
		// ADD-BY-LEETEN 2014-12/20-BEGIN
		double dSum = 0.0;
		// ADD-BY-LEETEN 2014-12/20-END
		for(vector<pair<string, CLOCK>>::iterator 
				ivpairTime = vpairTimes.begin();
			ivpairTime != vpairTimes.end();
			ivpairTime ++ )
		{
			osLabels<< ivpairTime->first<<",";
			// MOD-BY-LEETEN 2014-12/20:	osTimes<< UGetMsec(ivpairTime->second)<<",";
			double dTime = DGetMsec(ivpairTime->second);
			osTimes<< dTime<< ",";
			dSum += dTime;
			// MOD-BY-LEETEN 2014-12/20-END
		}
		// ADD-BY-LEETEN 2014-12/20-BEGIN
		osLabels<<"TOTAL";
		osTimes<<dSum;
		// ADD-BY-LEETEN 2014-12/20-END
		ostringstream osList;
		osList<< strName<< "("<< osLabels.str()<< "): "<< osTimes.str();
		return osList.str();
	}
};

// Macros that can mimic libclock so there is no need to declare the timers.

#define TIMER_BEGIN(NAME)	\
	{	\
		CTimer __cTimer__(NAME);	\

#define TIMER_TIC(LABEL)	\
		__cTimer__._Tic(LABEL);

#define TIMER_TOC()			\
		__cTimer__._Toc();

#define TIMER_PRINT()		\
		LOG(cout<<__cTimer__.STRToString());

#define TIMER_END()			\
	}	
