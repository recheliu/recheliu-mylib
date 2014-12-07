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
		COptBoolean(const char* szArgName, DT* piDest, VT iDefaultValue):
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
			cout<<szArgName<<" <var>"<<endl;
			_Print("var", piDest);
		}

		virtual void _Print(string szName, void* pDest) const
		{
			char *szValue = (*(DT*)pDest)?"true":"false";
			fprintf(stderr, "\tbool %s = (%s)%s\n", szName.c_str(), typeid(DT).name(), szValue);
		}
	};
}

