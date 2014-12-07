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
	struct COptStringVector:
		public COptValueVector<string, const char*> 
	{
		typedef string DT;

		virtual void _Print(string szName, void* pDest) const 
		{
			cerr<<"\tstring "<<szName<<" = \""<<*(string*)pDest<<"\""<<endl;
		}

		virtual void _ParseValue(const char* szArg, DT& Dest) 
		{
			Dest = DT(szArg);
		}

		COptStringVector(const char* szArgName, size_t uNrOfDests, ...):
			COptValueVector(szArgName)
		{
			va_list vaDests;	
			va_start(vaDests, uNrOfDests);	
			_SetDests(uNrOfDests, vaDests);
			va_end(vaDests);	
		}
	};
}

