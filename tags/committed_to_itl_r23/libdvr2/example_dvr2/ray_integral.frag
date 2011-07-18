/*

This is the shader program for direct volume rendering

*/
	uniform sampler3D t3dVolume;	// the texture hold the depths of each knots
	uniform sampler2D t2dPrevLayer;	// the texture hold the depths of each knots
	// MOD-BY-LEETEN 09/10/2010-FROM:
		// uniform sampler2DShadow t2dsDepth;	// the texture hold the depths of each knots
	// TO:
	uniform sampler2D t2dsDepth;	// the texture hold the depths of each knots
	// MOD-BY-LEETEN 09/10/2010-END
	uniform float fThicknessGain;
	uniform float fWindowWidth;
	uniform float fWindowHeight;

	#include "clip_frag_func.frag.h"
	#include "tf1d_frag_func.frag.h"

void 
main()
{
	vec4 v4FragCoord = gl_FragCoord;
	v4FragCoord.x /= fWindowWidth;
	v4FragCoord.y /= fWindowHeight;
	v4FragCoord.w = 1.0 / v4FragCoord.w;

				// read the previous layer
	vec4 v4PrevFragData = texture2D(t2dPrevLayer, v4FragCoord.xy);
	float fPrevDepth = v4PrevFragData.r;
	float fPrevV_normalized = v4PrevFragData.g;

						// calcualte the distance between this fragment and the previous fragment in the object space 
	vec4 v4PrevCoord_ndc = v4FragCoord;
	v4PrevCoord_ndc.z = fPrevDepth;
	v4PrevCoord_ndc.xyz = v4PrevCoord_ndc.xyz * 2.0 - 1.0;
	v4PrevCoord_ndc.w = 1.0;
	vec4 v4PrevCoord_obj = gl_ModelViewProjectionMatrixInverse * v4PrevCoord_ndc;
	v4PrevCoord_obj = v4PrevCoord_obj / v4PrevCoord_obj.w;
	vec4 v4PrevCoord_eye = gl_ModelViewMatrix * v4PrevCoord_obj;

	vec4 v4Coord_ndc = v4FragCoord;
	v4Coord_ndc.xyz = v4Coord_ndc.xyz * 2.0 - 1.0; 
	v4Coord_ndc.w = 1.0;
	vec4 v4Coord_obj = gl_ModelViewProjectionMatrixInverse * v4Coord_ndc;
	v4Coord_obj = v4Coord_obj / v4Coord_obj.w;
	vec4 v4Coord_eye = gl_ModelViewMatrix * v4Coord_obj;

	float fThickness_obj;
	fThickness_obj = distance(v4Coord_obj, v4PrevCoord_obj);

	fThickness_obj *= fThicknessGain;

	float fThicknessRatio = FAdjustThickness(v4FragCoord.z, fPrevDepth, v4FragCoord.xy);
	fThickness_obj *= fThicknessRatio;

				// read the value 
	float fV_normalized = texture3D(t3dVolume, gl_TexCoord[0].xyz).x;

				// convert the value into color via the transfer function
	vec4 v4Color = F4GetColorFrom1DTf(fV_normalized);

	// MOD-BY-LEETEN 05/13/2010-FROM:
		// v4Color.a = 1.0 - pow(1.0 - v4Color.a, fThickness_obj);
	// TO:
	v4Color.a = 1.0 - exp(-v4Color.a * fThickness_obj);
	// MOD-BY-LEETEN 05/13/2010-END

	// MOD-BY-LEETEN 09/10/2010-FROM:
		// float fBackgroundDepth = shadow2D(t2dsDepth, v4FragCoord.xyz).r;
	// TO:
	float fBackgroundDepth = texture2D(t2dsDepth, v4FragCoord.xy).r;
	// MOD-BY-LEETEN 09/10/2010-END
	if( v4FragCoord.z > fBackgroundDepth )
		v4Color.a = 0.0;

	vec4 v4Data = vec4(
		v4FragCoord.z, 
		fV_normalized,
		0.0, 
		1.0);

	gl_FragData[0] = v4Color;
	gl_FragData[1] = v4Data;
}

/*

$Log: not supported by cvs2svn $
Revision 1.3  2010/09/10 14:08:40  leeten

[09/10/2010]
1. [MOD] Treat the sampler t2dsDepth as a regular 2D texture.

Revision 1.2  2010/05/15 16:06:23  leeten

[05/15/2010]
1. [MOD] Change the ray integral equation from A = 1 - (1 - A)^D to A = 1 - exp(-AD).

Revision 1.1  2010/04/06 20:01:15  leeten

[04/06/2010]
1. [1ST] First time checkin.


*/
