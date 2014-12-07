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

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		virtual void _Print(string szName, void* pDest) const 
		{
			cerr<<"\tchar* "<<szName<<" = ";
			// MOD-BY-LEETEN 2014/12/06:	fprintf(stderr, "%s\n", *(const char**)pDest);
			fprintf(stderr, "\"%s\"\n", *(const char**)pDest);
			// MOD-BY-LEETEN 2014/12/06-END
		}
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		virtual void _Print(const string& szName, void* pDest) const 
		{
			string szVarValue("(null)");
			if( NULL != *(const char**)pDest ) {
				ostringstream os;
				os<<"\""<<*(const char**)pDest<<"\"";
				szVarValue = os.str();
			}
			_PrintVarValue("const char*", szName, szVarValue);
		}
		#endif	// MOD-BY-LEETEN 2014/12/06-END

		virtual void _ParseValue(const char* szArg, DT& Dest) 
		{
			Dest = DT(szArg);
		}

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		COptCharPtrVector(const char* szArgName, size_t uNrOfDests, ...):
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		COptCharPtrVector(const string& szArgName, size_t uNrOfDests, ...):
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

