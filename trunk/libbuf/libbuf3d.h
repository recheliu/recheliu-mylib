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

	TBuffer3D():TBuffer<t>()
	{
		iWidth = 0;
		iHeight = 0;
		iDepth = 0;
	}

	~TBuffer3D()	
	{	
	}

	t* New(int n)
	{
		iWidth = n;
		iHeight = 1;
		iDepth = 1;
		return TBuffer<t>::New(n);
	}

	t* New(int w, int h, int d)
	{
		assert(w > 0 && h > 0 && d > 0);
		iWidth = w;
		iHeight = h;
		iDepth = d;
		return TBuffer<t>::New(iWidth * iHeight * iDepth);
	}

	t* alloc(int n)
	{
		return TBuffer<t>::alloc(n);
	}

	t* alloc(int w, int h, int d)
	{
		assert(w > 0 && h > 0 && d > 0);
		iWidth = w;
		iHeight = h;
		iDepth = d;
		return TBuffer<t>::alloc(iWidth * iHeight * iDepth);
	}

	t& GetElement(int x, int y, int z)
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		assert( 0 <= z && z < iDepth);
		return TBuffer<t>::data[x + y * iWidth + z * iWidth * iHeight];
	}

	const t& GetElement(int x, int y, int z) const
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		assert( 0 <= z && z < iDepth);
		return TBuffer<t>::data[x + y * iWidth + z * iWidth * iHeight];
	}

	void _Save(const char *szFilenamePrefix)
	{
		if( !TBuffer<t>::BIsAllocated() )
		{
			LOG(printf("Non-initialized buffer"));
			return;
		}

		char szFilename[1024];
		sprintf(szFilename, "%s.%s.b3d", szFilenamePrefix, typeid(t).name());

		FILE *fp;
		fp = fopen(szFilename, "wb");
		ASSERT_OR_LOG(fp, perror(szFilename)); 

		fwrite(&iWidth, sizeof(iWidth), 1, fp);
		fwrite(&iHeight, sizeof(iHeight), 1, fp);
		fwrite(&iDepth, sizeof(iDepth), 1, fp);
		fwrite(TBuffer<t>::data, sizeof(t), TBuffer<t>::num, fp);

		fclose(fp);
	}
};

#endif	// __LIB_BUFFER_3D_H__

