// DEL-BY-LEETEN 02/04/2013:	#include "lib3ds/vector.h"

#include "ClipWin.h"

#include <glm/glm.hpp>	// ADD-BY-LEETEN 02/04/2013

#include <iostream>
#include "liblog.h"

void 
CClipWin::CClipEditor::_DisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glLoadMatrixd(this->tViewMatrix);

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

	glPopMatrix();

	glPushAttrib(GL_ENABLE_BIT);
	glPushAttrib(GL_POLYGON_BIT);

	#if	0	// MOD-BY-LEETEN 02/04/2013-FROM:
	Lib3dsVector v3Normal;
	v3Normal[0] = f4ClipPlane.x;
	v3Normal[1] = f4ClipPlane.y;
	v3Normal[2] = f4ClipPlane.z;
	float fNormalLength = lib3ds_vector_length(v3Normal);
	#else	// MOD-BY-LEETEN 02/04/2013-TO:
	glm::vec3 v3Normal = glm::vec3(f4ClipPlane.x, f4ClipPlane.y, f4ClipPlane.z);
	float fNormalLength = glm::length(v3Normal);
	#endif	// MOD-BY-LEETEN 02/04/2013-END

	if( 0.0f < fNormalLength )
	{
		glPolygonMode(GL_BACK, GL_LINE);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2i(-1, -1);
		glVertex2i(+1, -1);
		glVertex2i(+1, +1);
		glVertex2i(-1, +1);
		glEnd();

		glLineWidth(1.0f);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, fNormalLength);
		glEnd();
	}
	glPopAttrib();	//	glPushAttrib(GL_ENABLE_BIT);
	glPopAttrib();	//	glPushAttrib(GL_POLYGON_BIT);
}

void 
CClipWin::CClipEditor::_InitFunc()
{
	assert(pcClipPlanes);

	_KeepUpdateOn();

	iFlag = pcClipPlanes->piFlags[iActiveClipPlaneId];
	f4ClipPlane.x = pcClipPlanes->ppdPlanes[iActiveClipPlaneId][0];
	f4ClipPlane.y = pcClipPlanes->ppdPlanes[iActiveClipPlaneId][1];
	f4ClipPlane.z = pcClipPlanes->ppdPlanes[iActiveClipPlaneId][2];
	f4ClipPlane.w = pcClipPlanes->ppdPlanes[iActiveClipPlaneId][3];

	GLUI_Listbox *pcListbox;
	pcListbox = PCGetGluiSubwin()->add_listbox("Plane",		&iActiveClipPlaneId,		IAddWid(EVENT_LIST_CLIP_PLANE), &CGlutWin::_GluiCB_static);
	for(int i = 0; i < CClipPlanes::NR_OF_CLIP_PLANES; i++)
		pcListbox->add_item(i, SZSprintf("%d", i));

	PCGetGluiSubwin()->add_checkbox("Enabled?", &iFlag, IAddWid(EVENT_CHECKBOX_ENABLE), &CGlutWin::_GluiCB_static);
	PCGetGluiSubwin()->add_column();

	PCGetGluiSubwin()->add_spinner("X", GLUI_SPINNER_FLOAT, &f4ClipPlane.x,	IAddWid(EVENT_SPINNER_PLANE_X), &CGlutWin::_GluiCB_static);
	PCGetGluiSubwin()->add_spinner("Y", GLUI_SPINNER_FLOAT, &f4ClipPlane.y,	IAddWid(EVENT_SPINNER_PLANE_Y), &CGlutWin::_GluiCB_static);
	PCGetGluiSubwin()->add_spinner("Z", GLUI_SPINNER_FLOAT, &f4ClipPlane.z,	IAddWid(EVENT_SPINNER_PLANE_Z), &CGlutWin::_GluiCB_static);
	PCGetGluiSubwin()->add_spinner("W", GLUI_SPINNER_FLOAT, &f4ClipPlane.w,	IAddWid(EVENT_SPINNER_PLANE_W), &CGlutWin::_GluiCB_static);

	_Reshape(384, 384);
}

