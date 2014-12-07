#pragma once

#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#include <assert.h>
#include <stdarg.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "OptValueVector.h"

namespace OptExt {
	template<class DT>
	struct COptPrimitiveTypeVector: public COptValueVector<DT>
	{
		virtual void _ParseValue(const char* szArg, DT& Dest) 
		{
			string str(szArg);
			std::istringstream is(str);
			is >> Dest;
		}

		COptPrimitiveTypeVector(const char* szArgName, size_t uNrOfDests, ...):
			COptValueVector(szArgName)
		{
			va_list vaDests;	
			va_start(vaDests, uNrOfDests);	
			_SetDests(uNrOfDests, vaDests);
			va_end(vaDests);	
		}
	};

	typedef COptPrimitiveTypeVector<int> COptIntVector;
	typedef COptPrimitiveTypeVector<float> COptFloatVector;
	typedef COptPrimitiveTypeVector<double> COptDoubleVector;
}

