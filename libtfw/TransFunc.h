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

#include <stdio.h>
#include <string.h>

#pragma once

	#include "knot.h"

	#include "FloatValue.h"

class CTransFunc {

	CFloatValue cDomainMin, cDomainMax;

public:
	void _SetTfDomain(float fMin, float fMax)
	{
		cDomainMin._SetValue(fMin);
		cDomainMax._SetValue(fMax);
	}

	void _GetTfDomain(float *pfMin, float *pfMax)
	{
		*pfMin = cDomainMin.FGetValue();
		*pfMax = cDomainMax.FGetValue();
	}

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
	CTransFunc();
	~CTransFunc();

public:
	void _LoadRainBow();

										// methods about load/save files

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

	void _ExportColorMap(float pfColorMap[], int iNrOfEntries);

	friend class CTfWin;
	friend class CTfUi;

};

