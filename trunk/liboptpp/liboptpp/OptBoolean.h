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
		COptBoolean(const string& szArgName, DT* piDest, VT iDefaultValue):
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
			cerr<<szArgName<<" <"<<pairDest.first<<">"<<endl;
			_Print(pairDest.first, pairDest.second);
		}

		virtual void _Print(const string& szName, void* pDest) const
		{
			ostringstream os;
			os<<"("<<typeid(DT).name()<<")"<<((*(DT*)pDest)?"true":"false");
			_PrintVarValue("bool", szName, os.str());
		}
	};
}

