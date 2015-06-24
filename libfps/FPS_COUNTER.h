//////////////////////////////////////////////////////////////////////////////////////////
//	FPS_COUNTER.h
//	Frames per sceond counter class
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#ifdef WIN32
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment (lib, "winmm.lib")      // link with my own library libfps
#else
	#include <time.h>
	#pragma message("Remerber to add the argument '-l rt' to the linker options.")
#endif

class FPS_COUNTER
{
public:
	FPS_COUNTER()	:	fps(0.0f), lastTime(0.0f), frames(0), time(0.0f)
	{}
	~FPS_COUNTER()	{}

	void Update(void);
	float GetFps(void)	{	return fps;	}

protected:
	float fps;

	int frames;
	double lastTime;
	double time;
};

#endif	//FPS_COUNTER_H
