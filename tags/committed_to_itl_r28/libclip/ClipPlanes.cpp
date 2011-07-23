#include "ClipPlanes.h"

CClipPlanes::CClipPlanes(void)
{
	memset(ppdPlanes, 0, sizeof(ppdPlanes));
	memset(piFlags, 0, sizeof(piFlags));
}

CClipPlanes::~CClipPlanes(void)
{
}

/*

$Log: not supported by cvs2svn $

*/
