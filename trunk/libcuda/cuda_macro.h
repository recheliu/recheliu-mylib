/*

This file defines several important macros for CUDA.

*/

#ifndef __CUDA__MACRO__H__
#define __CUDA__MACRO__H__

#include <cutil.h>

// ADD-BY-TLEE 07/30/2008-BEGIN
#if CUDART_VERSION <= 1010

	#define CUDA_INIT(argc, argv)	CUT_DEVICE_INIT()

#else
	#define CUDA_INIT(argc, argv)	CUT_DEVICE_INIT(argc, argv)

#endif
// ADD-BY-TLEE 07/30/2008-END

#define ADDRESS_2D(type, base, eb, wb, w, h)	\
	((type*)((unsigned char*)(base) + (wb) * (h) + (w) * (eb)))

#define SETUP_ARRAY(a, w, h, cr, cg, cb, ca, f)	\
	{	\
		cudaChannelFormatDesc cDesc;	\
		cDesc = cudaCreateChannelDesc((cr), (cg), (cb), (ca), (f));	\
	    CUDA_SAFE_CALL( cudaMallocArray(&(a), &cDesc, (w), (h))	);	\
	}

#define BIND_ARRAY_AS_TEXTURE(t, a, a0, a1, f, n)	\
	{	\
		(t).addressMode[0] = a0;	\
		(t).addressMode[1] = a1;	\
		(t).filterMode	 = f;		\
		(t).normalized	 = n;					\
	    CUDA_SAFE_CALL( cudaBindTextureToArray( (t), (a))	);	\
	}

#define BIND_TEXTURE(t, m, m_size, cr, cg, cb, ca, f)	\
	{	\
		size_t uOffset;	\
		cudaChannelFormatDesc cDesc;	\
		cDesc = cudaCreateChannelDesc((cr), (cg), (cb), (ca), (f));	\
		(t).addressMode[0] = cudaAddressModeClamp;	\
		(t).addressMode[1] = cudaAddressModeClamp;	\
		(t).filterMode	 = cudaFilterModePoint;		\
		(t).normalized	 = false;					\
		CUDA_SAFE_CALL_NO_SYNC( cudaBindTexture(&uOffset, (t), (m), cDesc, (m_size))	);	\
	}

#define SETUP_ARRAY_AND_TEXTURE(w, h, cr, cg, cb, ca, f, a, t)	\
	{	\
		SETUP_ARRAY((a), (w), (h), (cr), (cg), (cb), (ca), (f));	\
		BIND_ARRAY_AS_TEXTURE((t), (a), cudaAddressModeClamp, cudaAddressModeClamp, cudaFilterModePoint, false);	\
	}
/*
	{	\
		cudaChannelFormatDesc cDesc;	\
		cDesc = cudaCreateChannelDesc((cr), (cg), (cb), (ca), (f));	\
	    CUDA_SAFE_CALL( cudaMallocArray(&(a), &cDesc, (w), (h))	);	\
		(t).addressMode[0] = cudaAddressModeClamp;	\
		(t).addressMode[1] = cudaAddressModeClamp;	\
		(t).filterMode	 = cudaFilterModePoint;		\
		(t).normalized	 = false;					\
	    CUDA_SAFE_CALL( cudaBindTextureToArray( (t), (a))	);	\
	}
*/

#define FREE_KERNEL_MEMORY(p, f)	\
	{	\
		if(	(p) ) \
		{	\
			f((p));	\
			(p) = NULL;	\
		}	\
	}

#define FREE_ARRAY(p)	FREE_KERNEL_MEMORY(p, cudaFreeArray)
#define FREE_MEMORY(p)	FREE_KERNEL_MEMORY(p, cudaFree)
#define FREE_MEMORY_ON_HOST(p)	FREE_KERNEL_MEMORY(p, cudaFreeHost)

#define CLOCK_INIT(flag, header)									\
	{														\
		static unsigned int _uTimer = 0;					\
		static float _pfTimers[64];							\
		size_t _t = 0;										\
		if( (flag)	)										\
		{													\
			if( !_uTimer )									\
				CUT_SAFE_CALL( cutCreateTimer(&_uTimer) );	\
			printf("%s", header);							\
		}


#define CLOCK_BEGIN(flag)								\
	if( (flag)	)										\
	{													\
		CUT_SAFE_CALL( cutStartTimer(_uTimer) );		\
		CUT_SAFE_CALL( cutResetTimer(_uTimer) );		\
	}

#define CLOCK_END(flag, syn)							\
	if( (flag)	)										\
	{													\
		if( syn )										\
			CUDA_SAFE_CALL(cudaThreadSynchronize() );	\
		CUT_SAFE_CALL( cutStopTimer(_uTimer));			\
		_pfTimers[_t++] = cutGetTimerValue(_uTimer);	\
	}

#define CLOCK_PRINT(flag)									\
		if( (flag)	)										\
		{													\
			float fTotalTime = 0;							\
			for(size_t i=0; i<_t; i++)						\
			{												\
				fTotalTime += _pfTimers[i];					\
				printf("%.2f,", _pfTimers[i]);				\
			}												\
			printf("%.2f\n", fTotalTime);					\
		}													\
	}

template<typename T>
struct CBuffer2D 
{
	size_t uWidth;
	size_t uHeight;

	size_t uWidthInBytes;
	size_t uPitch;
	size_t uPitchInWords;

	size_t uSize;

	T *pDataOnDevice;

	CBuffer2D()	
	{ 
		uWidth = uHeight = uWidthInBytes = uPitch = uPitchInWords = 0;
		pDataOnDevice = NULL;
	}

	void* PAlloc(size_t w, size_t h)	
	{ 
		uWidth = w;
		uHeight = h;
		uWidthInBytes = uWidth * sizeof(*pDataOnDevice);
		CUDA_SAFE_CALL_NO_SYNC(
			cudaMallocPitch(
				(void**)&pDataOnDevice, &uPitch,
				uWidthInBytes, uHeight)	);
		uPitchInWords = uPitch / sizeof(*pDataOnDevice);
		uSize = uPitch * uHeight;

		return pDataOnDevice;
	}

	/*
	~CBuffer2D()	
	{ 
		FREE_MEMORY(pDataOnDevice);
	}
	*/
};

#endif	// __CUDA__MACRO__H__

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/06/12 22:37:55  leeten

[06/12/2008]
1. Frist time checkin.

Revision 1.1.1.1  2008/05/02 19:05:10  leeten

[05/02/2008]
1. First time checkin.

Revision 1.1.1.1  2007/09/07 14:47:47  leeten

[09/07/2007]
1. Frist time checkin.

Revision 1.2  2007/08/23 22:43:02  leeten

[08/23/2007]
1. Add call to make sure CBuffer2D's member function PAlloc successfully allocate the memory buffer.
2. Add new member uSize to CBuffer2D to record the total size of allocated buffer.

Revision 1.1.1.1  2007/07/24 13:41:08  leeten

[07/24/2007]
1. First time checkin.

Revision 1.2  2007/07/13 15:15:52  leeten

[07/13/2007]
1. Let macros CLOCK_INIT and CLOCK_PRINT form a new scope.
2. Remove the header parameter in macro CLOCK_INIT.

Revision 1.1  2007/07/11 16:20:30  leeten

[07/11/2007]
1. First time checkin.


*/



