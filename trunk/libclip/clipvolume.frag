/*

This is the shader program for constructing the clipping volume, which is the region bounded by the clipping planes

*/

	// current display window's res.
	uniform float fWindowWidth;
	uniform float fWindowHeight;

	// # clipping planes
	uniform int iNrOfClipPlanes;

void 
main()
{
	vec2 v2FragCoord2D_ndc = vec2(
		gl_FragCoord.x/fWindowWidth, 
		gl_FragCoord.y/fWindowHeight) * 2.0 - 1.0;

	// calculate the obj/eye coordinate of the nearest fragment
	vec4 v4FragNearCoord_obj = gl_ModelViewProjectionMatrixInverse * vec4(v2FragCoord2D_ndc, -1.0, 1.0);
	v4FragNearCoord_obj /= v4FragNearCoord_obj.w;
	vec4 v4FragNearCoord_eye = gl_ModelViewMatrix * v4FragNearCoord_obj;

	// calculate the obj/eye coordinate of the furthest fragment
	vec4 v4FragFarCoord_obj = gl_ModelViewProjectionMatrixInverse *  vec4(v2FragCoord2D_ndc, +1.0, 1.0);
	v4FragFarCoord_obj /= v4FragFarCoord_obj.w;
	vec4 v4FragFarCoord_eye = gl_ModelViewMatrix * v4FragFarCoord_obj;

	// the 2 variables [fT_near, fT_far] defined the range of T to boudn the clipping volume along this viewing ray
	float fT_far = 1.0;
	float fT_near = 0.0;

	for(int cp = 0; cp < iNrOfClipPlanes; cp++)
	{
		float fC_near	= dot(v4FragNearCoord_eye, gl_ClipPlane[cp]);
		float fC_far	= dot(v4FragFarCoord_eye,  gl_ClipPlane[cp]);

		// if both the near and the far fragments are clipped away, there is no clipping volume along this viewing ray
		if( fC_near < 0.0 && fC_far < 0.0 )
		{
			fT_far = 0.0;
			fT_near = 1.0;
			break;
		}

		// if both the near and the far fragments are in the clipping volume of this clipping plane, do nothing
		if( fC_near > 0.0 && fC_far > 0.0 )
			continue;

		if( fC_near != fC_far )
		{
			float fT = fC_near / (fC_near - fC_far);
			if( 0.0 < fT && fT < 1.0 )
			{
				if( fC_near > 0.0 )
					fT_far = min(fT_far, fT);
				else
				if( fC_near < 0.0 )
					fT_near = max(fT_near, fT);
			}
		}
	}

						// convert the t values to z values in the window coordinate
	vec4 v4FragNearCoord_ndc = gl_ProjectionMatrix * (v4FragNearCoord_eye + (v4FragFarCoord_eye - v4FragNearCoord_eye) * fT_near);
	v4FragNearCoord_ndc /= v4FragNearCoord_ndc.w;

	vec4 v4FragFarCoord_ndc = gl_ProjectionMatrix * (v4FragNearCoord_eye + (v4FragFarCoord_eye - v4FragNearCoord_eye) * fT_far);
	v4FragFarCoord_ndc /= v4FragFarCoord_ndc.w;

						// store the 2 z values
	gl_FragData[0] = vec4(
		(v4FragNearCoord_ndc.z + 1.0) / 2.0, 
		(v4FragFarCoord_ndc.z + 1.0) / 2.0, 
		0.0, 
		0.0);
}

/*

$Log: not supported by cvs2svn $

*/
