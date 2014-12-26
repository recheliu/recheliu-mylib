#ifndef __LIB_BUFFER_2D_H__
#define __LIB_BUFFER_2D_H__

#include <memory.h>
#include <assert.h>

#include "libbuf.h"

// a template for 2D buffer
template <typename t> 
struct TBuffer2D: public TBuffer<t>
{
	int iWidth;
	int iHeight;

	TBuffer2D():TBuffer<t>()
	{
		iWidth = 0;
		iHeight = 0;
	}

	~TBuffer2D()	
	{	
	}

	t * New(int n)
	{
		assert(n > 0);
		iWidth = n;
		iHeight = 1;
		return TBuffer<t>::New(iWidth * iHeight);
	}

	t * New(int w, int h)
	{
		assert(w > 0 && h > 0);
		iWidth = w;
		iHeight = h;
		return TBuffer<t>::New(iWidth * iHeight);
	}

	t* alloc(int n)
	{
		TBuffer<t>::alloc(n);
		return TBuffer<t>::data;
	}

	t * alloc(int w, int h)
	{
		assert(w > 0 && h > 0);
		iWidth = w;
		iHeight = h;
		return TBuffer<t>::alloc(iWidth * iHeight);
	}

	t& GetElement(int x, int y)
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		return TBuffer<t>::data[x + y * iWidth];
	}

	const t& GetElement(int x, int y) const
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		return TBuffer<t>::data[x + y * iWidth];
	}

	void _Save(const char *szFilenamePrefix)
	{
		if( !TBuffer<t>::BIsAllocated() )
		{
			LOG(printf("Non-initialized buffer"));
			return;
		}

		char szFilename[1024];
		sprintf(szFilename, "%s.%s.b2d", szFilenamePrefix, typeid(t).name());

		FILE *fp;
		fp = fopen(szFilename, "wb");
		ASSERT_OR_LOG(fp, perror(szFilename)); 

		fwrite(&iWidth, sizeof(iWidth), 1, fp);
		fwrite(&iHeight, sizeof(iHeight), 1, fp);
		fwrite(TBuffer<t>::data, sizeof(t), TBuffer<t>::num, fp);

		fclose(fp);
	}
};

#endif	// __LIB_BUFFER_2D_H__

