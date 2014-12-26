#pragma once

#include <sstream>
using namespace std;

#include <stdarg.h>
#include <stdio.h>

#include "liblog.h"

// ADD-BY-LEETEN 2014/12/25-BEGIN
namespace NDArray {
// ADD-BY-LEETEN 2014/12/25-END

// a template for N-dimensional array.
template <typename t, size_t NDIMS=1> 
class TArrayND
{
protected:
	size_t uNrOfPoints;
	t* pData;
	size_t puDimLengths[NDIMS];

	static void _MoveVAToArray(size_t puDimLength[NDIMS], size_t l1, va_list vaDimLengths) 
	{
		puDimLength[0] = l1;
		for(size_t d = 0; d < NDIMS - 1; d++) 
		{	
			puDimLength[d+1] = va_arg(vaDimLengths, size_t);
		}	
	}
public:
	size_t size()
	{
		return uNrOfPoints;
	}

	t* data() 
	{
		return pData;
	}

	const t* data() const 
	{
		return pData;
	}

	t& operator[](size_t i)
	{
		return pData[i];
	}

	const t& operator[](size_t i) const
	{
		return pData[i];
	}

	size_t UGetDimLength(size_t d) const 
	{
		ASSERT_OR_LOG(d < NDIMS, "");
		return puDimLengths[d];
	}

	void
	free() 
	{
		if( NULL != pData ) 
		{
			delete [] pData;
		}
		pData = NULL;
	}

	void resize(const size_t puNewDimLengths[NDIMS]) 
	{
		uNrOfPoints = 1;
		for(size_t d = 0; d < NDIMS; d++) 
		{	
			size_t uDimLength = puNewDimLengths[d];
			ASSERT_OR_LOG(uDimLength > 0, "");
			puDimLengths[d] = uDimLength;
			uNrOfPoints *= uDimLength;
		}	
		pData = new t[uNrOfPoints];
	}
	
	void resize(size_t l1, va_list vaDimLengths) 
	{
		static size_t puNewDimLengths[NDIMS];
		_MoveVAToArray(puNewDimLengths, l1, vaDimLengths);
		resize(puNewDimLengths);
	}

	void resize(size_t l1, ...) 
	{
		va_list vaDimLengths;	
		va_start(vaDimLengths, l1);	
		static size_t puNewDimLengths[NDIMS];
		_MoveVAToArray(puNewDimLengths, l1, vaDimLengths);
		resize(puNewDimLengths);
		va_end(vaDimLengths);	
	}
	
	TArrayND():
		uNrOfPoints(0),
		pData(NULL)
	{
	}
	
	TArrayND(size_t l1, ...):
		uNrOfPoints(0),
		pData(NULL)
	{
		va_list vaDimLengths;	
		va_start(vaDimLengths, l1);	
		resize(l1, vaDimLengths);
		va_end(vaDimLengths);	
	}
	
	TArrayND(const size_t puNewDimLengths[NDIMS]):
		uNrOfPoints(0),
		pData(NULL)
	{
		resize(puNewDimLengths);
	}


	~TArrayND()
	{
		free();
	}


	const size_t UGetIndex(const size_t puIndices[NDIMS]) const
	{
		size_t uSliceSize = 1;
		size_t uIndex = 0;
		for(size_t d = 0; d < NDIMS; d++) 
		{	
			size_t uDimIndex = puIndices[d];
			ASSERT_OR_LOG(uIndex < puDimLengths[d], "");
			uIndex += uDimIndex * uSliceSize;
			uSliceSize *= puDimLengths[d];
		}	
		return uIndex;
	}

	t& _Get(size_t i, ...)
	{
		va_list vaDimIndices;	
		va_start(vaDimIndices, i);	
		size_t puIndices[NDIMS];
		_MoveVAToArray(puIndices, i, vaDimIndices);
		size_t uIndex = UGetIndex(puIndices);
		va_end(vaDimIndices);	
		return data()[uIndex];
	}

	const t& _Get(size_t i, ...) const
	{
		va_list vaDimIndices;	
		va_start(vaDimIndices, i);	
		size_t puIndices[NDIMS];
		_MoveVAToArray(puIndices, i, vaDimIndices);
		size_t uIndex = UGetIndex(puIndices);
		va_end(vaDimIndices);	
		return data()[uIndex];
	}
	
	string
	STRGetFilename(const string& strFilenamePrefix)
	{
		ostringstream osFilename;
		osFilename<<strFilenamePrefix<<"."<<typeid(t).name()<<".v"<<NDIMS<<"d";
		return osFilename.str();
	}
	

	void _Save(const string& strFilenamePrefix)
	{
		string& strFilename = STRGetFilename(strFilenamePrefix);

		FILE *fp;
		fp = fopen(strFilename.c_str(), "wb");
		ASSERT_OR_LOG(fp, perror(strFilename.c_str()));
		
		unsigned long long ullTemp;
		
		ullTemp = NDIMS;
		fwrite(&ullTemp, sizeof(ullTemp), 1, fp);

		for(size_t d = 0; d < NDIMS; d++) 
		{
			ullTemp = puDimLengths[d];
			fwrite(&ullTemp, sizeof(ullTemp), 1, fp);
		}

		fwrite(data(), sizeof(t), size(), fp);

		fclose(fp);
	}
	
	void _Load(const string& strFilenamePrefix)
	{
		string& strFilename = STRGetFilename(strFilenamePrefix);

		FILE *fp;
		fp = fopen(strFilename.c_str(), "rb");
		ASSERT_OR_LOG(fp, perror(strFilename.c_str()));
		
		unsigned long long ullTemp;
		
		fread(&ullTemp, sizeof(ullTemp), 1, fp);
		ASSERT_OR_LOG(NDIMS == ullTemp, "");

		size_t puNewDimLengths[NDIMS];
		for(size_t d = 0; d < NDIMS; d++) 
		{
			fread(&ullTemp, sizeof(ullTemp), 1, fp);
			puNewDimLengths[d] = (t)ullTemp;
		}
		resize(puNewDimLengths);

		fread(data(), sizeof(t), size(), fp);

		fclose(fp);
	}
};

// ADD-BY-LEETEN 2014/12/25-BEGIN
}	// namespace NDArray
// ADD-BY-LEETEN 2014/12/25-END

