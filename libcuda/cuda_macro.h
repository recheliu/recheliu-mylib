/*

This file defines several important macros for CUDA.

*/

#ifndef __CUDA__MACRO__H__
#define __CUDA__MACRO__H__

#include <vector>
#include <string>
#include <sstream>
using std::ios;
using std::pair;
using std::string;

#include "logvar.h"

#include <stdlib.h> 
	#include <cuda_runtime_api.h>

	#  define CUDA_SAFE_CALL_NO_SYNC( call) do {                                 \
	    cudaError err = call;                                                    \
	    if( cudaSuccess != err) {                                                \
		fprintf(stderr, "Cuda error in file '%s' in line %i : %s.\n",        \
			__FILE__, __LINE__, cudaGetErrorString( err) );              \
		exit(EXIT_FAILURE);                                                  \
	    } } while (0)

	#  define CUDA_SAFE_CALL( call) do {                                         \
	    CUDA_SAFE_CALL_NO_SYNC(call);                                            \
	    cudaError err = cudaThreadSynchronize();                                 \
	    if( cudaSuccess != err) {                                                \
		fprintf(stderr, "Cuda error in file '%s' in line %i : %s.\n",        \
			__FILE__, __LINE__, cudaGetErrorString( err) );              \
		exit(EXIT_FAILURE);                                                  \
	    } } while (0)

	//! Check for CUDA error
	#ifdef _DEBUG
	#  define CUT_CHECK_ERROR(errorMessage) {                                    \
		cudaError_t err = cudaGetLastError();                                    \
		if( cudaSuccess != err) {                                                \
			fprintf(stderr, "Cuda error: %s in file '%s' in line %i : %s.\n",    \
				errorMessage, __FILE__, __LINE__, cudaGetErrorString( err) );\
			exit(EXIT_FAILURE);                                                  \
		}                                                                        \
		err = cudaThreadSynchronize();                                           \
		if( cudaSuccess != err) {                                                \
			fprintf(stderr, "Cuda error: %s in file '%s' in line %i : %s.\n",    \
				errorMessage, __FILE__, __LINE__, cudaGetErrorString( err) );\
			exit(EXIT_FAILURE);                                                  \
		}                                                                        \
	}

	#else	//	#ifdef _DEBUG
	#  define CUT_CHECK_ERROR(errorMessage) {                                    \
		cudaError_t err = cudaGetLastError();                                    \
		if( cudaSuccess != err) {                                                \
			fprintf(stderr, "Cuda error: %s in file '%s' in line %i : %s.\n",    \
			errorMessage, __FILE__, __LINE__, cudaGetErrorString( err) );\
			exit(EXIT_FAILURE);                                                  \
		}                                                                        \
	}

	#endif	//	#ifdef _DEBUG


#include <math.h>

#ifndef __DEVICE_EMULATION__

	#undef _HUGE
	#define _HUGE	1.0e+10

	#undef HUGE_VAL
	#define HUGE_VAL _HUGE

#endif	// __DEVICE_EMULATION__

#if CUDART_VERSION <= 1010

	#define CUDA_INIT(argc, argv)	CUT_DEVICE_INIT()

#else
	#define CUDA_INIT(argc, argv)	CUT_DEVICE_INIT(argc, argv)

#endif

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

#define GPUCLOCK_INIT(flag, header)									\
	{														\
		const char* szHeader = header;								\
		vector < pair<const char*, float> > timers;							\
		cudaEvent_t _start, _stop;							\
		cudaEventCreate(&_start);							\
		cudaEventCreate(&_stop); 


#define GPUCLOCK_BEGIN(flag, name)								\
	if( (flag) )										\
	{													\
		timers.push_back(make_pair(name, 0.0f));	\
		cudaEventRecord(_start, 0);						\
	}

#define GPUCLOCK_END(flag)							\
	if( (flag) )										\
	{													\
		cudaEventRecord(_stop, 0);						\
		cudaEventSynchronize(_stop);						\
		cudaEventElapsedTime(&timers.rbegin()->second, _start, _stop);	\
	}

inline std::string STRSummarizeTimers(
	const char* szHeader,
	const std::vector< pair<const char*, float> >& vszTimers)
{
	std::ostringstream osBuffer;
	osBuffer
		<< szHeader
		<< "("
		;
	for (auto itimer = vszTimers.begin(); itimer != vszTimers.end(); itimer++)
	{
		osBuffer
			<< itimer->first
			<< ","
			;
	}
	osBuffer << "TOTAL):  ";
	osBuffer.setf(ios::fixed);
	osBuffer.precision(2);
	float fTotalTime = 0.0f;
	for (auto itimer = vszTimers.begin(); itimer != vszTimers.end(); itimer++)
	{
		fTotalTime += itimer->second;
		osBuffer
			<< itimer->second
			<< ","
			;
	}
	osBuffer << fTotalTime;
	return osBuffer.str();
}

#define GPUCLOCK_PRINT(flag)									\
		if( (flag) )										\
		{													\
			LOG_VAR(STRSummarizeTimers(szHeader, timers));	\
		}	\
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

template<typename T>
struct CBufferPitch: public cudaPitchedPtr
{
	size_t uWidthInBytes;
	size_t uPitchInWords;
	size_t uSize;

	void* PAlloc(size_t w, size_t h)	
	{ 
		xsize = w;
		ysize = h;
		uWidthInBytes = w * sizeof(T);
		CUDA_SAFE_CALL_NO_SYNC(
			cudaMallocPitch(
				(void**)&ptr, &pitch,
				uWidthInBytes, ysize)	);
		uPitchInWords = pitch / sizeof(T);
		uSize = pitch * ysize;

		return ptr;
	}
};

#endif	// __CUDA__MACRO__H__

