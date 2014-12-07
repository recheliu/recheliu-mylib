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
		vector<void*> vpDests;

		virtual void _Print(string szName, void* pDest) const 
		{
			cerr<<"\t"<<typeid(DT).name()<<" "<<szName<<" = "<<*(DT*)pDest<<endl;
		}

		virtual void _ParseValue(const char* szArg, DT& Dest) = 0;

		virtual void _PrintUsage()
		{
			COptEntry::_PrintUsage();
			fprintf(stderr, "%s ", szArgName.c_str());
			for(int d = 0; d < vpDests.size(); d++)
			{									
				fprintf(stderr, "<var%d> ", d);
			}
			fprintf(stderr, "\n");
			for(int d = 0; d < vpDests.size(); d++)
			{
				char szName[16];
				sprintf(szName, "var%d", d);
				_Print(string(szName), vpDests[d]);
			}
		}

		virtual void _Parse(size_t argc, char* argv[], int *piStart) 
		{
			// check the number of parameters
			_CheckArgumentNumbers(*piStart, this->vpDests.size(), argc);

			// set the parameters
			for(vector<void*>::iterator 
					ivpDest = vpDests.begin();  
				ivpDest != vpDests.end();  
				ivpDest++ ) 
			{
				_ParseValue(argv[++*piStart], *(DT*)(*ivpDest));	
			}
		}

		void _SetDests(size_t uNrOfDests, va_list vaDests)
		{
			for(int d = 0; d < uNrOfDests; d++) 
			{	
				DT *pDest = va_arg(vaDests, DT*);	
				VT default_value = va_arg(vaDests, VT);	
				*pDest = DT(default_value);	
				vpDests.push_back((void*)pDest);
			}	
		}

		COptValueVector() 
		{
		}

		COptValueVector(const char* szArgName):
			COptEntry(szArgName)
		{
		}

		COptValueVector(const char* szArgName, size_t uNrOfDests, va_list vaDests):
			COptEntry(szArgName)
		{
			_SetDests(uNrOfDests, vaDests);
		}
	};
}

