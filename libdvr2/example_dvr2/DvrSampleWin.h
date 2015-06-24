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

