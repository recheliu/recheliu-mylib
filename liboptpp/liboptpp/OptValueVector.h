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

#include "OptEntry.h"

namespace OptExt {
	template<class DT, class VT=DT>
	struct COptValueVector: public COptEntry
	{
		vector<pair<string, void*>> vpairDests;

		virtual void _Print(const string& szName, void* pDest) const 
		{
			ostringstream os;
			os<<*(DT*)pDest;
			_PrintVarValue(typeid(DT).name(), szName, os.str());
		}

		virtual void _ParseValue(const char* szArg, DT& Dest) = 0;

		virtual void _PrintUsage()
		{
			COptEntry::_PrintUsage();
			cerr<<szArgName<<" ";
			for(vector<pair<string, void*>>::iterator 
					ivpairDest = vpairDests.begin();
				ivpairDest != vpairDests.end();
				ivpairDest++)
			{									
				cerr<<"<"<<ivpairDest->first<<"> ";
			}

			fprintf(stderr, "\n");
			for(vector<pair<string, void*>>::iterator 
					ivpairDest = vpairDests.begin();
				ivpairDest != vpairDests.end();
				ivpairDest++)
			{
				_Print(ivpairDest->first, ivpairDest->second);
			}
		}

		virtual void _Parse(size_t argc, char* argv[], int *piStart) 
		{
			// check the number of parameters
			_CheckArgumentNumbers(*piStart, this->vpairDests.size(), argc);
			for(vector<pair<string, void*>>::iterator 
					ivpairDest = vpairDests.begin();
				ivpairDest != vpairDests.end();
				ivpairDest++)
			{
				_ParseValue(argv[++*piStart], *(DT*)(ivpairDest->second));	
			}
		}

		void _SetDests(size_t uNrOfDests, va_list vaDests)
		{
			// MOD-BY-LETEN 2014/12/25:	for(int d = 0; d < uNrOfDests; d++) 
			for(size_t d = 0; d < uNrOfDests; d++) 
			// MOD-BY-LETEN 2014/12/25-END
			{	
				DT *pDest = va_arg(vaDests, DT*);	
				VT default_value = va_arg(vaDests, VT);	
				*pDest = DT(default_value);	
				char szValueName[16];
				sprintf(szValueName, "var%d", d);
				vpairDests.push_back(make_pair<string, void*>(szValueName, (void*)pDest));
			}	
		}

		COptValueVector() 
		{
		}

		COptValueVector(const string& szArgName):
			COptEntry(szArgName)
		{
		}

		COptValueVector(const string& szArgName, size_t uNrOfDests, va_list vaDests):
			COptEntry(szArgName)
		{
			_SetDests(uNrOfDests, vaDests);
		}
	};
}

