//////////////////////////////////////////////////////////////////////////////////////////
//	FPS_COUNTER.cpp
//	Functions for Frames per second counter class
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//////////////////////////////////////////////////////////////////////////////////////////

#include "FPS_COUNTER.h"

void FPS_COUNTER::Update(void)
{
	//keep track of time passed and frame count
	#ifdef WIN32
	time = timeGetTime()/1000.0;
	#else
	struct timespec cTime;
		clock_gettime(CLOCK_REALTIME, &cTime);
	time = (double)cTime.tv_sec + (double)cTime.tv_nsec/1.0e+9;
	#endif
	//If a second has passed
	if(time-lastTime>1.0)
	{
		fps=frames/(time-lastTime);	//update the number of frames per second
		lastTime=time;				//set time for the start of the next count
		frames=0;					//reset fps for this second
	}
	frames++;
}
