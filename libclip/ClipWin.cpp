#include "ClipWin.h"

void 
CClipWin::_SetBBox(
	float fX0, float fY0, float fZ0,
	float fX1, float fY1, float fZ1)
{
	cBBox.f3LowerCorner.x = fX0;
	cBBox.f3LowerCorner.y = fY0;
	cBBox.f3LowerCorner.z = fZ0;
	cBBox.f3UpperCorner.x = fX1;
	cBBox.f3UpperCorner.y = fY1;
	cBBox.f3UpperCorner.z = fZ1;
}

void
CClipWin::_DisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// plot the bounding box
	glPushMatrix();
	glTranslatef(
		cBBox.f3LowerCorner.x,
		cBBox.f3LowerCorner.y,
		cBBox.f3LowerCorner.z);
	glScalef(
		(cBBox.f3UpperCorner.x - cBBox.f3LowerCorner.x)/2.0f,
		(cBBox.f3UpperCorner.y - cBBox.f3LowerCorner.y)/2.0f,
		(cBBox.f3UpperCorner.z - cBBox.f3LowerCorner.z)/2.0f);
	glTranslatef(1.0f, 1.0f, 1.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutWireCube(2.0f);
	glPopMatrix();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

								// step-2:
								// plot the clipping planes
	if( ibPlotClippingPlanes )
	{
		glPushAttrib(GL_POLYGON_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#if	1	
		glBegin(GL_QUADS);
		for(int cp = 0; cp < CClipPlanes::NR_OF_CLIP_PLANES; cp++) 
		{
			if( cClipPlanes.piFlags[cp] )
			{
				// find the primal direction
				int iPrimalDir = 0;
				for(int j = 1; j < 3; j++)
					if( fabs(cClipPlanes.ppdPlanes[cp][iPrimalDir]) < fabs(cClipPlanes.ppdPlanes[cp][j]) )
						iPrimalDir = j;

				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				switch(iPrimalDir)
				{
				case 0:
					{
					float fX;
					fX = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][1] * cBBox.f3LowerCorner.y + cClipPlanes.ppdPlanes[cp][2] * cBBox.f3LowerCorner.z)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(fX, cBBox.f3LowerCorner.y, cBBox.f3LowerCorner.z);

					fX = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][1] * cBBox.f3UpperCorner.y + cClipPlanes.ppdPlanes[cp][2] * cBBox.f3LowerCorner.z)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(fX, cBBox.f3UpperCorner.y, cBBox.f3LowerCorner.z);

					fX = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][1] * cBBox.f3UpperCorner.y + cClipPlanes.ppdPlanes[cp][2] * cBBox.f3UpperCorner.z)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(fX, cBBox.f3UpperCorner.y, cBBox.f3UpperCorner.z);

					fX = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][1] * cBBox.f3LowerCorner.y + cClipPlanes.ppdPlanes[cp][2] * cBBox.f3UpperCorner.z)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(fX, cBBox.f3LowerCorner.y, cBBox.f3UpperCorner.z);
					}

					break;

				case 1:
					{
					float fY;
					fY = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][0] * cBBox.f3LowerCorner.x + cClipPlanes.ppdPlanes[cp][2] * cBBox.f3LowerCorner.z)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(cBBox.f3LowerCorner.x, fY, cBBox.f3LowerCorner.z);

					fY = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][0] * cBBox.f3UpperCorner.x + cClipPlanes.ppdPlanes[cp][2] * cBBox.f3LowerCorner.z)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(cBBox.f3UpperCorner.x, fY, cBBox.f3LowerCorner.z);

					fY = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][0] * cBBox.f3UpperCorner.x + cClipPlanes.ppdPlanes[cp][2] * cBBox.f3UpperCorner.z)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(cBBox.f3UpperCorner.x, fY, cBBox.f3UpperCorner.z);

					fY = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][0] * cBBox.f3LowerCorner.x + cClipPlanes.ppdPlanes[cp][2] * cBBox.f3UpperCorner.z)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(cBBox.f3LowerCorner.x, fY, cBBox.f3UpperCorner.z);
					}

					break;

				case 2:
					{
					float fZ;
					fZ = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][0] * cBBox.f3LowerCorner.x + cClipPlanes.ppdPlanes[cp][1] * cBBox.f3LowerCorner.y)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(cBBox.f3LowerCorner.x, cBBox.f3LowerCorner.y, fZ);

					fZ = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][0] * cBBox.f3UpperCorner.x + cClipPlanes.ppdPlanes[cp][1] * cBBox.f3LowerCorner.y)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(cBBox.f3UpperCorner.x, cBBox.f3LowerCorner.y, fZ);

					fZ = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][0] * cBBox.f3UpperCorner.x + cClipPlanes.ppdPlanes[cp][1] * cBBox.f3UpperCorner.y)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(cBBox.f3UpperCorner.x, cBBox.f3UpperCorner.y, fZ);

					fZ = -(cClipPlanes.ppdPlanes[cp][3] + cClipPlanes.ppdPlanes[cp][0] * cBBox.f3LowerCorner.x + cClipPlanes.ppdPlanes[cp][1] * cBBox.f3UpperCorner.y)/cClipPlanes.ppdPlanes[cp][iPrimalDir];
					glVertex3f(cBBox.f3LowerCorner.x, cBBox.f3UpperCorner.y, fZ);
					}

					break;
				}
			}
		}
