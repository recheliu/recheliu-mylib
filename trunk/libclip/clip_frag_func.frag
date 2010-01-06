/*

Filename: clip_frag_func.frag

Introduction: 
	This file defines the functions used to clip fragments against a clip volume. 

Usage:
	Include the file clip_frag_func.frag.h in the fragment shader, and assign the following variable in the OpenGL program:
	
		- t2dClipVolume

*/

uniform sampler2D t2dClipVolume;

float FGetZNear(vec2 v2FragCoord)
{
	return texture2D(t2dClipVolume, v2FragCoord).r;
}

float FGetZFar(vec2 v2FragCoord)
{
	return texture2D(t2dClipVolume, v2FragCoord).g;
}

bool 
BIsInClipvolume
(
	float fDepth, 
	vec2 v2FragCoord
)
{
	vec4 v4ClipVolume = texture2D(t2dClipVolume, v2FragCoord);
	float fZ_near = v4ClipVolume.r;
	float fZ_far = v4ClipVolume.g;
	return ( fZ_near < fDepth	&& fDepth	< fZ_far )?true:false;
}

// note: this function assume that fDepth is smaller than fPrevDepth
float 
FAdjustThickness
(
	float fDepth, 
	float fPrevDepth, 
	vec2 v2FragCoord
)
{
	vec4 v4ClipVolume = texture2D(t2dClipVolume, v2FragCoord);
	float fZ_near = v4ClipVolume.r;
	float fZ_far = v4ClipVolume.g;
	float fThicknessRatio = 0.0;
	bool bPassClipPlanes =		( fZ_near <= fDepth		&&	fDepth		<= fZ_far )?true:false;
	bool bPrevPassClipPlanes =	( fZ_near <= fPrevDepth	&&	fPrevDepth	<= fZ_far )?true:false;

	if( 0.0 == fPrevDepth )
	{
		// if the prev. depth is zero, which means that there is no previous layer exists. In this case, the thickness is reduced to zero.
		fThicknessRatio = 0.0;
	}
	else
	{
		if( false == bPrevPassClipPlanes	&& false == bPassClipPlanes )
			fThicknessRatio = 0.0;
		else
		if( true == bPrevPassClipPlanes		&& true	== bPassClipPlanes )
			fThicknessRatio = 1.0;
		else
		if( false == bPrevPassClipPlanes	&& true == bPassClipPlanes )
			fThicknessRatio = (fZ_far - fDepth)/(fPrevDepth - fDepth);
		else
		if( true == bPrevPassClipPlanes		&& false == bPassClipPlanes )
			fThicknessRatio = (fPrevDepth - fZ_near)/(fPrevDepth - fDepth);
	}
	return fThicknessRatio;
}

/*

$Log: not supported by cvs2svn $

*/

