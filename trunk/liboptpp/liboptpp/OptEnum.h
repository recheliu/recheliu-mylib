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
		DT* piDest;
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
			throw new invalid_argument("Cannot find the name for value " + iValue);
		}

		COptEnum(string szArgName, DT* piDest, DT iDefaultValue, size_t uNrOfNames, ...):
			COptEntry(szArgName)
		{
			this->piDest = piDest;

			va_list vaNames;	
			va_start(vaNames, uNrOfNames);	
			for(size_t n = 0; n < uNrOfNames; n++) 
			{	
				string szName = string(va_arg(vaNames, const char*));
				DT iValue = va_arg(vaNames, DT);
				umapNameValue[szName] = iValue;
			}	
			va_end(vaNames);	
			SZGetValueName(iDefaultValue);
			*piDest = iDefaultValue;
		}

		virtual void _PrintUsage()
		{
			COptEntry::_PrintUsage();
			cerr<<szArgName<<" <var>"<<endl;
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
			_Print(string("var"), piDest);
		}

		virtual void _Parse(size_t argc, char* argv[], int *piStart) 
		{
			_CheckArgumentNumbers(*piStart, 1, argc);

			// set the parameters
			string strName(argv[++*piStart]);
			unordered_map<string, DT>::iterator iumapNameValue = umapNameValue.find(strName);
			if( iumapNameValue == umapNameValue.end() ) 
			{
				throw new invalid_argument("Unknown value " + strName + " for " + this->szArgName);
			}
			*this->piDest = iumapNameValue->second;
		}

		virtual void _Print(string szName, void* pDest) const
		{
			DT iValue = *(DT*)pDest;
			string szValueName = SZGetValueName(iValue);
			fprintf(stderr, "\tenum %s = %s\n", szName.c_str(), szValueName.c_str());
		}
	};
}

