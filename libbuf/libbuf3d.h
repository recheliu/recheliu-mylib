/*

This file defines several important macros for CUDA.

*/

#ifndef __LIB_BUFFER_3D_H__
#define __LIB_BUFFER_3D_H__

#include <memory.h>
#include <assert.h>

#include "libbuf.h"

// a template for 2D buffer
template <typename t> 
struct TBuffer3D: public TBuffer<t>
{
	int iWidth;
	int iHeight;
	int iDepth;

	TBuffer3D():TBuffer()
	{
		iWidth = 0;
		iHeight = 0;
		iDepth = 0;
	}

	~TBuffer3D()	
	{	
	}

	t* alloc(int w, int h, int d)
	{
		assert(w > 0 && h > 0 && d > 0);
		iWidth = w;
		iHeight = h;
		iDepth = d;
		TBuffer::alloc(iWidth * iHeight * iDepth);
		return data;
	}

	t& GetElement(int x, int y, int z)
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		assert( 0 <= z && z < iDepth);
		return data[x + y * iWidth + z * iWidth * iHeight];
	}

	const t& GetElement(int x, int y, int z) const
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		assert( 0 <= z && z < iDepth);
		return data[x + y * iWidth + z * iWidth * iHeight];
	}
};

#endif	// __LIB_BUFFER_3D_H__

/*

$Log: not supported by cvs2svn $

*/



