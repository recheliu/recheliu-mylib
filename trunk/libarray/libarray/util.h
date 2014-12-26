#pragma once

#include <sstream>
#include <string>
using namespace std;

#include <string.h>

#include "vectorND.h"
#include "arrayND.h"

namespace NDArray {
	namespace Util {
		string STRGetVarName
			(
				const string& strFilename, 
				const string& strFunctionName,
				const string& strVarName
			)
		{
			ostringstream osFielname;
			#if defined(WIN32)
			osFielname<<strrchr(strFilename.c_str(), '\\') + 1;	
			#else	// #if defined(WIN32)	
			osFielname<<strFilename;	
			#endif	// #if defined(WIN32)
			osFielname<<"."<<strFunctionName<<"."<<strVarName;	
			string strNewFilename = osFielname.str();
			for(size_t i = strNewFilename.find(":"); i < strNewFilename.npos; i = strNewFilename.find(":")) 
			{
				strNewFilename.replace(i, 1, "_");
			}
			return strNewFilename;
		}
	}	// namespace Util 
}	// namespace NDArray

//! Save a ND array with its variable name as the filename.
#define ND_ARRAY_SAVE(array_name)	\
	{	\
		string strFilename = NDArray::Util::STRGetVarName(__FILE__, __FUNCTION__, #array_name);	\
		array_name._Save(strFilename); \
	}	

//! Load a ND array with its variable name as the filename.
#define ND_ARRAY_LOAD(array_name)	\
	{	\
		string strFilename = NDArray::Util::STRGetVarName(__FILE__, __FUNCTION__, #array_name);	\
		array_name._Load(strFilename); \
	}	


