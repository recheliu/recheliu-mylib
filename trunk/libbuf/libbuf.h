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

	~TBuffer()	
	{	
		if( data ) 
			// free(data);	
			delete [] data;
		data = NULL;
	}

	t* alloc(int n)
	{
		assert(n > 0);
		num = n;

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

*/



