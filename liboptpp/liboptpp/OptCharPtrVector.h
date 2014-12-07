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
	struct COptCharPtrVector:
		public COptValueVector<const char*, const char*> 
	{
		typedef const char* DT;

		virtual void _Print(string szName, void* pDest) const 
		{
			cerr<<"\tchar* "<<szName<<" = ";
			fprintf(stderr, "%s\n", *(const char**)pDest);
		}

		virtual void _ParseValue(const char* szArg, DT& Dest) 
		{
			Dest = DT(szArg);
		}

		COptCharPtrVector(const char* szArgName, size_t uNrOfDests, ...):
			COptValueVector(szArgName)
		{
			va_list vaDests;	
			va_start(vaDests, uNrOfDests);	
			_SetDests(uNrOfDests, vaDests);
			va_end(vaDests);	
		}
	};
}

