#pragma once

#include <math.h>

class CFloatValue
{
public:
	float fExponent;
	float fSignificand;

	void _SetValue(float fValue)
	{
		if( fabsf(fValue) > 0.0f )
		{
			float fSign = (fValue>=0.0f)?(1.0f):(-1.0f);
			float fCommonLogarithm = log10f(fabsf(fValue));
			fExponent = floorf(fCommonLogarithm);
			fSignificand = fSign * powf(10.0f, fCommonLogarithm - fExponent);
		}
		else
		{
			fExponent = 1.0f;
			fSignificand = 0.0f;
		}
	}

	float FGetValue()
	{
		return fSignificand * powf(10.0f, fExponent);
	}

	CFloatValue(void)
	{
		fExponent = 0.0f;
		fSignificand = 0.0f;
	}

	virtual ~CFloatValue(void)
	{
	}
};
