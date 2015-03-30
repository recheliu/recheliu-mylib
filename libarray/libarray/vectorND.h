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
	// ADD-BY-LEETEN 2014/12/28-BEGIN
	const 
	vector<size_t>& 
	VUGetDimLengths()
	const
	{
		return vuDimLengths;
	}
	// ADD-BY-LEETEN 2014/12/28-END

	// MOD-BY-LEETEN 2015/03/29:	size_t UGetDimLength(size_t d)
	size_t 
	UGetDimLength(size_t d) const 
	// MOD-BY-LEETEN 2015/03/29-END
	{
		ASSERT_OR_LOG(d < NDIMS, "");
		return vuDimLengths[d];
	}

	#if	0	// DEL-BY-LEETEN 2014/12/25-BEGIN
	TVectorND():
		vector<t>(),
		vuDimLengths(NDIMS)
	{
	}
	#endif	// DEL-BY-LEETEN 2014/12/25-END
	
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
			#if	0	// DEL-BY-LEETEN 2015/03/29-BEGIN
			ASSERT_NETCDF(
				nc_get_vara_double(iNcId, ncVarSAT, puStarts, puCounts, pdSums) );
			#endif	// DEL-BY-LEETEN 2015/03/29-END

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
	
	// ADD-BY-LEETEN 2014/12/25-BEGIN
	TVectorND():
		vector<t>(),
		vuDimLengths(NDIMS)
	{
	}
	// ADD-BY-LEETEN 2014/12/25-END
	
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

	#if	0	// MOD-BY-LEETEN 2014/12/25-FROM:	
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

	#else	// MOD-BY-LEETEN 2014/12/25-TO:
	size_t 
	UGetIndex
	(
		const vector<size_t>& vuSubscripts
	) 
	const
	{
		size_t uSliceSize = 1;
		size_t uIndex = 0;
		for(size_t d = 0; d < NDIMS; d++) 
		{	
			size_t uDimSub = vuSubscripts[d];
			ASSERT_OR_LOG(uDimSub < vuDimLengths[d], "");
			uIndex += uDimSub * uSliceSize;
			uSliceSize *= vuDimLengths[d];
		}	
		return uIndex;
	}

	size_t 
	UGetIndex
	(
		size_t i, 
		va_list vaDimIndices
	) 
	const
	{
		size_t uSliceSize = vuDimLengths[0];
		static vector<size_t> vuSubs(NDIMS);
		vuSubs[0] = i;
		for(size_t d = 1; d < NDIMS; d++) 
		{	
			vuSubs[d] = va_arg(vaDimIndices, size_t);
		}	
		return UGetIndex(vuSubs);
	}

	t& _Get(const vector<size_t>& vuSub)
	{
		return data()[UGetIndex(vuSub)];
	}

	const t& _Get(const vector<size_t>& vuSub) const
	{
		return data()[UGetIndex(vuSub)];
	}
	#endif	// MOD-BY-LEETEN 2014/12/25-END

	t& _Get(size_t i, ...)
	{
		va_list vaDimIndices;	
		va_start(vaDimIndices, i);	
		// MOD-BY-LEETEN 2014/12/25:	size_t uIndex = uGetIndex(i, vaDimIndices);
		size_t uIndex = UGetIndex(i, vaDimIndices);
		// MOD-BY-LEETEN 2014/12/25-END
		va_end(vaDimIndices);	
		return data()[uIndex];
	}

	const t& _Get(size_t i, ...) const
	{
		va_list vaDimIndices;	
		va_start(vaDimIndices, i);	
		// MOD-BY-LEETEN 2014/12/25:	size_t uIndex = uGetIndex(i, vaDimIndices);
		size_t uIndex = UGetIndex(i, vaDimIndices);
		// MOD-BY-LEETEN 2014/12/25-END
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
		#if	0	// MOD-BY-LEETEN 2015/03/29-FROM:
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
		#else	// MOD-BY-LEETEN 2015/03/29-TO:
		string strHeaderFilepath= strFilename + ".txt";
		string strRawFilepath = strFilename + ".raw";

		fp = fopen(strHeaderFilepath.c_str(), "wt");
		ASSERT_OR_LOG(fp, perror(strHeaderFilepath.c_str()));
		fprintf(fp, "%d\n", NDIMS);
		for(size_t d = 0; d < NDIMS; d++) 
		{
			fprintf(fp, "%d\n", vuDimLengths[d]);
		}

		size_t uLastIndexOfSlash = strRawFilepath.find_last_of("/");
		#if	defined(WIN32)
		if( string::npos == uLastIndexOfSlash ) 
			uLastIndexOfSlash = strRawFilepath.find_last_of("\\");
		#endif	// #if	defined(WIN32)
		string strRawFilename = strRawFilepath;
		if( string::npos != uLastIndexOfSlash ) 
			strRawFilename = strRawFilepath.substr(uLastIndexOfSlash + 1);
		fprintf(fp, "%s\n", strRawFilename.c_str());
		fclose(fp);

		fp = fopen(strRawFilepath.c_str(), "wb");
		ASSERT_OR_LOG(fp, perror(strRawFilename.c_str()));
		fwrite(data(), sizeof(t), size(), fp);
		fclose(fp);
		#endif	// MOD-BY-LEETEN 2015/03/29-END
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

