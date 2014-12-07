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

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		virtual void _Print(string szName, void* pDest) const 
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		virtual void _Print(const string& szName, void* pDest) const 
		#endif	// MOD-BY-LEETEN 2014/12/06-END
		{
			// MOD-BY-LEETEN 2014/12/06:	cerr<<"\tstring "<<szName<<" = \""<<*(string*)pDest<<"\""<<endl;
			ostringstream os;
			os<<"\""<<*(string*)pDest<<"\"";
			_PrintVarValue("string", szName, os.str());
			// MOD-BY-LEETEN 2014/12/06-END
		}

		virtual void _ParseValue(const char* szArg, DT& Dest) 
		{
			Dest = DT(szArg);
		}

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		COptStringVector(const char* szArgName, size_t uNrOfDests, ...):
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		COptStringVector(const string& szArgName, size_t uNrOfDests, ...):
		#endif	// MOD-BY-LEETEN 2014/12/06-END
			COptValueVector(szArgName)
		{
			va_list vaDests;	
			va_start(vaDests, uNrOfDests);	
			_SetDests(uNrOfDests, vaDests);
			va_end(vaDests);	
		}
	};
}

