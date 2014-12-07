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
		// MOD-BY-LEETEN 2014/12/06:	unordered_map<string, COptEntry*> mapNameOpt;
		map<string, COptEntry*> mapNameOpt;
		// MOD-BY-LEETEN 2014/12/06-END

		// MOD-BY-LEETEN 2014/12/06-FROM:
		/*
		void 
		_AddOptEntry(COptEntry *pcEntry) 
		{
			mapNameOpt[pcEntry->szArgName] = pcEntry;
		}
		*/
		// MOD-BY-LEETEN 2014/12/06-TO:
		//! The entries to release by the constructor.
		list<COptEntry*> lpEntryToRelease;

		// ADD-BY-LEETEN 2014/12/06-BEGIN
		unordered_map<string, bool> umapRequiredArgs;

		void
		_SetRequired(const string& szArg, bool bIsRequired) 
		{
			if( bIsRequired ) 
			{
				umapRequiredArgs[szArg] = false;
			}
			else
			{
				unordered_map<string, bool>::iterator iumapRequiredArg = umapRequiredArgs.find(szArg);
				if( iumapRequiredArg != umapRequiredArgs.end() ) 
				{
					umapRequiredArgs.erase(iumapRequiredArg);
				}
			}
		}
		// ADD-BY-LEETEN 2014/12/06-END

		void 
		// MOD-BY-LEETEN 2014/12/06:	_AddOptEntry(COptEntry *pcEntry, bool bNotToRelease = false) 
		_AddOptEntry(COptEntry *pcEntry, bool bIsRequired=false, bool bNotToRelease = false) 
		// MOD-BY-LEETEN 2014/12/06-END
		{
			mapNameOpt[pcEntry->szArgName] = pcEntry;
			if( !bNotToRelease ) 
			{
				lpEntryToRelease.push_back(pcEntry);
			}
		}
		// MOD-BY-LEETEN 2014/12/06-END

		void
		_OptPrintComment()
		{
			// MOD-BY-LEETEN 2014/12/06:	for(unordered_map<string, COptEntry*>::iterator 
			for(map<string, COptEntry*>::iterator 
			// MOD-BY-LEETEN 2014/12/06-END
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
				// MOD-BY-LEETEN 2014/12/06:	unordered_map<string, COptEntry*>::iterator imapNameOpt = mapNameOpt.find(string(argv[iA]));
				map<string, COptEntry*>::iterator imapNameOpt = mapNameOpt.find(string(argv[iA]));
				// MOD-BY-LEETEN 2014/12/06-END
				if( mapNameOpt.end() == imapNameOpt ) 
				{
					// Stop if found a unknown argument.
					break;
				}
				COptEntry *pcOptEntry = imapNameOpt->second;
				pcOptEntry->_Parse(argc, argv, &iA);
				// ADD-BY-LEETEN 2014/12/06-BEGIN
				unordered_map<string, bool>::iterator iumapRequiredArg = umapRequiredArgs.find(pcOptEntry->szArgName);
				if( iumapRequiredArg != umapRequiredArgs.end() ) 
				{
					iumapRequiredArg->second = true;
				}
				// ADD-BY-LEETEN 2014/12/06-END
			}

			// ADD-BY-LEETEN 2014/12/06-BEGIN
			size_t uNrOfMissedArguments = 0;
			for(unordered_map<string, bool>::iterator 
					iumapRequiredArg = umapRequiredArgs.begin();
				iumapRequiredArg != umapRequiredArgs.end();
				iumapRequiredArg++)
			{
				if( !iumapRequiredArg->second ) 
				{
					LOG(cerr<<"Miss required argument "<<iumapRequiredArg->first);
					uNrOfMissedArguments++;
				}
			}
			if( 0 < uNrOfMissedArguments ) 
			{
				bResult = false;
			}

			// ADD-BY-LEETEN 2014/12/06-END

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
			// ADD-BY-LEETEN 2014/12/06-BEGIN
			for(list<COptEntry*>::iterator 
					ilpEntry = lpEntryToRelease.begin();
				ilpEntry != lpEntryToRelease.end();
				ilpEntry++)
			{
				delete *ilpEntry;
			}
			lpEntryToRelease.clear();
			// ADD-BY-LEETEN 2014/12/06-END

		}
	};
}

