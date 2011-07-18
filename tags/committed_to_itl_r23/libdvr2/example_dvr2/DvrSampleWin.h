#pragma once

	#include "libclip/ClipVolume.h"
	#include "libdvr2/DvrWin2.h"

class CDvrSampleWin :
	public CDvrWin2, public CClipVolume
{
public:
	GLuint pidRayIntegral;

	void _BeginDisplay();
	void _EndDisplay();
	void _RenderSlab(
		int iSlab, int iNrOfSlabs,
		double pdModelviewMatrix[], double pdProjectionMatrix[], int piViewport[],
		double dMinX, double dMaxX, 
		double dMinY, double dMaxY, 
		double dMinZ, double dMaxZ);

	void _InitFunc();
	void _ReshapeFunc(int w, int h);

	CDvrSampleWin(void);
	~CDvrSampleWin(void);
};

/*

$Log: not supported by cvs2svn $
Revision 1.1  2010/04/06 20:01:15  leeten

[04/06/2010]
1. [1ST] First time checkin.


*/
