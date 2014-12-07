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

namespace OptExt {
	struct COptEntry 
	{
		string szArgName;
		string szDescription;

		virtual void _Print(string szName, void* pDest) const = 0;

		virtual void _PrintUsage()
		{
			fprintf(stderr, "\n%s:\t%s\n", szArgName.c_str(), szDescription.c_str());
		}

		void _CheckArgumentNumbers(size_t uIndex, size_t uNrOfValues, size_t uNrOfArguments) 
		{
					// check the number of parameters
			if( uIndex + uNrOfValues >= uNrOfArguments) 
			{
				throw new invalid_argument("Too few parameters for " + this->szArgName);
			}
		}


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