#else
		glBegin(GL_TRIANGLES);
		for(int cp = 0; cp < CClipPlanes::NR_OF_CLIP_PLANES; cp++) 
		{
			if( cClipPlanes.piFlags[cp] )
			{
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

				float pfVertex[3];
				for(int a = 0; a < 3; a++)
				{
					memset(pfVertex, 0, sizeof(pfVertex));
					if( cClipPlanes.ppdPlanes[cp][a] != 0.0 )
					{
						pfVertex[a] = -(cClipPlanes.ppdPlanes[cp][3])/cClipPlanes.ppdPlanes[cp][a];
					}
					glVertex3fv(pfVertex);
				}
			}
		}
#endif
		glEnd();
		glPopAttrib();	// glPushAttrib(GL_POLYGON_BIT);
	}	//	if( ibPlotClippingPlanes )

								// step-3:
								// plot the interior volume
	if( cInteriorVolume.ibIsEnabled )
	{
		
									// step-1:
									// setup clipping planes
		// ADD-BY-LEETEN 2009/11/04-BEGIN
		if( !ibDisableAll )
		// ADD-BY-LEETEN 2009/11/04-END

		for(int i=0; i < CClipPlanes::NR_OF_CLIP_PLANES; i++) 
		{
			if( cClipPlanes.piFlags[i] )
			{
				glEnable(GL_CLIP_PLANE0 + i);
				glClipPlane(GL_CLIP_PLANE0 + i, cClipPlanes.ppdPlanes[i]);
			}
		}

		glEnable( GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		int iNrOfSlices = 16;

		glColor4f(1.0f, 1.0f, 1.0f, cInteriorVolume.fOpacity / (3.0f * (float)iNrOfSlices));

		glPushMatrix();
		glTranslatef(
			cBBox.f3LowerCorner.x,
			cBBox.f3LowerCorner.y,
			cBBox.f3LowerCorner.z);
		glScalef(
			(cBBox.f3UpperCorner.x - cBBox.f3LowerCorner.x)/2.0f,
			(cBBox.f3UpperCorner.y - cBBox.f3LowerCorner.y)/2.0f,
			(cBBox.f3UpperCorner.z - cBBox.f3LowerCorner.z)/2.0f);
		glTranslatef(1.0f, 1.0f, 1.0f);

		static GLuint lidInteriorVolume;

		if( !lidInteriorVolume )
		{
			lidInteriorVolume = glGenLists(1);
			glNewList(lidInteriorVolume, GL_COMPILE_AND_EXECUTE);
			for(int i = 0; i < 3; i++)
			{
				glPushMatrix();
				switch(i)
				{
				case 0:		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);	break;
				case 1:		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);	break;
				}
				glBegin(GL_QUADS);
				for(int z=0; z <= iNrOfSlices; z++)
				{
					double dZ = 2.0 * (double)z / (double)iNrOfSlices - 1.0; 

					glVertex3d(-1, -1, dZ);
					glVertex3d(+1, -1, dZ);
					glVertex3d(+1, +1, dZ);
					glVertex3d(-1, +1, dZ);
				}
				glEnd();
				glPopMatrix();
			}
			glEndList();
		}
		else
		{
			glCallList(lidInteriorVolume);
		}
		glPopMatrix();
	}	// if( ibPlotInteriorVolume )

	glPopAttrib();	// glPushAttrib(GL_ENABLE_BIT);	

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(1, 0, 0);
	glEnd();
	_DrawString3D("X", 1.0f, 0.0f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 1, 0);
	glEnd();
	_DrawString3D("Y", 0.0f, 1.0f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 1);
	glEnd();
	_DrawString3D("Z", 0.0f, 0.0f, 1.0f);
}

