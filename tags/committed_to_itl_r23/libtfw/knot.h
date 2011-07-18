/***********************************************

knot: 

This file defines a class of knots for splines. 

************************************************/

#pragma once

struct CKnot {
	float fX;
	float fY;
	CKnot()	
	{
		fX = 0.0f;	fY = 0.0f;
	};
	CKnot(float x, float y)	
	{ 
		fX = x;		fY = y;	
	}
};

/*

$Log: not supported by cvs2svn $

*/
