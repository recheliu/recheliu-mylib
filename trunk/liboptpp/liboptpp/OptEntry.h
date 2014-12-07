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

// ADD-BY-LEETEN 2014/12/06-BEGIN
#include "liblog.h"
// ADD-BY-LEETEN 2014/12/06-END

namespace OptExt {
	struct COptEntry 
	{
		string szArgName;
		string szDescription;

		// ADD-BY-LEETEN 2014/12/06-BEGIN
		void _PrintVarValue(const string& szTypeName, const string& szVarName, const string& szVarValue) const
		{
			cerr<<"\t"<<szTypeName<<" "<<szVarName<<" = "<<szVarValue<<";"<<endl;
		}
		// ADD-BY-LEETEN 2014/12/06-END

		// ADD-BY-LEETEN 2014/12/06-BEGIN
		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		void setDescription(string szDescription) 
		{
			this->szDescription = szDescription;
		}

		COptEntry* withDescription(string szDescription) 
		{
			setDescription(szDescription);
			return this;
		}
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		void setDescription(const string& szDescription) 
		{
			this->szDescription = szDescription;
		}

		COptEntry* withDescription(const string& szDescription) 
		{
			setDescription(szDescription);
			return this;
		}
		#endif	// MOD-BY-LEETEN 2014/12/06-END
		// ADD-BY-LEETEN 2014/12/06-END

		// ADD-BY-LEETEN 2014/12/06-BEGIN
		//! Print the current value of the specified variable (*pDest) with the name szName.
		// ADD-BY-LEETEN 2014/12/06-END
		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		virtual void _Print(string szName, void* pDest) const = 0;
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		virtual void _Print(const string& szName, void* pDest) const = 0;
		#endif	// MOD-BY-LEETEN 2014/12/06-END

		virtual void _PrintUsage()
		{
			fprintf(stderr, "\n%s:\t%s\n", szArgName.c_str(), szDescription.c_str());
		}

		void _CheckArgumentNumbers(size_t uIndex, size_t uNrOfValues, size_t uNrOfArguments) 
		{
					// check the number of parameters
			// MOD-BY-LEETEN 2014/12/06-FROM:
			/*
			if( uIndex + uNrOfValues >= uNrOfArguments) 
			{
				throw new invalid_argument("Too few parameters for " + this->szArgName);
			}
			*/
			// MOD-BY-LEETEN 2014/12/06-TO:
			ASSERT_OR_LOG(uIndex + uNrOfValues < uNrOfArguments, cerr<<"Too few parameters for " + this->szArgName);
			// MOD-BY-LEETEN 2014/12/06-END
		}


		// ADD-BY-LEETEN 2014/12/06-BEGIN
		//! Parse the value from *piStart.
		// ADD-BY-LEETEN 2014/12/06-END
		virtual void _Parse(size_t argc, char* argv[], int* piStart) = 0;

		COptEntry():
			szArgName(""),
			szDescription("")
		{
		}

		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		COptEntry(string szArgName):
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		COptEntry(const string& szArgName):
		#endif	// MOD-BY-LEETEN 2014/12/06-END
			szArgName(szArgName),
			szDescription("")
		{
		}

	};
}

