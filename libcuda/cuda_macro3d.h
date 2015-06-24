#pragma once

//////////////////////////////////////////////////////////////
template<typename T>
void
_Malloc3D(cudaExtent cExtent3D, cudaPitchedPtr *pPtr)
{
	cudaExtent cExtent = cExtent3D;
	cExtent.width *= sizeof(T);
	CUDA_SAFE_CALL_NO_SYNC( 
		cudaMalloc3D(pPtr, cExtent) );
}

//////////////////////////////////////////////////////////////
template<typename T>
void
	_Malloc3DArray(cudaExtent cExtent3D, cudaArray **ppArray, bool bIsPrecisionHalf = false)
{
	cudaChannelFormatDesc cChannelDesc;
	if( bIsPrecisionHalf )
		switch( sizeof(T) / sizeof(float) )
		{
		case 1:	cChannelDesc = cudaCreateChannelDescHalf();		break;
		case 2:	cChannelDesc = cudaCreateChannelDescHalf2();	break;
		case 4:	cChannelDesc = cudaCreateChannelDescHalf4();	break;
		}
	else
		cChannelDesc = cudaCreateChannelDesc<T>();

	CUDA_SAFE_CALL_NO_SYNC( 
		cudaMalloc3DArray(
			ppArray, 
			&cChannelDesc, 
			cExtent3D) );
}

//////////////////////////////////////////////////////////////
template<typename T>
void
_Memcpy3DToDevice
(
	cudaExtent cExtent3D, 

	T *pfSrc_host,
	cudaPitchedPtr *pSrc_pitched,
	cudaArray  *pcSrc_array,

	cudaPitchedPtr *pDst_pitched,
	cudaArray  *pcDst_array,

	int iOffsetX = 0,
	int iOffsetY = 0,
	int iOffsetZ = 0
)
{
	cudaMemcpy3DParms cMemcpy3DParams = {0};
	// setup the src pointer
	if( NULL != pfSrc_host )
	{
		cMemcpy3DParams.srcPtr = make_cudaPitchedPtr(
			(void*)pfSrc_host, 
			cExtent3D.width * sizeof(T),
			cExtent3D.width * sizeof(T), 
			cExtent3D.height);
		// the memory is transfered from host to device
		cMemcpy3DParams.kind = cudaMemcpyHostToDevice;
	}

	if( NULL != pSrc_pitched )
	{
		cMemcpy3DParams.srcPtr		= *pSrc_pitched;

		// the memory is transfered on device
		cMemcpy3DParams.kind = cudaMemcpyDeviceToDevice;
	}

	if( NULL != pcSrc_array )
	{
		cMemcpy3DParams.srcArray	= pcSrc_array;

		// the memory is transfered on device
		cMemcpy3DParams.kind = cudaMemcpyDeviceToDevice;
	}

	// setup the dst pointer and extent
	if( NULL != pDst_pitched )
	{
		cMemcpy3DParams.dstPtr		= *pDst_pitched;
		cMemcpy3DParams.extent		= cExtent3D;
		cMemcpy3DParams.extent.width *= sizeof(T);
	}

	if( NULL != pcDst_array )
	{
		cMemcpy3DParams.dstArray	= pcDst_array;
		cMemcpy3DParams.extent		= cExtent3D;
	}

	// setup the offsete
	cMemcpy3DParams.dstPos		= make_cudaPos(iOffsetX, iOffsetY, iOffsetZ);

	// execute the memory copy
	CUDA_SAFE_CALL_NO_SYNC( 
		cudaMemcpy3D(&cMemcpy3DParams) );  
}

template<typename T>
void
_Memcpy3DToHost
(
	cudaExtent cExtent3D, 
	cudaPitchedPtr *pSrc_pitched,
	T *pDst_host
)
{
	cudaExtent cExtent = cExtent3D;
	cExtent.width *= sizeof(T);

	cudaMemcpy3DParms cCopyParamsDeviceToHost = {0};
	cCopyParamsDeviceToHost.srcPtr	= *pSrc_pitched;
	cCopyParamsDeviceToHost.dstPtr	= make_cudaPitchedPtr(
		(void*)pDst_host, 
		cExtent3D.width * sizeof(T),
		cExtent3D.width * sizeof(T), 
		cExtent3D.height);
	cCopyParamsDeviceToHost.extent		= cExtent;
	cCopyParamsDeviceToHost.kind		= cudaMemcpyDeviceToHost;
	CUDA_SAFE_CALL( cudaMemcpy3D(&cCopyParamsDeviceToHost) );  
}

//////////////////////////////////////////////////////////////
#define SETUP_TEXTURE(T, AMODE0, AMODE1, AMODE2, FILTER, IS_NORMALIZED)	\
	{											\
		T.addressMode[0] = AMODE0;				\
		T.addressMode[1] = AMODE1;				\
		T.addressMode[2] = AMODE2;				\
		T.filterMode	 = FILTER;				\
		T.normalized	 = IS_NORMALIZED;		\
	}

//////////////////////////////////////////////////////////////