void
CClipWin::_InitFunc()
{
		// keep updating the display window to reflect the modification from the window CClipEditor
	_KeepUpdateOn();

	PCGetGluiSubwin()->add_checkbox("Plot Clipping Planes?", &ibPlotClippingPlanes);
	PCGetGluiSubwin()->add_checkbox("Keep Update?", &ibKeepUpdate);
	// MOD-BY-LEETEN 2009/11/04-FROM:
		// PCGetGluiSubwin()->add_checkbox("Keep Update?", &ibDisableAll);
	// TO:
	PCGetGluiSubwin()->add_checkbox("Disable All?", &ibDisableAll);
	// MOD-BY-LEETEN 2009/11/04-END
	PCGetGluiSubwin()->add_button("Update", IAddWid(EVENT_BUTTON_UPDATE), &CGlutWin::_GluiCB_static);

	PCGetGluiSubwin()->add_column();
	cInteriorVolume._AddGlui(PCGetGluiSubwin());

	// ADD-BY-LEETEN 2009/11/04-BEGIN
	this->_DisableVerticalSync();
	// ADD-BY-LEETEN 2009/11/04-END

	cClipEditor._SetClipPlanes(&cClipPlanes);
	cClipEditor.ICreate("Clip Plane Editor", false, 100, 100, 256, 384);
	// ADD-BY-LEETEN 2009/11/04-BEGIN
	cClipEditor._DisableVerticalSync();
	// ADD-BY-LEETEN 2009/11/04-END


	_Set();

	_Reshape(384, 384);
	
}

void
CClipWin::_PassEnabledClipPlanes()
{
	static double ppdEnalbedClipPlanes[CClipPlanes::NR_OF_CLIP_PLANES][4];
	int iNrOfEnabledClipPlanes = 0;

	// ADD-BY-LEETEN 2009/11/04-BEGIN
	if( !ibDisableAll )
	// ADD-BY-LEETEN 2009/11/04-END

		for(int cp = 0; cp < CClipPlanes::NR_OF_CLIP_PLANES; cp++)
			if( cClipPlanes.piFlags[cp] )
			{
				memcpy(ppdEnalbedClipPlanes[iNrOfEnabledClipPlanes], cClipPlanes.ppdPlanes[cp], sizeof(ppdEnalbedClipPlanes[0]));
				iNrOfEnabledClipPlanes++;
			}

	CGlutWin::_GlobalCB(
		IGetId(), 
		CGlutWin::CB_MANUAL, 
		EVENT_BUTTON_UPDATE, 
		iNrOfEnabledClipPlanes,		// #enabled clipping planes
		(double**)ppdEnalbedClipPlanes);		// pointer to the array of enabled clipping planes
}

void 
CClipWin::_IdleFunc()
{
	if( ibKeepUpdate )
		_PassEnabledClipPlanes();
}


void 
CClipWin::_GluiFunc(unsigned short usValue)
{
	switch(usValue)
	{
	case EVENT_BUTTON_UPDATE:
		_PassEnabledClipPlanes();
		break;
	}
}

CClipWin::CClipWin(void)
{
	_AddGluiSubwin(GLUI_SUBWINDOW_TOP);

	cBBox.f3LowerCorner.x = -1.0f;
	cBBox.f3LowerCorner.y = -1.0f;
	cBBox.f3LowerCorner.z = -1.0f;
	cBBox.f3UpperCorner.x =  1.0f;
	cBBox.f3UpperCorner.y =  1.0f;
	cBBox.f3UpperCorner.z =  1.0f;
}

CClipWin::~CClipWin(void)
{
}

/*

$Log: not supported by cvs2svn $
Revision 1.1  2009/08/26 16:01:12  leeten

[2009/08/26]
1. [1ST] First time checkin.


*/
