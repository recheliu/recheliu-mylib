/*

This file defines several important macros for CUDA.

*/

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

	// MOD-BY-LEETEN 01/25/2011-FROM:
		// TBuffer2D():TBuffer()
	// TO:
	TBuffer2D():TBuffer<t>()
	// MOD-BY-LEETEN 01/25/2011-END
	{
		iWidth = 0;
		iHeight = 0;
	}

	~TBuffer2D()	
	{	
	}

	// ADD-BY-LEETEN 01/26/2010-BEGIN
	t* alloc(int n)
	{
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// TBuffer::alloc(n);
		// TO:
		TBuffer<t>::alloc(n);
		// MOD-BY-LEETEN 01/25/2011-END
		// MOD-BY-LEETEN 01/30/2011-FROM:
			// return data;
		// TO:
		return TBuffer<t>::data;
		// MOD-BY-LEETEN 01/30/2011-END
	}
	// ADD-BY-LEETEN 01/26/2010-END

	t * alloc(int w, int h)
	{
		assert(w > 0 && h > 0);
		iWidth = w;
		iHeight = h;
		#if	0	// MOD-BY-LEETEN 01/25/2011-FROM:
			TBuffer::alloc(iWidth * iHeight);
			return data;
		#else	// MOD-BY-LEETEN 01/25/2011-TO:
		TBuffer<t>::alloc(iWidth * iHeight);
		return TBuffer<t>::data;
		#endif	// MOD-BY-LEETEN 01/25/2011-END
	}

	t& GetElement(int x, int y)
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// return data[x + y * iWidth];
		// TO:
		return TBuffer<t>::data[x + y * iWidth];
		// MOD-BY-LEETEN 01/25/2011-END
	}

	const t& GetElement(int x, int y) const
	{
		assert( 0 <= x && x < iWidth);
		assert( 0 <= y && y < iHeight);
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// return data[x + y * iWidth];
		// TO:
		return TBuffer<t>::data[x + y * iWidth];
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
			// MOD-BY-LEETEN 02/24/2011-FROM:
				// LOG("Non-initialized buffer");
			// TO:
			LOG(printf("Non-initialized buffer"));
			// MOD-BY-LEETEN 02/24/2011-END
			return;
		}

		char szFilename[1024];
		sprintf(szFilename, "%s.%s.b2d", szFilenamePrefix, typeid(t).name());

		FILE *fp;
		fp = fopen(szFilename, "wb");
		if( !fp )
		{
			perror(szFilename);
			return;
		}

		fwrite(&iWidth, sizeof(iWidth), 1, fp);
		fwrite(&iHeight, sizeof(iHeight), 1, fp);
		// MOD-BY-LEETEN 01/25/2011-FROM:
			// fwrite(data, sizeof(t), num, fp);
		// TO:
		fwrite(TBuffer<t>::data, sizeof(t), TBuffer<t>::num, fp);
		// MOD-BY-LEETEN 01/25/2011-END

		fclose(fp);
	}
	// ADD-BY-LEETEN 09/19/2010-END
};

#endif	// __LIB_BUFFER_2D_H__

/*

$Log: not supported by cvs2svn $
Revision 1.5  2011/01/31 03:18:08  leeten

[01/30/2011]
1. [MOD] Add the prefix TBuffer<t>:: when return the data pointer.

Revision 1.4  2011/01/25 06:19:17  leeten

[01/25/2011]
1. [MOD] Add the type declarere <t> when call the class TBuffer or access its member,

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



