/***********************************************

TransFunc: 

This file defines a class for the transfer functions. 
In this class, a transfer function is defined as 4 
L1-splines for the RGBA channels.

The main funtionality of this class is to load transfer function,
save transfere function, export a color map from the splines.

To display the splines, it can be passed to a object belong to a 
frend class called CTfWin.

To edit the splines, it can be passed to a object belong to a 
frend class called CTfUi.

************************************************/


#include <list>
using namespace std;

#pragma once

	#include "knot.h"

	// ADD-BY-LEETEN 08/12/2009-BEGIN
	#include "FloatValue.h"
	// ADD-BY-LEETEN 08/12/2009-END

class CTransFunc {

// ADD-BY-TLEE 08/21/2008-BEGIN
	// MOD-BY-LEETEN 08/12/2009-FROM:
		// float fDomainMin, fDomainMax;
	// TO:
	CFloatValue cDomainMin, cDomainMax;
	// MOD-BY-LEETEN 08/12/2009-END

public:
	void _SetTfDomain(float fMin, float fMax)
	{
		#if	0	// MOD-BY-LEETEN 08/12/2009-FROM:
			fDomainMin = fMin;
			fDomainMax = fMax;
		#else	// MOD-BY-LEETEN 08/12/2009-TO:
		cDomainMin._SetValue(fMin);
		cDomainMax._SetValue(fMax);
		#endif	// MOD-BY-LEETEN 08/12/2009-END
	}

	void _GetTfDomain(float *pfMin, float *pfMax)
	{
		#if	0	// MOD-BY-LEETEN 08/12/2009-FROM:
			*pfMin = fDomainMin;
			*pfMax = fDomainMax;
		#else	// MOD-BY-LEETEN 08/12/2009-TO:
		*pfMin = cDomainMin.FGetValue();
		*pfMax = cDomainMax.FGetValue();
		#endif	// MOD-BY-LEETEN 08/12/2009-END
	}
// ADD-BY-TLEE 08/21/2008-END

public:
	enum {
		COLOR_R,
		COLOR_G,
		COLOR_B,
		COLOR_A,
		NR_OF_COLORS
	};

protected:
	list<CKnot>	plSplines[NR_OF_COLORS];		// the L1 spline

public:
	CTransFunc()
	{
		CKnot cBegin(0.0f, 0.0f), cEnd(1.0f, 0.0f);
		for(int c = 0; c < NR_OF_COLORS; c++)	
		{
			plSplines[c].push_back(cBegin);
			plSplines[c].push_back(cEnd);
		}

		#if	0	// DEL-BY-LEETEN 08/12/2009-BEGIN
			// ADD-BY-TLEE 08/14/2008-BEGIN
			fDomainMin = 0.0;
			fDomainMax = 1.0;
			// ADD-BY-TLEE 08/14/2008-END
		#endif	// DEL-BY-LEETEN 08/12/2009-END
	}

public:
	~CTransFunc()
	{
	}

public:
	void _LoadRainBow();

										// methods about load/save files
	#if	0	// MOD-BY-LEETEN 08/15/2008-FROM:
		void _Load(char szFilename[]);
		void _Save(char szFilename[]);
	#else	// MOD-BY-LEETEN 08/15/2008-TO:

										// this method checks the validabilty of the file
										// if the file is valid, return TRUE else FALSE
	bool BCheckFile(const char szFilename[]);

										// this method checks if the file's extension is ".TF"
										// if true, return TRUE else FALSE
	bool BCheckFilenameExt(const char szFilename[])
	{
		const char* szExt = strrchr(szFilename, '.');

						// the filename should contain an extension
		if( !szExt )
			return false;

						// the extension should be .tf ot .TF
		if( 0 != strcmp(szExt, ".tf") && 0 != strcmp(szExt, ".TF") )
			return false;

		return true;

	}
								
										// this method open the specified file 
										// if true, return TRUE else FALSE
	bool BOpenFile(const char szFilename[]);

										// this method write the four splines to the specified file 
										// if true, return TRUE else FALSE
	bool BSaveFile(const char szFilename[]);
	#endif	// MOD-BY-LEETEN 08/15/2008-END

	void _ExportColorMap(float pfColorMap[], int iNrOfEntries);

	friend class CTfWin;
	friend class CTfUi;

};

/*

$Log: not supported by cvs2svn $
Revision 1.3  2008/08/21 14:49:36  leeten

[2008/08/21]
1. [ADD] Define variables fDomainMin/fDomainMax as the support.
2. [ADD] Define methods _GetDomain/_SetDomain to get/set the domain.

Revision 1.2  2008/08/15 14:54:11  leeten

[2008/08/15]
1. [ADD] Declare new methods BOpenFile/BSaveFile to open/save the TF as files.
2. [ADD] Declare a new methods BCheckFile to check the validabilty of a file. It is used to prevent from loading wrong TF and destroying the current TF because calling another method BOpenFile can immediately destroy the TF.
3. [ADD] Define a new method BCheckFilenameExt to check if the file extension is '.TF.'

Revision 1.1.1.1  2008/08/14 14:44:02  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN]. This library defines classes for trasnfer functions, including editing and displaying.


*/
