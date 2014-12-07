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

#include "liblog.h"

namespace OptExt {
	struct COptEntry 
	{
		string szArgName;
		string szDescription;

		void _PrintVarValue(const string& szTypeName, const string& szVarName, const string& szVarValue) const
		{
			cerr<<"\t"<<szTypeName<<" "<<szVarName<<" = "<<szVarValue<<";"<<endl;
		}

		void setDescription(const string& szDescription) 
		{
			this->szDescription = szDescription;
		}

		COptEntry* withDescription(const string& szDescription) 
		{
			setDescription(szDescription);
			return this;
		}

		//! Print the current value of the specified variable (*pDest) with the name szName.
		virtual void _Print(const string& szName, void* pDest) const = 0;

		virtual void _PrintUsage()
		{
			fprintf(stderr, "\n%s:\t%s\n", szArgName.c_str(), szDescription.c_str());
		}

		void _CheckArgumentNumbers(size_t uIndex, size_t uNrOfValues, size_t uNrOfArguments) 
		{
					// check the number of parameters
			ASSERT_OR_LOG(uIndex + uNrOfValues < uNrOfArguments, cerr<<"Too few parameters for " + this->szArgName);
		}


		//! Parse the value from *piStart.
		virtual void _Parse(size_t argc, char* argv[], int* piStart) = 0;

		COptEntry():
			szArgName(""),
			szDescription("")
		{
		}

		COptEntry(const string& szArgName):
			szArgName(szArgName),
			szDescription("")
		{
		}

	};
}

