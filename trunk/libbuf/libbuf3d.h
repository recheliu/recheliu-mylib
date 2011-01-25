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

	// MOD-BY-LEETEN 01/25/2011-FROM:
		// TBuffer3D():TBuffer()
	// TO:
	TBuffer3D():TBuffer<t>()
	// MOD-BY-LEETEN 01/25/2011-END
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
		#if	0	// MOD-BY-LEETEN 01/25/2011-FROM:
			TBuffer::alloc(n);
			return data;
		#else	// MOD-BY-LEETEN 01/25/2011-TO:
		TBuffer<t>::alloc(n);
		return TBuffer<t>::data;
		#endif	// MOD-BY-LEETEN 01/25/2011-END
	}
	// ADD-BY-LEETEN 01/26/2010-END

	t* alloc(int w, int h, int d)
	{
		assert(w > 0 && h > 0 && d > 0);
		iWidth = w;
		iHeight = h;
		iDepth = d;
		#if	0	// MOD-BY-LEETEN 01/25/2011-FROM:
			TBuffer::alloc(iWidth * iHeight * iDepth);
			return data;
		#else	// MOD-BY-LEETEN 01/25/2011-TO:
		TBuffer<t>::alloc(iWidth * iHeight * iDepth);
		return TBuffer<t>::data;
		#endif	// MOD-BY-LEETEN 01/25/2011-END
	}

	t& GetElement(int x, int y, int z)
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		assert( 0 <= z && z < iDepth);
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// return data[x + y * iWidth + z * iWidth * iHeight];
		// TO:
		return TBuffer<t>::data[x + y * iWidth + z * iWidth * iHeight];
		// MOD-BY-LEETEN 01/25/2011-END
	}

	const t& GetElement(int x, int y, int z) const
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		assert( 0 <= z && z < iDepth);
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// return data[x + y * iWidth + z * iWidth * iHeight];
		// TO:
		return TBuffer<t>::data[x + y * iWidth + z * iWidth * iHeight];
		// MOD-BY-LEETEN 01/25/2011-END
	}

	// ADD-BY-LEETEN 09/19/2010-BEGIN
	void _Save(char *szFilenamePrefix)
	{
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// if( !BIsAllocated() )
		// TO:
		if( !TBuffer<t>::BIsAllocated() )
		// MOD-BY-LEETEN 01/25/2011-END
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
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// fwrite(data, sizeof(t), num, fp);
		// TO:
		fwrite(TBuffer<t>::data, sizeof(t), TBuffer<t>::num, fp);
		// MOD-BY-LEETEN 01/25/2011-END

		fclose(fp);
	}
	// ADD-BY-LEETEN 09/19/2010-END
};

#endif	// __LIB_BUFFER_3D_H__

/*

$Log: not supported by cvs2svn $
Revision 1.3  2010/09/21 13:29:15  leeten

[09/21/2010]
1. [ADD] Define a new method _Save() to dump the data in the buffer.
2. [ADD] Define a new macro SAVE_BUF to dump the data into a file which is automatically named according to the variable's name and scope.

Revision 1.2  2010/01/29 19:57:00  leeten

[01/29/2010]
1. [ADD] Overload the method alloc(int num).

Revision 1.1  2009/11/04 20:42:13  leeten

[2009/11/04]
1. [1ST] First time checin. These header files define the template class for 2D/3D memory buffer.


*/