void 
CClipWin::CClipEditor::_GluiFunc(unsigned short usValue)
{
	switch(usValue)
	{
	case EVENT_LIST_CLIP_PLANE:
		memcpy(tModelMatrix, pmMatrices[iActiveClipPlaneId], sizeof(tModelMatrix));

		iFlag = pcClipPlanes->piFlags[iActiveClipPlaneId];
		f4ClipPlane.x = pcClipPlanes->ppdPlanes[iActiveClipPlaneId][0];
		f4ClipPlane.y = pcClipPlanes->ppdPlanes[iActiveClipPlaneId][1];
		f4ClipPlane.z = pcClipPlanes->ppdPlanes[iActiveClipPlaneId][2];
		f4ClipPlane.w = pcClipPlanes->ppdPlanes[iActiveClipPlaneId][3];
		break;

	case EVENT_SPINNER_PLANE_X:
	case EVENT_SPINNER_PLANE_Y:		
	case EVENT_SPINNER_PLANE_Z:
		{
			#if	0	// MOD-BY-LEETEN 02/04/2013-FROM:
			Lib3dsVector v3Z;
			v3Z[0] = 0.0f; 
			v3Z[1] = 0.0f; 
			v3Z[2] = 1.0f;
		
			Lib3dsVector v3Normal;
			v3Normal[0] = f4ClipPlane.x;
			v3Normal[1] = f4ClipPlane.y;
			v3Normal[2] = f4ClipPlane.z;
			lib3ds_vector_normalize(v3Normal);

			Lib3dsVector v3Up;
			lib3ds_vector_cross(v3Up, v3Z, v3Normal);
			lib3ds_vector_normalize(v3Up);

			float fCos = lib3ds_vector_dot(v3Z, v3Normal);
			#else	// MOD-BY-LEETEN 02/04/2013-TO:
			glm::vec3 v3Z(0.0f, 0.0f, 1.0f);
			glm::vec3 v3Normal = 
				glm::normalize(
					glm::vec3(f4ClipPlane.x, f4ClipPlane.y, f4ClipPlane.z));
		
			glm::vec3 v3Up = glm::normalize(glm::cross(v3Z, v3Normal));
			float fCos = glm::dot(v3Z, v3Normal);
			#endif	// MOD-BY-LEETEN 02/04/2013-END
			float fAngle_degree = 180.0f * acos(fCos) / M_PI;

			glPushMatrix();
			glLoadIdentity();
 
			float3 f3Translation;
			memset(&f3Translation, 0, sizeof(f3Translation));
			switch(usValue)
			{
			case EVENT_SPINNER_PLANE_X:	if(f4ClipPlane.x) f3Translation.x = -f4ClipPlane.w / f4ClipPlane.x;	break;
			case EVENT_SPINNER_PLANE_Y:	if(f4ClipPlane.y) f3Translation.y = -f4ClipPlane.w / f4ClipPlane.y;	break;	
			case EVENT_SPINNER_PLANE_Z:	if(f4ClipPlane.z) f3Translation.z = -f4ClipPlane.w / f4ClipPlane.z;	break;
			}

			glTranslatef(f3Translation.x, f3Translation.y, f3Translation.z);
			glRotatef(fAngle_degree, v3Up[0], v3Up[1], v3Up[2]);
			glTranslatef(-f3Translation.x, -f3Translation.y, -f3Translation.z);

			glGetDoublev(GL_MODELVIEW_MATRIX, tModelMatrix);

			glPopMatrix();
		}
		break;
	case EVENT_SPINNER_PLANE_W:
		{
			// give the plane equation ax + by + cz + d = 0, calculate the current d
			float3 f3Origin;
			f3Origin.x = tModelMatrix[12];
			f3Origin.y = tModelMatrix[13];
			f3Origin.z = tModelMatrix[14];
			float fD = -(f4ClipPlane.x * f3Origin.x + f4ClipPlane.y * f3Origin.y + f4ClipPlane.z * f3Origin.z);
			float fT = (fD - f4ClipPlane.w) / (f4ClipPlane.x * f4ClipPlane.x + f4ClipPlane.y * f4ClipPlane.y + f4ClipPlane.z * f4ClipPlane.z);
			tModelMatrix[12] += fT * f4ClipPlane.x; 
			tModelMatrix[13] += fT * f4ClipPlane.y; 
			tModelMatrix[14] += fT * f4ClipPlane.z; 
		}
		break;
	}
}

void 
CClipWin::CClipEditor::_IdleFunc()
{
	f4ClipPlane.x = (float)tModelMatrix[8];
	f4ClipPlane.y = (float)tModelMatrix[9];
	f4ClipPlane.z = (float)tModelMatrix[10];
	f4ClipPlane.w = -(float)(f4ClipPlane.x * tModelMatrix[12] + f4ClipPlane.y * tModelMatrix[13] + f4ClipPlane.z * tModelMatrix[14]);

	pcClipPlanes->piFlags[iActiveClipPlaneId] = iFlag;
	pcClipPlanes->ppdPlanes[iActiveClipPlaneId][0] = f4ClipPlane.x;
	pcClipPlanes->ppdPlanes[iActiveClipPlaneId][1] = f4ClipPlane.y;
	pcClipPlanes->ppdPlanes[iActiveClipPlaneId][2] = f4ClipPlane.z;
	pcClipPlanes->ppdPlanes[iActiveClipPlaneId][3] = f4ClipPlane.w;

	memcpy(pmMatrices[iActiveClipPlaneId], tModelMatrix, sizeof(tModelMatrix));
}

CClipWin::CClipEditor::CClipEditor()
{
	iActiveClipPlaneId = 0;
	pcClipPlanes = NULL;
	_AddGluiSubwin(GLUI_SUBWINDOW_TOP);

	static TMatrix mIdentity = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	};

	for(int cp = 0; cp < CClipPlanes::NR_OF_CLIP_PLANES; cp++)
	{
		memcpy(pmMatrices[cp], mIdentity, sizeof(TMatrix));
	}
}


CClipWin::CClipEditor::~CClipEditor()
{
}

/*

$Log: not supported by cvs2svn $

*/
