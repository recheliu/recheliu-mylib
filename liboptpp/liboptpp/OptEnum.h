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
		// MOD-BY-LEETEN 2014/12/06:	DT* piDest;
		pair<string, DT*> pairDest;
		// MOD-BY-LEETEN 2014/12/06-END
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
			// MOD-BY-LEETEN 2014/12/06:	throw new invalid_argument("Cannot find the name for value " + iValue);
			ASSERT_OR_LOG(false, cerr<<"Cannot find the name for value "<<iValue);
			// MOD-BY-LEETEN 2014/12/06-END
		}

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		COptEnum(string szArgName, DT* piDest, DT iDefaultValue, size_t uNrOfNames, ...):
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		COptEnum(const string& szArgName, DT* piDest, DT iDefaultValue, size_t uNrOfNames, ...):
		#endif	// MOD-BY-LEETEN 2014/12/06-END
			COptEntry(szArgName)
		{
			// MOD-BY-LEETEN 2014/12/06:	this->piDest = piDest;
			pairDest.first = string("var");
			pairDest.second= piDest;
			// MOD-BY-LEETEN 2014/12/06-END

			va_list vaNames;	
			va_start(vaNames, uNrOfNames);	
			for(size_t n = 0; n < uNrOfNames; n++) 
			{	
				string szName = string(va_arg(vaNames, const char*));
				DT iValue = va_arg(vaNames, DT);
				umapNameValue[szName] = iValue;
			}	
			va_end(vaNames);	
			// MOD-BY-LEETEN 2014/12/06-FROM:
			/*
			SZGetValueName(iDefaultValue);
			*piDest = iDefaultValue;
			*/
			// MOD-BY-LEETEN 2014/12/06-TO:
			// Check whether the value is valid.
			// This must be called after the map has been configured.
			SZGetValueName(iDefaultValue);
			*piDest = iDefaultValue;
			// MOD-BY-LEETEN 2014/12/06-END
		}

		virtual void _PrintUsage()
		{
			COptEntry::_PrintUsage();
			// MOD-BY-LEETEN 2014/12/06:	cerr<<szArgName<<" <var>"<<endl;
			cerr<<szArgName<<" <"<<pairDest.first<<">"<<endl;
			// MOD-BY-LEETEN 2014/12/06-END
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
			// MOD-BY-LEETEN 2014/12/06:	_Print(string("var"), piDest);
			_Print(pairDest.first, pairDest.second);
			// MOD-BY-LEETEN 2014/12/06-END
		}

		virtual void _Parse(size_t argc, char* argv[], int *piStart) 
		{
			_CheckArgumentNumbers(*piStart, 1, argc);

			// set the parameters
			string strName(argv[++*piStart]);
			unordered_map<string, DT>::iterator iumapNameValue = umapNameValue.find(strName);
			// MOD-BY-LEETEN 2014/12/06-FROM:
			/*
			if( iumapNameValue == umapNameValue.end() ) 
			{
				throw new invalid_argument("Unknown value " + strName + " for " + this->szArgName);
			}
			*/
			// MOD-BY-LEETEN 2014/12/06-TO:
			ASSERT_OR_LOG( iumapNameValue != umapNameValue.end(), cerr<<"Unknown value " + strName + " for " + this->szArgName );
			// MOD-BY-LEETEN 2014/12/06-END
			// MOD-BY-LEETEN 2014/12/06:	*this->piDest = iumapNameValue->second;
			*pairDest.second = iumapNameValue->second;
			// MOD-BY-LEETEN 2014/12/06-END
		}

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		virtual void _Print(string szName, void* pDest) const
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		virtual void _Print(const string& szName, void* pDest) const
		#endif	// MOD-BY-LEETEN 2014/12/06-END
		{
			DT iValue = *(DT*)pDest;
			string szValueName = SZGetValueName(iValue);
			// MOD-BY-LEETEN 2014/12/06:	fprintf(stderr, "\tenum %s = %s\n", szName.c_str(), szValueName.c_str());
			_PrintVarValue("enum", szName, szValueName);
			// MOD-BY-LEETEN 2014/12/06-END
		}
	};
}

