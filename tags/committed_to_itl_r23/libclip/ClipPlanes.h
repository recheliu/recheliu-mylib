#pragma once

	#include "GlutWin.h"

class CClipPlanes
{
public:
	enum {
		NR_OF_CLIP_PLANES = 6,
	};

	double ppdPlanes[NR_OF_CLIP_PLANES][4];
	int piFlags[NR_OF_CLIP_PLANES];

	CClipPlanes(void);
public:
	~CClipPlanes(void);
};

/*

$Log: not supported by cvs2svn $

*/
