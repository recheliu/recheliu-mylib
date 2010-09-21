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

	// ADD-BY-LEETEN 01/26/2010-BEGIN
	t* alloc(int n)
	{
		TBuffer::alloc(n);
		return data;
	}
	// ADD-BY-LEETEN 01/26/2010-END

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

	// ADD-BY-LEETEN 09/19/2010-BEGIN
	void _Save(char *szFilenamePrefix)
	{
		if( !BIsAllocated() )
		{
			LOG("Non-initialized buffer");
			return;
		}

		char szFilename[1024];
		sprintf(szFilename, "%s.%s.b3d", szFilenamePrefix, typeid(t).name());

		FILE *fp;
		fp = fopen(szFilename, "wb");
		if( !fp )
		{
			perror(szFilename);
			return;
		}

		fwrite(&iWidth, sizeof(iWidth), 1, fp);
		fwrite(&iHeight, sizeof(iHeight), 1, fp);
		fwrite(&iDepth, sizeof(iDepth), 1, fp);
		fwrite(data, sizeof(t), num, fp);

		fclose(fp);
	}
	// ADD-BY-LEETEN 09/19/2010-END
};

#endif	// __LIB_BUFFER_3D_H__

/*

$Log: not supported by cvs2svn $
Revision 1.2  2010/01/29 19:57:00  leeten

[01/29/2010]
1. [ADD] Overload the method alloc(int num).

Revision 1.1  2009/11/04 20:42:13  leeten

[2009/11/04]
1. [1ST] First time checin. These header files define the template class for 2D/3D memory buffer.


*/



