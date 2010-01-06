/*

Filename: tf1d_frag_func.frag

Introduction: 
	This file defines the functions used to clip fragments against a clip volume. 

Usage:
	Include the file 'tf1d_frag_func.h' in the fragment shader, and assign the following variable in the OpenGL program:
	
		- t1dTf
		- fDataValueMin
		- fDataValueMax
		- fTfDomainMin
		- fTfDomainMax

*/

	uniform sampler1D t1dTf;		// the texture hold the depths of each knots

	uniform float fDataValueMin;
	uniform float fDataValueMax;

	// range of the TF's support 
	uniform float fTfDomainMin;
	uniform float fTfDomainMax;

vec4 F4GetColorFrom1DTf(float fV_normalized)
{
	vec4 v4Color;
	float fV_data = fDataValueMin + fV_normalized * (fDataValueMax - fDataValueMin);
	float fV_tf = (fV_data - fTfDomainMin) / (fTfDomainMax - fTfDomainMin);
	if( fV_tf < 0.0 || fV_tf > 1.0 )
		v4Color = vec4(0.0);
	else
		v4Color = texture1D(t1dTf, fV_tf);
	return v4Color;
}

/*

$Log: not supported by cvs2svn $

*/

