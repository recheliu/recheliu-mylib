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

#include "OptEnum.h"

namespace OptExt {
	template<class DT=bool, class VT=DT>
	struct COptBoolean: public COptEnum<DT>
	{
		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		COptBoolean(const char* szArgName, DT* piDest, VT iDefaultValue):
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		COptBoolean(const string& szArgName, DT* piDest, VT iDefaultValue):
		#endif	// MOD-BY-LEETEN 2014/12/06-END
			COptEnum(szArgName, piDest, iDefaultValue, 4,
				"0",		DT(0),
				"false",	DT(0),
				"1",		DT(1),
				"true",		DT(1)
				)
		{
		}

		virtual void _PrintUsage()
		{
			COptEntry::_PrintUsage();
			// MOD-BY-LEETEN 2014/12/06-FROM:
			/*
			cout<<szArgName<<" <var>"<<endl;
			_Print("var", piDest);
			*/
			// MOD-BY-LEETEN 2014/12/06-TO:
			cerr<<szArgName<<" <"<<pairDest.first<<">"<<endl;
			_Print(pairDest.first, pairDest.second);
			// MOD-BY-LEETEN 2014/12/06-END
		}

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		virtual void _Print(string szName, void* pDest) const
		{
			char *szValue = (*(DT*)pDest)?"true":"false";
			fprintf(stderr, "\tbool %s = (%s)%s\n", szName.c_str(), typeid(DT).name(), szValue);
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		virtual void _Print(const string& szName, void* pDest) const
		{
			ostringstream os;
			os<<"("<<typeid(DT).name()<<")"<<((*(DT*)pDest)?"true":"false");
			_PrintVarValue("bool", szName, os.str());
		#endif	// MOD-BY-LEETEN 2014/12/06-END
		}
	};
}

