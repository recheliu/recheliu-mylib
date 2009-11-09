#pragma once

	#include "FboWin.h"
	#include "libclip/ClipPlanes.h"

struct CClipVolume
{
	int iNrOfClipPlanes;
	double ppdClipPlanes[CClipPlanes::NR_OF_CLIP_PLANES][4];

	GLuint pidClipVolume;	// ID of the shader program to construct the clipping volume

	CFrameBufferTexture<
		GL_RGBA32F_ARB,
		GL_RGBA> cTexture;
	GLuint fid;

	void _SetClipPlanes(int iNrOfClipPlanes, double* pdClipPlanes);
	void _InitFunc();
	void _ReshapeFunc(int w, int h);
	void _Create();

	CClipVolume(void);
	virtual ~CClipVolume(void);
};

/*

$Log: not supported by cvs2svn $

*/
