#pragma once

	#include <vector_types.h>	// ADD-BY-LEETEN 2013/07/31

	#include "GlutWin.h"
	#include "ClipPlanes.h"

class CClipWin :
	public CGlutWin
{
	CClipPlanes cClipPlanes;

	struct CBBox{
		float3 f3LowerCorner;
		float3 f3UpperCorner;
	};
	CBBox cBBox;

	int ibPlotClippingPlanes;

	int ibKeepUpdate;

	int ibDisableAll;

	struct CInteriorVolume
	{
		int ibIsEnabled;
		float fOpacity;

		CInteriorVolume()
		{
			ibIsEnabled = 1;
			fOpacity = 1.0f;
		}

		void _AddGlui(GLUI *pcGlui)
		{
			GLUI_Panel *pcPanel = pcGlui->add_panel("Interior Volume");
			GLUI_Checkbox *pcCheckbox = pcGlui->add_checkbox_to_panel(pcPanel, "Enabled?", &ibIsEnabled);
			GLUI_Spinner *pcSpinner = pcGlui->add_spinner_to_panel(pcPanel, "Opacity", GLUI_SPINNER_FLOAT, &fOpacity);
		}
	} cInteriorVolume;
public:

	enum {
		EVENT_BASE = 0x0100,

		EVENT_BUTTON_UPDATE
	};
	
	class CClipEditor :
		public CGlutWin
	{
		CClipPlanes *pcClipPlanes;
	public:
		enum {
			EVENT_BASE = 0x0200,

			EVENT_DISPLAY,
			EVENT_LIST_CLIP_PLANE,
			EVENT_CHECKBOX_ENABLE,
			EVENT_SPINNER_PLANE_X,
			EVENT_SPINNER_PLANE_Y,
			EVENT_SPINNER_PLANE_Z,
			EVENT_SPINNER_PLANE_W,
		};

		int iActiveClipPlaneId;
		int iFlag;
		float4 f4ClipPlane;
		int *piActiveFlag;

		TMatrix pmMatrices[CClipPlanes::NR_OF_CLIP_PLANES];
			
		void _DisplayFunc();
		void _GluiFunc(unsigned short usValue);
		void _InitFunc();
		void _IdleFunc();

		void _SetClipPlanes(CClipPlanes *pcClipPlanes)
		{
			this->pcClipPlanes = pcClipPlanes;
		}

		CClipEditor();
		~CClipEditor();
	};
	CClipEditor cClipEditor;

	void _SetBBox(
		float fX0, float fY0, float fZ0,
		float fX1, float fY1, float fZ1);
	void _PassEnabledClipPlanes();

	void _GluiFunc(unsigned short usValue);
	void _DisplayFunc();
	void _InitFunc();
	void _IdleFunc();

public:
	CClipWin(void);
public:
	virtual ~CClipWin(void);
};

/*

$Log: not supported by cvs2svn $
Revision 1.1  2009/08/26 16:01:12  leeten

[2009/08/26]
1. [1ST] First time checkin.


*/
