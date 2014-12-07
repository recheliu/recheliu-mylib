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
	#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:	
	template<class DT>
	struct COptPrimitiveTypeVector: public COptValueVector<DT>
	#else	// MOD-BY-LEETEN 2014/12/06-TO:
	template<class DT, class VT=DT>
	struct COptPrimitiveTypeVector: public COptValueVector<DT, VT>
	#endif	// MOD-BY-LEETEN 2014/12/06-END
	{
		virtual void _ParseValue(const char* szArg, DT& Dest) 
		{
			string str(szArg);
			std::istringstream is(str);
			is >> Dest;
		}

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		COptPrimitiveTypeVector(const char* szArgName, size_t uNrOfDests, ...):
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		COptPrimitiveTypeVector(const string& szArgName, size_t uNrOfDests, ...):
		#endif	// MOD-BY-LEETEN 2014/12/06-END
			COptValueVector(szArgName)
		{
			va_list vaDests;	
			va_start(vaDests, uNrOfDests);	
			_SetDests(uNrOfDests, vaDests);
			va_end(vaDests);	
		}
	};

	typedef COptPrimitiveTypeVector<int> COptIntVector;
	// MOD-BY-LEETEN 2014/12/06:	typedef COptPrimitiveTypeVector<float> COptFloatVector;
	typedef COptPrimitiveTypeVector<float, double> COptFloatVector;
	// MOD-BY-LEETEN 2014/12/06-END
	typedef COptPrimitiveTypeVector<double> COptDoubleVector;
}

