///////////////////////////////////////////////////////////////////////
// headers
	#include <GL/glew.h>

	#include <stdio.h>

	// ADD-By-LEETEN 10/21/2011-BEGIN
	#if		WITH_NRRD
	#include "NrrdIO.h"
	#ifdef WIN32
		#if		defined(_DEBUG)
		#pragma comment (lib, "ITKNrrdIOd.lib")      
		#pragma comment (lib, "zlibd.lib")      // link with my own library libfps
		#else	// #if	defined(_DEBUG)
		#pragma comment (lib, "ITKNrrdIO.lib")     
		#pragma comment (lib, "zlib.lib")      // link with my own library libfps
		#endif	// #if	defined(_DEBUG)
	#endif	// #ifdef	WIN32
	#endif	// #if		WITH_NRRD
	// ADD-By-LEETEN 10/21/2011-END

	
	// my own program argument parser
	#include "libopt.h"

	// my own template library for allocating buffers
	#include "libbuf3d.h"

	#include "DvrSuiteWin.h"

					// the GLUT wins
	CDvrSuiteWin cDvrWin;

/////////////////////////////////////////////////////////
void
quit()
{
}

int
main(int argn, char *argv[])
{
	CGlutWin::_Init(
		&argn, argv, 
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_STENCIL);

	int iNextArg;
	_OPTInit(true);

	bool bIsOptParsed = BOPTParse(argv, argn, 1, &iNextArg);
	assert(bIsOptParsed);

	atexit(quit);

	// load the transfer func. and data set
										// create the DVR window
	cDvrWin._SetHistogramSize(32);			// optional: Call it if you want to change the resolution of the histogram
	cDvrWin._SetTfSize(256);					// optional: Call it if you want to change the resolution of the transfer function
	cDvrWin._LoadData(argv[1]);
	cDvrWin.ICreate("Direct Volume Rendering"); 
	cDvrWin._CreateTfWins();				// optional: Call it if you want to edit/view the transfer function
		
	glutMainLoop();

	return 0;
}

