#include "TransFunc.h"

void 
CTransFunc::_LoadRainBow()
{

			// setup the red channel
	plSplines[COLOR_R].clear();
	plSplines[COLOR_R].push_back(CKnot(0.0f, 1.0f));
	plSplines[COLOR_R].push_back(CKnot(1.0f, 0.0f));

			// setup the green channel
	plSplines[COLOR_G].clear();
	plSplines[COLOR_G].push_back(CKnot(0.0f, 0.0f));
	plSplines[COLOR_G].push_back(CKnot(0.5f, 1.0f));
	plSplines[COLOR_G].push_back(CKnot(1.0f, 0.0f));

			// setup the blue channel
	plSplines[COLOR_B].clear();
	plSplines[COLOR_B].push_back(CKnot(0.0f, 0.0f));
	plSplines[COLOR_B].push_back(CKnot(1.0f, 1.0f));

			// setup the alpha channel
	plSplines[COLOR_A].clear();
	plSplines[COLOR_A].push_back(CKnot(0.0f, 0.2f));
	plSplines[COLOR_A].push_back(CKnot(1.0f, 0.2f));
}

void 
CTransFunc::_Load(char szFilename[])
{
}

void 
CTransFunc::_Save(char szFilename[])
{
}

void 
CTransFunc::_ExportColorMap(float pfColorMap[], int iNrOfEntries)
{
	for(int			c = 0; c < CTransFunc::NR_OF_COLORS; c++)
	{
		list<CKnot>::iterator liKnot = plSplines[c].begin();
		list<CKnot>::iterator liPrevKnot;
		for(int		e = 0; e < iNrOfEntries; e++)
		{
			float v = (0.5f + (float)e) / (float)iNrOfEntries;
			for( ; liKnot != plSplines[c].end() && liKnot->fX <= v; liKnot++)
			{
				liPrevKnot = liKnot;
			}

			float fNextX = liKnot->fX;
			float fNextY = liKnot->fY;
			float fPrevX = liPrevKnot->fX;
			float fPrevY = liPrevKnot->fY;

			float fY = (fPrevY * (fNextX - v) + fNextY * (v - fPrevX) ) / (fNextX - fPrevX);
			pfColorMap[e * CTransFunc::NR_OF_COLORS + c] = fY;
		}
	}
}

/*

$Log: not supported by cvs2svn $

*/
