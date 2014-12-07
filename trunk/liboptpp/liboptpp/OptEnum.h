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
	template<class DT=int>
	struct COptEnum: public COptEntry
	{
		pair<string, DT*> pairDest;
		unordered_map<string, DT> umapNameValue;

		const string& SZGetValueName(DT iValue) const
		{
			for(unordered_map<string, DT>::const_iterator 
					imapNameValue = umapNameValue.begin(); 
				imapNameValue != umapNameValue.end(); 
				imapNameValue++)
			{
				if( imapNameValue->second == iValue) 
				{
					return imapNameValue->first;
				}
			}
			ASSERT_OR_LOG(false, cerr<<"Cannot find the name for value "<<iValue);
		}

		COptEnum(const string& szArgName, DT* piDest, DT iDefaultValue, size_t uNrOfNames, ...):
			COptEntry(szArgName)
		{
			pairDest.first = string("var");
			pairDest.second= piDest;

			va_list vaNames;	
			va_start(vaNames, uNrOfNames);	
			for(size_t n = 0; n < uNrOfNames; n++) 
			{	
				string szName = string(va_arg(vaNames, const char*));
				DT iValue = va_arg(vaNames, DT);
				umapNameValue[szName] = iValue;
			}	
			va_end(vaNames);	
			// Check whether the value is valid.
			// This must be called after the map has been configured.
			SZGetValueName(iDefaultValue);
			*piDest = iDefaultValue;
		}

		virtual void _PrintUsage()
		{
			COptEntry::_PrintUsage();
			cerr<<szArgName<<" <"<<pairDest.first<<">"<<endl;
			fprintf(stderr, "\tAllowed values of <var>:\n");
			fprintf(stderr, "\t{\n");
			for(unordered_map<string, DT>::const_iterator 
					imapNameValue = umapNameValue.begin(); 
				imapNameValue != umapNameValue.end(); 
				imapNameValue++)
			{
				cerr<<"\t\t"<<imapNameValue->first<<","<<endl;
			}
				fprintf(stderr, "\t}\n");
			_Print(pairDest.first, pairDest.second);
		}

		virtual void _Parse(size_t argc, char* argv[], int *piStart) 
		{
			_CheckArgumentNumbers(*piStart, 1, argc);

			// set the parameters
			string strName(argv[++*piStart]);
			unordered_map<string, DT>::iterator iumapNameValue = umapNameValue.find(strName);
			ASSERT_OR_LOG( iumapNameValue != umapNameValue.end(), cerr<<"Unknown value " + strName + " for " + this->szArgName );
			*pairDest.second = iumapNameValue->second;
		}

		virtual void _Print(const string& szName, void* pDest) const
		{
			DT iValue = *(DT*)pDest;
			string szValueName = SZGetValueName(iValue);
			_PrintVarValue("enum", szName, szValueName);
		}
	};
}

