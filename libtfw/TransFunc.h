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
	void _Load(char szFilename[]);
	void _Save(char szFilename[]);
	void _ExportColorMap(float pfColorMap[], int iNrOfEntries);

	friend class CTfWin;
	friend class CTfUi;

};

/*

$Log: not supported by cvs2svn $

*/
