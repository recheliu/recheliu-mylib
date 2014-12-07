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
		//! Print the current value of the specified variable (*pDest) with the name szName.
		// ADD-BY-LEETEN 2014/12/06-END
		virtual void _Print(string szName, void* pDest) const = 0;

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

		COptEntry(string szArgName):
			szArgName(szArgName),
			szDescription("")
		{
		}

	};
}

