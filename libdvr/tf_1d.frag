/*

This is the shader program for direct volume rendering

*/

	uniform sampler3D t3dVol;		// the texture hold the depths of each knots
	uniform sampler1D t1dTf;		// the texture hold the depths of each knots

	uniform float fThickness;

	// current display window's res.
	uniform float fWindowWidth;
	uniform float fWindowHeight;

	// range of the data
	uniform float fDataValueMin;
	uniform float fDataValueMax;

	// range of the TF's support 
	uniform float fTfDomainMin;
	uniform float fTfDomainMax;

void 
main()
{
				// read the value 
	float fV = texture3D(t3dVol, gl_TexCoord[0].xyz).x;

				// convert the value into color via the transfer function
	vec4 v4Color;
	fV = fDataValueMin + fV * (fDataValueMax - fDataValueMin);
	fV = (fV - fTfDomainMin) / (fTfDomainMax - fTfDomainMin);
	if( fV < 0.0 || fV > 1.0 )
		v4Color = vec4(0.0);
	else
		v4Color = texture1D(t1dTf, fV);


				// calcualte the distance between this fragment and the previous fragment in the object space 
	vec4 v4PrevCoord_eye = gl_FragCoord;
	v4PrevCoord_eye.x /= fWindowWidth;
	v4PrevCoord_eye.y /= fWindowHeight;
	v4PrevCoord_eye.z += fThickness;

	v4PrevCoord_eye.xyz = v4PrevCoord_eye.xyz * 2.0 - 1.0;
	v4PrevCoord_eye.xyz /= v4PrevCoord_eye.w;
	v4PrevCoord_eye.w = 1.0 / v4PrevCoord_eye.w;

	vec4 v4PrevCoord_obj = gl_ModelViewProjectionMatrixInverse * v4PrevCoord_eye;

	vec4 v4Coord_obj = gl_TexCoord[0];
	v4Coord_obj.xyz = v4Coord_obj.xyz * 2.0 - 1.0; 

	float fThickness_obj = distance(v4Coord_obj, v4PrevCoord_obj);

	v4Color.a = 1.0 - exp(-v4Color.a * fThickness_obj);

	gl_FragColor = v4Color;
}

