/*

I collect the callbacks for GLUT into a library called GCB(GLUT Callbacks) in order to resue those callbacks. 

*/

#ifndef __GLUT_CALLBACK_H__
#define __GLUT_CALLBACK_H__

// ADD-BY-LEETEN 03/10/2008-BEGIN
#pragma comment (lib, "winmm.lib")      /* link with Windows MultiMedia lib */

#if USING_FREEGLUT

#include <GL/freeglut.h>
#pragma comment (lib, "freeglut.lib")

#else

// ADD-BY-LEETEN 03/10/2008-END

#include <GL/glut.h>

// ADD-BY-LEETEN 03/10/2008-BEGIN
#pragma comment (lib, "glut32.lib")   
#endif
// ADD-BY-LEETEN 03/10/2008-END

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include <math.h>

#if 0	// MOD-BY-LEETEN 03/05/2008-FROM:

#ifndef M_PI_2

#define M_PI_2 (3.14159 / 2.0)

#endif

#else	// MOD-BY-LEETEN 03/05/2008-TO:

#ifndef M_PI

#define	M_PI	3.14159
#define M_PI_2 (M_PI / 2.0)

#endif

#endif 	// MOD-BY-LEETEN 03/05/2008-END

#ifdef NDEBUG
	#undef assert
	#define assert(expr) \
	{\
		if( !(expr) ) \
		{	\
		fprintf(stderr, "%s in %s (%d):", #expr, __FILE__, __LINE__);\
			perror("");	\
			exit(-1);	\
		}\
	}\
			
#else
	#include <assert.h>
#endif

typedef GLdouble TMatrix[16];
extern TMatrix tViewMatrix;
extern TMatrix tModelMatrix;
extern TMatrix tProjectionMatrix;
extern int piViewport[4];

void gcbDisplayFunc(void (*_MyDisplayFunc)());
void gcbReshapeFunc(void (*_MyReshapeFunc)(int, int));
void gcbKeyboardFunc(void (*_MyKeyboardFunc)(unsigned char, int, int));
void gcbSpecialFunc(void (*_MySpecialFunc)(int, int, int));
void gcbIdleFunc(void (*_MyIdleFunc)());

// ADD-BY-LEETEN 03/12/2008-BEGIN
void gcbSetSnapshotFilename(char* szSnapshotFilename, bool bSnapshotThenExit = false);
// ADD-BY-LEETEN 03/12/2008-END

// ADD-BY-LEETEN 03/31/2008-BEGIN
void gcbAnimeOn();
void gcbAnimeNrOfFrames(	size_t _uNrOfFrames);
void gcbAnimeSaveFrames();
void gcbMatrixRecord(	char _szMatrixFilename[]);
void gcbMatrixPlay(		char _szMatrixFilename[]);
// ADD-BY-LEETEN 03/31/2008-END

void gcbInit(void (*_InitFunc)() = NULL, void (*_QuitFunc)() = NULL);

#endif	// __GLUT_CALLBACK_H__

/*

$Log: not supported by cvs2svn $
Revision 1.7  2008/04/02 19:25:26  leeten

[04/02/2008]
1. Remove the function gcbEnableAnime(); instead, it is replaced by a set of functions to control the animation:
  a. gcbAnimeOn(): make the animation is automatically played
  b. gcbAnimeNrOfFrames(_uNrOfFrames): decide the #frames; after the number is reached, the program terminate itself.
  c. gcbAnimeSaveFrames(): enable that all frames are saved.
  d. gcbMatrixRecord(_szMatrixFilename): specify the filename to which the matrices of all frames should be saved.
  e. gcbMatrixPlay(_szMatrixFilename): specify the filename from which the matrices of all frames should be read.

Revision 1.6  2008/03/30 21:28:24  leeten

[03/30/2008]
1. Declare gcbEnableAnime to enable the animation mode.

Revision 1.5  2008/03/23 19:50:00  leeten

[03/23/2008]
1. Print out the expresion in my defined macro 'assert'.

Revision 1.4  2008/03/14 22:57:49  leeten

[03/14/2008]
1. Declare function gcbSetSnapshotFilename() to specify the filename of the snapshot and if the program should be terminated afer the first snapshot is taken.
2. Define the assert macro.

Revision 1.3  2008/03/10 21:28:44  leeten

[03/10/2008]
1. Include FREEGLUT other than GLUT if the preprocessor USING_FREEGLUT is given.
2. Include the GLUT or FREEGLUT libraries in this file.

Revision 1.2  2008/03/06 22:01:25  leeten

[03/06/2008]
1. Define M_PI.

Revision 1.1.1.1  2008/02/23 03:05:24  leeten

[02/22/2008]
1. First time checkin.


*/
