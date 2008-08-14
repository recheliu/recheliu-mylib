/*

This file defines several important macros for CUDA.

*/

#ifndef __LIB_BUFFER_H__
#define __LIB_BUFFER_H__

#include <memory.h>
#include <assert.h>

// a template for buffer
template <typename t> struct TBuffer 
{
	int num;
	t* data;

	TBuffer()	
	{	
		num = 0;
		data = NULL;	
	}

	// ADD-BY-TLEE 2008/08/14-BEGIN
	void free()
	{
		if( data ) 
			delete [] data;
		data = NULL;
	}
	// ADD-BY-TLEE 2008/08/14-END

	~TBuffer()	
	{	
		#if	0	// MOD-BY-TLEE 2008/08/14-FROM:
			if( data ) 
				// free(data);	
				delete [] data;
			data = NULL;
		#else	// MOD-BY-TLEE 2008/08/14-TO:

		free();

		#endif	// MOD-BY-TLEE 2008/08/14-END
	}

	t* alloc(int n)
	{
		assert(n > 0);
		num = n;

		// ADD-BY-TLEE 2008/08/14-BEGIN
		free();
		// ADD-BY-TLEE 2008/08/14-END

		// data = (t*)calloc(num, sizeof(t));
		data = new t[num];
		assert(data);
		memset(data, 0, sizeof(t) * num);
		return data;
	}

	t& operator[](int index)
	{
		assert( index < num );
		return data[index];
	}

	const t& operator[](int index) const
	{
		assert( index < num );
		return data[index];
	}
};

#endif	// __LIB_BUFFER_H__

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/10 18:54:05  leeten

[2008/08/10]
1. First time checkin. This lib. is used to wrap a buffer in order to avoid the routine to check and free memory.


*/



