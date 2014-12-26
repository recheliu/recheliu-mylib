#pragma once

#include <sstream>
#include <vector>
using namespace std;

#include <stdarg.h>
#include <stdio.h>

#include "liblog.h"

// ADD-BY-LEETEN 2014/12/25-BEGIN
namespace NDArray {
// ADD-BY-LEETEN 2014/12/25-END

// a template for N-dimensional array.
template <typename t, size_t NDIMS=1> 
class TVectorND: public vector<t>
{
protected:
	vector<size_t> vuDimLengths;

public:
	size_t UGetDimLength(size_t d)
	{
		ASSERT_OR_LOG(d < NDIMS, "");
		return vuDimLengths[d];
	}

	TVectorND():
		vector<t>(),
		vuDimLengths(NDIMS)
	{
	}
	
	void resize(const vector<size_t>& vuNewDimLengths) 
	{
		ASSERT_OR_LOG(NDIMS == vuNewDimLengths.size(), "");
		vuDimLengths.clear();
		size_t uNrOfPoints = 1;
		for(size_t d = 0; d < NDIMS; d++) 
		{	
			size_t uDimLength = vuNewDimLengths[d];
			ASSERT_OR_LOG(uDimLength > 0, "");
			vuDimLengths.push_back(uDimLength);
			uNrOfPoints *= uDimLength;
		}	
		vector<t>::resize(uNrOfPoints);
	}
	
	void resize(size_t l, va_list vaDimLengths) 
	{
		vector<size_t> vuNewDimLengths;
		vuNewDimLengths.push_back(l);
		for(size_t d = 0; d < NDIMS - 1; d++) 
		{	
			vuNewDimLengths.push_back(va_arg(vaDimLengths, size_t));
		}	
		resize(vuNewDimLengths);
	}

	void resize(size_t l, ...) 
	{
		va_list vaDimLengths;	
		va_start(vaDimLengths, l);	
		resize(l, vaDimLengths);
		va_end(vaDimLengths);	
	}
	
	TVectorND(size_t l, ...):
		vector<t>()
	{
		va_list vaDimLengths;	
		va_start(vaDimLengths, l);	
		resize(l, vaDimLengths);
		va_end(vaDimLengths);	
	}
	
	TVectorND(const vector<size_t>& vuNewDimLengths):
		vector<t>()
	{
		resize(vuNewDimLengths);
	}

	const size_t uGetIndex(size_t i, va_list vaDimIndices) const
	{
		size_t uSliceSize = vuDimLengths[0];
		size_t uIndex = i;
		for(size_t d = 1; d < NDIMS; d++) 
		{	
			size_t uDimIndex = va_arg(vaDimIndices, size_t);
			ASSERT_OR_LOG(uIndex < vuDimLengths[d], "");
			uIndex += uDimIndex * uSliceSize;
			uSliceSize *= vuDimLengths[d];
		}	
		return uIndex;
	}

	t& _Get(size_t i, ...)
	{
		va_list vaDimIndices;	
		va_start(vaDimIndices, i);	
		size_t uIndex = uGetIndex(i, vaDimIndices);
		va_end(vaDimIndices);	
		return data()[uIndex];
	}

	const t& _Get(size_t i, ...) const
	{
		va_list vaDimIndices;	
		va_start(vaDimIndices, i);	
		size_t uIndex = uGetIndex(i, vaDimIndices);
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
			ullTemp = vuDimLengths[d];
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

		vector<size_t> vuNewDimLengths;
		for(size_t d = 0; d < NDIMS; d++) 
		{
			fread(&ullTemp, sizeof(ullTemp), 1, fp);
			vuNewDimLengths.push_back((t)ullTemp);
		}
		resize(vuNewDimLengths);

		fread(data(), sizeof(t), size(), fp);

		fclose(fp);
	}
};

// ADD-BY-LEETEN 2014/12/25-BEGIN
}	// namespace NDArray
// ADD-BY-LEETEN 2014/12/25-END

