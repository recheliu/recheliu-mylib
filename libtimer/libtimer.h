#pragma once

#include <sstream>
#include <vector>
using namespace std;

#if defined(WIN32)
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment (lib, "winmm.lib")      
#else	// #if defined(WIN32)
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
#endif	// #if defined(WIN32)

// TEST-ADD-BEGIN
#if !defined(GOOGLE_LOG_INFO)
// TEST-ADD-END
#include "liblog.h"
// TEST-ADD-BEGIN
#endif	// #if !defined(GOOGLE_LOG_INFO)
// TEST-ADD-END

class CTimer
{
public:
#if defined(WIN32)
	typedef DWORD CLOCK;

	static
	void _GetTime(CLOCK *pClock)
	{
		*pClock = timeGetTime();
	}

	static
	double
	DGetMsec
	(
		const CLOCK& time
	)
	{
		return (double)time;
	}
#else	// #if	defined(WIN32)
	typedef timespec CLOCK;
	static
	void _GetTime(CLOCK *pClock)
	{
		clock_gettime(CLOCK_REALTIME, pClock);
	}

	static
	double
	DGetMsec
	(
		const CLOCK& clock
	)
	{
		return ((double)clock.tv_sec * 1.0e+3 + (double)clock.tv_nsec/1.0e+6);
	}
#endif	// #if	defined(WIN32)

protected:
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
		// TEST-MOD:	vpairTimes.push_back(make_pair<string, CLOCK>(os.str(), c));
		vpairTimes.push_back(make_pair(os.str(), c));
		// TEST-MOD-END
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
		double dSum = 0.0;
		for(vector<pair<string, CLOCK>>::iterator 
				ivpairTime = vpairTimes.begin();
			ivpairTime != vpairTimes.end();
			ivpairTime ++ )
		{
			osLabels<< ivpairTime->first<<",";
			double dTime = DGetMsec(ivpairTime->second);
			osTimes<< dTime<< ",";
			dSum += dTime;
		}
		osLabels<<"TOTAL";
		osTimes<<dSum;
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
