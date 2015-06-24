	#include "shader.h"
	#include "ClipVolume.h"

void 
CClipVolume::_SetClipPlanes(int iNrOfClipPlanes, double* pdClipPlanes)
{
	this->iNrOfClipPlanes = iNrOfClipPlanes;

	for(int cp = 0; cp < iNrOfClipPlanes; cp++)
		memcpy(this->ppdClipPlanes[cp], &pdClipPlanes[4*cp], sizeof(this->ppdClipPlanes[0]));
}

void 
CClipVolume::_InitFunc()
{
	cTexture._Create();
	glGenFramebuffersEXT(1, &fid);	// allocate framebuffer object

	pidClipVolume = CSetShadersByString(
		NULL,
		#include "clipvolume.frag.h"	
	);
	assert( pidClipVolume );	// Program Handle for the fragment shader to generate LDI
}

void 
CClipVolume::_ReshapeFunc(int w, int h)
{
	if( w > 0 && h > 0 )
	{
		cTexture._SetResolution(w, h);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fid);
		glFramebufferTexture2DEXT(
			GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			cTexture.eTarget, cTexture.t2d, 0);
		unsigned int iFboStatus = EGetFboStatus(true);
		assert( GL_FRAMEBUFFER_COMPLETE_EXT == iFboStatus );

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

void 
CClipVolume::_Create()
{
	glPushAttrib(GL_CURRENT_BIT);

						// specify the clip planes
	for(int cp = 0; cp < iNrOfClipPlanes; cp++)
		glClipPlane(GL_CLIP_PLANE0 + cp, ppdClipPlanes[cp]);

	static double pdModelview[16];
	static double pdProjection[16];
	static int piViewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, pdModelview);
	glGetDoublev(GL_PROJECTION_MATRIX, pdProjection);
	glGetIntegerv(GL_VIEWPORT, piViewport);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fid);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgramObjectARB(pidClipVolume);

	SET_1F_VALUE_BY_NAME(pidClipVolume, "fWindowWidth", (float)piViewport[2]);
	SET_1F_VALUE_BY_NAME(pidClipVolume, "fWindowHeight", (float)piViewport[3]);
	SET_1I_VALUE_BY_NAME(pidClipVolume, "iNrOfClipPlanes", iNrOfClipPlanes);

	double dX, dY, dZ;
	double dW = (double)piViewport[2];
	double dH = (double)piViewport[3];
	glBegin(GL_QUADS);
		gluUnProject(0.0, 0.0, 0.5, pdModelview, pdProjection, piViewport, &dX, &dY, &dZ);	glVertex3d(dX, dY, dZ);
		gluUnProject(dW,  0.0, 0.5, pdModelview, pdProjection, piViewport, &dX, &dY, &dZ);	glVertex3d(dX, dY, dZ);
		gluUnProject(dW,  dH,  0.5, pdModelview, pdProjection, piViewport, &dX, &dY, &dZ);	glVertex3d(dX, dY, dZ);
		gluUnProject(0.0, dH,  0.5, pdModelview, pdProjection, piViewport, &dX, &dY, &dZ);	glVertex3d(dX, dY, dZ);
	glEnd();

//	glUseProgramObjectARB(0);
	glPopAttrib();	// glPushAttrib(GL_CURRENT_BIT);
}

CClipVolume::CClipVolume(void)
{
	FILE *fpFragShader;
	fpFragShader = fopen("clip_frag_func.frag.h", "wt");
	assert(fpFragShader);
	fputs(
		#include "clip_frag_func.frag.h"
		,
		fpFragShader);
	fclose(fpFragShader);
}

CClipVolume::~CClipVolume(void)
{
}

