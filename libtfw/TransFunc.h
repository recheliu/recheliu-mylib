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

class CTransFunc {
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
Revision 1.1.1.1  2008/08/14 14:44:02  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN]. This library defines classes for trasnfer functions, including editing and displaying.


*/
