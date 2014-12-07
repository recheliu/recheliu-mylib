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
	struct COptParser 
	{
		unordered_map<string, COptEntry*> mapNameOpt;

		void 
		_AddOptEntry(COptEntry *pcEntry) 
		{
			mapNameOpt[pcEntry->szArgName] = pcEntry;
		}

		void
		_OptPrintComment()
		{
			for(unordered_map<string, COptEntry*>::iterator 
					imapNameOpt = mapNameOpt.begin(); 
				imapNameOpt != mapNameOpt.end(); 
				imapNameOpt++) 
			{
				imapNameOpt->second->_PrintUsage();
			}
		}

		bool BOPTParse(int argc, char* argv[], int iBegin=1, int *piEnd = NULL)
		{
			bool bResult = false;
			int iA;

			// check whether --help is specified. This check is applied first such that --help can always show the 
			// default value.
			for(iA = iBegin; iA < argc; iA++)
				if( !strcmp(argv[iA], "--help") )
				{
					_OptPrintComment();
					exit(0);
				}

			bResult = true;
			for(iA = iBegin; iA < argc; iA++)
			{
				unordered_map<string, COptEntry*>::iterator imapNameOpt = mapNameOpt.find(string(argv[iA]));
				if( mapNameOpt.end() == imapNameOpt ) 
				{
					// Stop if found a unknown argument.
					break;
				}
				COptEntry *pcOptEntry = imapNameOpt->second;
				pcOptEntry->_Parse(argc, argv, &iA);
			}

			if( piEnd )
				*piEnd = iA;

			return bResult;
		}
		
		virtual ~COptParser() 
		{
			/*
			for(unordered_map<string, COptEntry*>::iterator 
					imapNameOpt = mapNameOpt.begin();
				imapNameOpt != mapNameOpt.end();
				imapNameOpt++)
			{
				delete imapNameOpt->second;
			}
			*/
		}
	};
}

