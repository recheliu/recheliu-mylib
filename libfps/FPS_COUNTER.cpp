//////////////////////////////////////////////////////////////////////////////////////////
//	FPS_COUNTER.cpp
//	Functions for Frames per second counter class
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//////////////////////////////////////////////////////////////////////////////////////////

#if	0	// DEL-BY-LEETEN 01/23/2011-BEGIN
	// ADD-BY-LEETEN-2009/06/01-BEGIN
	#ifdef WIN32	
	// ADD-BY-LEETEN-2009/06/01-END
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <mmsystem.h>
	// ADD-BY-LEETEN-2009/06/01-BEGIN
	#else
	#include <time.h>
	#endif
	// ADD-BY-LEETEN-2009/06/01-END
#endif	// DEL-BY-LEETEN 01/23/2011-END

#include "FPS_COUNTER.h"

void FPS_COUNTER::Update(void)
{
	//keep track of time passed and frame count
	#if	0	// MOD-BY-LEETEN 01/23/2011-FROM:
		// ADD-BY-LEETEN-2009/06/01-BEGIN
		#ifdef WIN32
		// ADD-BY-LEETEN-2009/06/01-END

		time=timeGetTime()*0.001f;

		// ADD-BY-LEETEN-2009/06/01-BEGIN
		#else
		time=(float)clock() * 0.001;
		#endif
		// ADD-BY-LEETEN-2009/06/01-END
		++frames;

		//If a second has passed
		if(time-lastTime>1.0f)
		{
			fps=frames/(time-lastTime);	//update the number of frames per second
			lastTime=time;				//set time for the start of the next count
			frames=0;					//reset fps for this second
		}
	#else	// MOD-BY-LEETEN 01/23/2011-TO:
	#ifdef WIN32
	time = timeGetTime()/1000.0;
	#else
	struct timespec cTime;
	clock_gettime(CLOCK_REALTIME, &cTime)
	time = (double)cTime.tv_sec + (double)cTime.tv_nsec/1.0e+9;
	#endif
	fps = (float)(1.0 / (time-lastTime));
	lastTime=time;				//set time for the start of the next count
	#endif		// MOD-BY-LEETEN 01/23/2011-END
}
