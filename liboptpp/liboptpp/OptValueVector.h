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
		// MOD-BY-LEETEN 2014/12/06:	vector<void*> vpDests;
		vector<pair<string, void*>> vpairDests;
		// MOD-BY-LEETEN 2014/12/06-END

		// MOD-BY-LEETEN 2014/12/06:	virtual void _Print(string szName, void* pDest) const 
		virtual void _Print(const string& szName, void* pDest) const 
		// MOD-BY-LEETEN 2014/12/06-END
		{
			// MOD-BY-LEETEN 2014/12/06:	cerr<<"\t"<<typeid(DT).name()<<" "<<szName<<" = "<<*(DT*)pDest<<endl;
			ostringstream os;
			os<<*(DT*)pDest;
			_PrintVarValue(typeid(DT).name(), szName, os.str());
			// MOD-BY-LEETEN 2014/12/06-END
		}

		virtual void _ParseValue(const char* szArg, DT& Dest) = 0;

		virtual void _PrintUsage()
		{
			COptEntry::_PrintUsage();
			// MOD-BY-LEETEN 2014/12/06-FROM:
			/*
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
			*/
			// TO:
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
			// MOD-BY-LEETEN 2014/12/06-END
		}

		virtual void _Parse(size_t argc, char* argv[], int *piStart) 
		{
			// check the number of parameters
			// MOD-BY-LEETEN 2014/12/06-FROM:
			/*
			_CheckArgumentNumbers(*piStart, this->vpDests.size(), argc);

			// set the parameters
			for(vector<void*>::iterator 
					ivpDest = vpDests.begin();  
				ivpDest != vpDests.end();  
				ivpDest++ ) 
			{
				_ParseValue(argv[++*piStart], *(DT*)(*ivpDest));	
			}
			*/
			// TO:
			_CheckArgumentNumbers(*piStart, this->vpairDests.size(), argc);
			for(vector<pair<string, void*>>::iterator 
					ivpairDest = vpairDests.begin();
				ivpairDest != vpairDests.end();
				ivpairDest++)
			{
				_ParseValue(argv[++*piStart], *(DT*)(ivpairDest->second));	
			}
			// MOD-BY-LEETEN 2014/12/06-END
		}

		void _SetDests(size_t uNrOfDests, va_list vaDests)
		{
			for(int d = 0; d < uNrOfDests; d++) 
			{	
				DT *pDest = va_arg(vaDests, DT*);	
				VT default_value = va_arg(vaDests, VT);	
				*pDest = DT(default_value);	
				// MOD-BY-LEETEN 2014/12/06:	vpDests.push_back((void*)pDest);
				char szValueName[16];
				sprintf(szValueName, "var%d", d);
				vpairDests.push_back(make_pair<string, void*>(szValueName, (void*)pDest));
				// MOD-BY-LEETEN 2014/12/06-END
			}	
		}

		COptValueVector() 
		{
		}

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		COptValueVector(const char* szArgName):
			COptEntry(szArgName)
		{
		}

		COptValueVector(const char* szArgName, size_t uNrOfDests, va_list vaDests):
			COptEntry(szArgName)
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		COptValueVector(const string& szArgName):
			COptEntry(szArgName)
		{
		}

		COptValueVector(const string& szArgName, size_t uNrOfDests, va_list vaDests):
			COptEntry(szArgName)
		#endif	// MOD-BY-LEETEN 2014/12/06-END
		{
			_SetDests(uNrOfDests, vaDests);
		}
	};
}

