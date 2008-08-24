							// include the header for accessing file system to obtain current path
	// ADD-BY-LEETEN 08/15/2008-BEGIN
	#if defined(WIN32)
		#include <windows.h>
	#else
		#include <unistd.h>
	#endif
	// ADD-BY-LEETEN 08/15/2008-END

#include "TfUi.h"

void 
CTfUi::_Undo(int c)
{
	for(	
		plcEditHistorys[c]._PrevAction();
		false == plcEditHistorys[c].BIsBegin();
		plcEditHistorys[c]._PrevAction() )
	{
		int iType;
		CKnot cKnot;
		plcEditHistorys[c]._CurrentAction(iType, cKnot);

		if( CEditHistory::EDIT_ACTION_MARKER == iType )
			break;

		switch(iType)
		{
		case CEditHistory::EDIT_ACTION_ADD:
			_DelKnot(c, cKnot.fX, cKnot.fY, false); 
			break;

		case CEditHistory::EDIT_ACTION_DEL:
			_AddKnot(c, cKnot.fX, cKnot.fY, false); 
			break;
		}
	}
	// ADD-BY-LEETEN 2008/08/17-BEGIN
	_ClearSelectedKnots(c);
	// ADD-BY-LEETEN 2008/08/17-END
}

void 
CTfUi::_Redo(int c)
{
	for(	
		plcEditHistorys[c]._NextAction();
		false == plcEditHistorys[c].BIsEnd();
		plcEditHistorys[c]._NextAction() )
	{
		int iType;
		CKnot cKnot;
		plcEditHistorys[c]._CurrentAction(iType, cKnot);

		if( CEditHistory::EDIT_ACTION_MARKER == iType )
			break;

		switch(iType)
		{
		case CEditHistory::EDIT_ACTION_ADD:
			_AddKnot(c, cKnot.fX, cKnot.fY, false); 
			break;

		case CEditHistory::EDIT_ACTION_DEL:
			_DelKnot(c, cKnot.fX, cKnot.fY, false); 
			break;
		}
	}
	// ADD-BY-LEETEN 2008/08/17-BEGIN
	_ClearSelectedKnots(c);
	// ADD-BY-LEETEN 2008/08/17-END
}

/////////////////////////////////////////////////////////////////////////

void 
CTfUi::_ClearSelectedKnots(int c)
{
	#if	0	// DEL-BY-LEETEN 08/14/2008-BEGIN
		for(vector<list<CKnot>::iterator>::iterator 
			viiCKnot = pviSelectedKnots[c].begin();
			viiCKnot != pviSelectedKnots[c].end();
			viiCKnot ++)
		{		
			list<CKnot>::iterator viCKnot = *viiCKnot;
			viCKnot->bSelected = false;
		}
	#endif	// DEL-BY-LEETEN 08/14/2008-END

	pviSelectedKnots[c].clear();
}

		// delete the selected knots from the spline in the specified channel
void								
CTfUi::_DeleteSelectedKnots(int c)	
{
	if( true == pviSelectedKnots[c].empty() )
		return;

									// add a marker to the edit history
	plcEditHistorys[c]._AddMarker();
	for(vector<list<CKnot>::iterator>::iterator 
		viiCKnot = pviSelectedKnots[c].begin();
		viiCKnot != pviSelectedKnots[c].end();
		viiCKnot ++)
	{		
		list<CKnot>::iterator viKnot = *viiCKnot;

		// ADD-BY-TLEE 2008/06/16-BEGIN
											// get next node
		list<CKnot>::iterator viNextKnot = viKnot;
		viNextKnot++;

											// if this knot is the beginning or the end of the spline
											// replace it by a new knot whose y is 0.0 
		if( viKnot == pcTransFunc->plSplines[c].begin() ||
			viNextKnot == pcTransFunc->plSplines[c].end() )
		{
			plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_DEL, *viKnot);
			viKnot->fY = 0.0f;
			plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_ADD, *viKnot);

			continue;
		}
		// ADD-BY-TLEE 2008/06/16-END

									// save thie operation to the history
		plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_DEL, *viKnot);

									// remove this knot
		pcTransFunc->plSplines[c].erase(viKnot);
	}

	// MOD-BY-LEETEN 2008/08/17-FROM:
		//	pviSelectedKnots[c].clear();
	// TO:
	_ClearSelectedKnots(c);
	// MOD-BY-LEETEN 2008/08/17-END
}

		// select knots in the region
void 
CTfUi::_SelectKnots(int c, float l, float r, float b, float t)
{
	for(list<CKnot>::iterator 
			viKnot	= pcTransFunc->plSplines[c].begin();
			viKnot != pcTransFunc->plSplines[c].end(); 
			viKnot++)
	{
		#if	0	// DEL-BY-LEETEN 2008/08/16-BEGIN
								// skip the fist and last knot
			if( 0.0f == viKnot->fX || 1.0f == viKnot->fX )
				continue;
		#endif	// DEL-BY-LEETEN 2008/08/16-END

		#if	0	// MOD-BY-LEETEN 08/14/2008-FROM:
			if( l <= viKnot->fX && viKnot->fX < r &&
				b <= viKnot->fY && viKnot->fY < t )
		#else	// MOD-BY-LEETEN 08/14/2008-TO:
		if( l <= viKnot->fX && viKnot->fX <= r &&
			b <= viKnot->fY && viKnot->fY <= t )
		#endif	// MOD-BY-LEETEN 08/14/2008-END
		{
			// DEL-BY-LEETEN 08/14/2008-BEGIN
				// viKnot->bSelected = true;
			// DEL-BY-LEETEN 08/14/2008-END
			pviSelectedKnots[c].push_back(viKnot);
		}

		#if	0	// DEL-BY-LEETEN 08/14/2008-BEGIN
			else
				viKnot->bSelected = false;
		#endif	// DEL-BY-LEETEN 08/14/2008-END
	}
}

void
CTfUi::_DelKnot(int c, float x, float y, bool bAddToHistory)
{
	assert(0.0f <= x && x <= 1.0f);
	assert(0.0f <= y && y <= 1.0f);

	for(list<CKnot>::iterator 
			viKnot	= pcTransFunc->plSplines[c].begin();
			viKnot != pcTransFunc->plSplines[c].end(); 
			viKnot++)
	{
		if( viKnot->fX == x && viKnot->fY == y )
		{
			if( bAddToHistory )
			{
				plcEditHistorys[c]._AddMarker();
				plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_DEL, *viKnot);
			}

			// ADD-BY-LEETEN 2008/08/14-BEGIN
											// if this knot is the begining or ending of this spline, 
											// instead of erase this node, reset thie height to zero
			if( x == 0.0f || x == 1.0f )
				viKnot->fY = y;
			else

			// ADD-BY-LEETEN 2008/08/14-END
			pcTransFunc->plSplines[c].erase(viKnot);
			break;
		}
	}
}

void
CTfUi::_AddKnot(int c, float x, float y, bool bAddToHistory)
{
	if( !(0.0f <= x && x <= 1.0f) ||
		!(0.0f <= y && y <= 1.0f)	)
		return;

	CKnot cNewKnot(x, y);

	for(list<CKnot>::iterator 
			viKnot	= pcTransFunc->plSplines[c].begin();
			viKnot != pcTransFunc->plSplines[c].end(); 
			viKnot++)
	{
		if( cNewKnot.fX < viKnot->fX )
		{
			if( bAddToHistory )
			{
				plcEditHistorys[c]._AddMarker();
				plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_ADD, cNewKnot);
			}

			pcTransFunc->plSplines[c].insert(viKnot, cNewKnot);
			break;
		}

		if( cNewKnot.fX == viKnot->fX )
		{
			if( bAddToHistory )
			{
				plcEditHistorys[c]._AddMarker();
				plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_DEL, *viKnot);
				plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_ADD, cNewKnot);
			}

			viKnot->fY = cNewKnot.fY;
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
#if	0	// DEL-BY-LEETEN 08/14/2008-BEGIN
	void
	CTfUi::_CreateTransferFunc(int iNrOfEntries, float pfTransferFunc[])
	{
	_Begin();
		for(int			c = 0; c < CTransFunc::NR_OF_COLORS; c++)
		{
			list<CKnot>::iterator liKnot = pcTransFunc->plSplines[c].begin();
			list<CKnot>::iterator liPrevKnot;
			for(int		e = 0; e < iNrOfEntries; e++)
			{
				float v = (0.5f + (float)e) / (float)iNrOfEntries;
				for( ; liKnot != pcTransFunc->plSplines[c].end() && liKnot->fX <= v; liKnot++)
				{
					liPrevKnot = liKnot;
				}

				float fNextX = liKnot->fX;
				float fNextY = liKnot->fY;
				float fPrevX = liPrevKnot->fX;
				float fPrevY = liPrevKnot->fY;

				float fY = (fPrevY * (fNextX - v) + fNextY * (v - fPrevX) ) / (fNextX - fPrevX);
				pfTransferFunc[e * CTransFunc::NR_OF_COLORS + c] = fY;
			}
		}
	_End();
	}
#endif	// DEL-BY-LEETEN 08/14/2008-END

///////////////////////////////////////////////////////////////////////////
void 
// MOD-BY-LEETEN 08/14/2008-FROM:
	// CTfUi::_PlotSpline(int c, bool bPlotKnots)
// TO:
CTfUi::_PlotSpline(int c, bool bEnhance)
// MOD-BY-LEETEN 08/14/2008-END
{
_Begin();
	switch(c)
	{
	case 0:	glColor4f(1.0, 0.0f, 0.0f, 1.0f);	break;
	case 1:	glColor4f(0.0, 1.0f, 0.0f, 1.0f);	break;
	case 2:	glColor4f(0.0, 0.0f, 1.0f, 1.0f);	break;
	case 3:	glColor4f(0.3, 0.3f, 0.3f, 1.0f);	break;
	}

	// ADD-BY-LEETEN 08/14/2008-BEGIN
	glPushAttrib(GL_LINE_BIT);
	if( true == bEnhance )
		glLineWidth(3.0f);
	// ADD-BY-LEETEN 08/14/2008-END

	glBegin(GL_LINE_STRIP);
		for(list<CKnot>::iterator 
				viKnot	= pcTransFunc->plSplines[c].begin();
				viKnot != pcTransFunc->plSplines[c].end(); 
				viKnot++)
			glVertex2f(viKnot->fX, viKnot->fY);
	glEnd();

	// ADD-BY-LEETEN 08/14/2008-BEGIN
	glPopAttrib();	//	glPushAttrib(GL_LINE_BIT);
	// ADD-BY-LEETEN 08/14/2008-END

	// MOD-BY-LEETEN 08/14/2008-FROM:
		// if( false == bPlotKnots )
	// TO:
	if( false == bEnhance )
	// MOD-BY-LEETEN 08/14/2008-END
		return;

	glPushAttrib(GL_POINT_BIT);
								// plot the selected knots

		#if	0	// MOD-BY-LEETEN 08/14/2008-FROM:
			glPointSize(8.0);
			glBegin(GL_POINTS);
				for(list<CKnot>::iterator 
						viKnot	= plSplines[c].begin();
						viKnot != plSplines[c].end(); 
						viKnot++)
				{
					if( viKnot->bSelected )
						glVertex2f(viKnot->fX, viKnot->fY);
				}
			glEnd();

									// plot the selected knots
			glPointSize(6.0);
			glPushAttrib(GL_CURRENT_BIT);
				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

				glBegin(GL_POINTS);
					for(list<CKnot>::iterator 
							viKnot	= plSplines[c].begin();
							viKnot != plSplines[c].end(); 
							viKnot++)
					{
						if( viKnot->bSelected )
							glVertex2f(viKnot->fX, viKnot->fY);
					}
				glEnd();
			glPopAttrib();	//	glPushAttrib(GL_CURRENT_BIT);

		#else	// MOD-BY-LEETEN 08/14/2008-TO:

		glPointSize(8.0);
		glBegin(GL_POINTS);
			for(vector<list<CKnot>::iterator>::iterator 
					viliKnot	=	pviSelectedKnots[c].begin();
					viliKnot	!=	pviSelectedKnots[c].end(); 
					viliKnot++)
			{
				list<CKnot>::iterator liKnot = *viliKnot;
				glVertex2f(liKnot->fX, liKnot->fY);
			}
		glEnd();

								// plot the selected knots
		glPointSize(6.0);
		glPushAttrib(GL_CURRENT_BIT);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

			glBegin(GL_POINTS);
				for(vector<list<CKnot>::iterator>::iterator 
						viliKnot	=	pviSelectedKnots[c].begin();
						viliKnot	!=	pviSelectedKnots[c].end(); 
						viliKnot++)
				{
					list<CKnot>::iterator liKnot = *viliKnot;
					glVertex2f(liKnot->fX, liKnot->fY);
				}
			glEnd();
		glPopAttrib();	//	glPushAttrib(GL_CURRENT_BIT);
		#endif	// MOD-BY-LEETEN 08/14/2008-END

								// plot the knots
		glPointSize(4.0);
		glBegin(GL_POINTS);
			for(list<CKnot>::iterator 
					viKnot	= pcTransFunc->plSplines[c].begin();
					viKnot != pcTransFunc->plSplines[c].end(); 
					viKnot++)
				glVertex2f(viKnot->fX, viKnot->fY);
		glEnd();
	glPopAttrib();	// glPushAttrib(GL_POINT_BIT);
_End();
}

///////////////////////////////////////////////////////////////////////////
void 
CTfUi::_MouseCB(int button, int state, int x, int y)
{
	_UpdateWinCoord(&x, &y);

	CMouseEvent cNewEvent;
	cNewEvent.iButton	= button;
	cNewEvent.iState	= state;
	cNewEvent.iX		= x;
	cNewEvent.iY		= y;
	vcMouseEvents.push_back(cNewEvent);
}

void 
CTfUi::_MotionCB(int x, int y)
{
	if( true == vcMouseEvents.empty() )
		return;

	_UpdateWinCoord(&x, &y);

	CMouseEvent cNewEvent;
	cNewEvent.iButton	= vcMouseEvents[0].iButton;
	cNewEvent.iState	= vcMouseEvents[0].iState;
	cNewEvent.iX		= x;
	cNewEvent.iY		= y;
	vcMouseEvents.push_back(cNewEvent);
}

void 
CTfUi::_KeyboardCB(unsigned char key, int x, int y)
{
	_UpdateWinCoord(&x, &y);

	switch(key) 
	{
	case 'Z': case 'z':
		this->_GluiFunc(EDIT_UNDO);
		break;

	case 'Y': case 'y':
		this->_GluiFunc(EDIT_REDO);
		break;

	// ADD-BY-LEETEN 2008/08/17-BEGIN
	case 'D': case 'd':	// D for 'delete'
	// ADD-BY-LEETEN 2008/08/17-END
	case 127:	// DEL
		#if	0	// MOD-BY-LEETEN 2008/08/17-FROM:
			_DeleteSelectedKnots(iEditingChannel);
			_Redisplay();
		#else	// MOD-BY-LEETEN 2008/08/17-TO:
		this->_GluiFunc(EDIT_DELETE);
		#endif	// MOD-BY-LEETEN 2008/08/17-END
		break;
	}
}

void
CTfUi::_IdleCB()
{
	if( false == vcMouseEvents.empty() )
	{
		vector<CMouseEvent>::reverse_iterator vriMouseEvent = vcMouseEvents.rbegin();
		switch( vriMouseEvent->iButton )
		{
		case GLUT_RIGHT_BUTTON:
			switch( vriMouseEvent->iState)
			{
			case GLUT_UP:
										// add one knot
				_AddKnot(iEditingChannel, (float)vriMouseEvent->iX / (float)piViewport[2], (float)vriMouseEvent->iY / (float)piViewport[3]);
				_ClearSelectedKnots(iEditingChannel);	
										// reset the mouse event
				vcMouseEvents.clear();
				break;
			}
			break;

		case GLUT_LEFT_BUTTON:
			switch( vriMouseEvent->iState)
			{	
			case GLUT_UP:				// select all knots in the region
				{
											// decide the region
					vector<CMouseEvent>::iterator vcFirstEvevt = vcMouseEvents.begin();
					vector<CMouseEvent>::iterator vcLastEvevt = vcMouseEvents.end() - 1;
					float l, b, r, t;
					l = (float)vcFirstEvevt->iX / (float)piViewport[2];
					r = (float)vcLastEvevt->iX	/ (float)piViewport[2];
					if( r < l )
						swap(l, r);
					b = (float)vcFirstEvevt->iY / (float)piViewport[3];
					t = (float)vcLastEvevt->iY	/ (float)piViewport[3];
					if( t < b )
						swap(b, t);

											// reset the selected knots
					_ClearSelectedKnots(iEditingChannel);

											// select the knots in the region
					_SelectKnots(iEditingChannel, l, r, b, t);
											// reset the mouse event
					vcMouseEvents.clear();
				}
				break;

			case GLUT_DOWN:
				break;

			}
			break;
		}
		_Redisplay();
	}
}

void
CTfUi::_ClearChannel(int channel)
{
											// create a check point in the history
	plcEditHistorys[channel]._AddMarker();
											// record that all knots are deleted to the history
	for(list<CKnot>::iterator 
			viKnot	= pcTransFunc->plSplines[channel].begin();
			viKnot != pcTransFunc->plSplines[channel].end(); 
			viKnot++)
		plcEditHistorys[channel]._AddAction(CEditHistory::EDIT_ACTION_DEL, *viKnot);

											// clear all knots
	pcTransFunc->plSplines[channel].clear();

											// add the two default knots
	CKnot cBegin(0.0f, 0.0f);
	CKnot cEnd(1.0f, 0.0f);
	plcEditHistorys[channel]._AddAction(CEditHistory::EDIT_ACTION_ADD, cEnd);
	plcEditHistorys[channel]._AddAction(CEditHistory::EDIT_ACTION_ADD, cBegin);
	pcTransFunc->plSplines[channel].push_back(cBegin);
	pcTransFunc->plSplines[channel].push_back(cEnd);

	// ADD-By-TLEE 08/14/2008-BEGIN
	// MOD-BY-LEETEN 2008/08/17-FROM:
		// pviSelectedKnots[channel].clear();
	// TO:
	_ClearSelectedKnots(channel);
	// MOD-BY-LEETEN 2008/08/17-END
	// ADD-By-TLEE 08/14/2008-END
}

void
CTfUi::_GluiFunc(unsigned short usValue)
{
	switch(usValue)
	{
	// ADD-BY-LEETEN 2008/08/17-BEGIN
	case EDIT_DELETE:
		_DeleteSelectedKnots(iEditingChannel);
		_Redisplay();
		break;
	// ADD-BY-LEETEN 2008/08/17-END

	case EDIT_REDO:
		_Redo(iEditingChannel);
		_Redisplay();
		break;

	case EDIT_UNDO:
		_Undo(iEditingChannel);
		_Redisplay();
		break;

	case EDIT_UPDATE:
		if( _UpdateFunc )
			_UpdateFunc();
		else
			_AddToLog("_UpdateFunc is not given yet.");
		break;

	// ADD-BY-LEETEN 08/14/2008-BEGIN
	case EDIT_CLEAR:
							// save all current knots to the history
		_ClearChannel(iEditingChannel);
		_Redisplay();
		break;
	// ADD-BY-LEETEN 08/14/2008-END

	// ADD-BY-LEETEN 08/15/2008-BEGIN
	case FILE_OPEN:			// load TF from a file
		{
			_GetPath();

			_AddToLog(SZSprintf("Open file %s", szPath));

			if( !pcTransFunc->BCheckFile(szPath) )
			{
				_AddToLog(SZSprintf("Cannot open %s", szPath));
			}
			else
			{
				for(int c = 0; c < CTransFunc::NR_OF_COLORS; c++)
				{
												// clear the set of selected knots
					// MOD-BY-TLEE 2008/08/20-FROM:
						// pviSelectedKnots[c].clear();
					// TO:
					_ClearSelectedKnots(c);
												// reset the edit history
					plcEditHistorys[c]._Clear();
				}
												// open the file. all knots will be erased
				pcTransFunc->BOpenFile(szPath);
				_Redisplay();
			}
		}
		break;

	case FILE_SAVE:				// save current TF to a file
		{
			_GetPath();

			_AddToLog(SZSprintf("Save to file %s", szPath));

			pcTransFunc->BSaveFile(szPath);
		}
		break;

	// ADD-BY-LEETEN 08/15/2008-END

	case EDIT_EXIT:
		exit(0);
	}
}

void 
CTfUi::_InitFunc()
{
	assert(pcTransFunc);

											// move the File panel to above the Edit panel
	// ADD-BY-LEETEN 08/15/2008-BEGIN
											// add new control for accessing TF as files
	// MOD-BY-LEETEN 08/16/2008-FROM:
		// GLUI_Panel *pcFilePanel = PCGetGluiSubwin()->add_panel("File");
	// TO:
	GLUI_Rollout* pcFilePanel = PCGetGluiSubwin()->add_rollout("File", false);
	// MOD-BY-LEETEN 08/16/2008-END
		pcEditText_Dir		= PCGetGluiSubwin()->add_edittext_to_panel(pcFilePanel, "Dir",		GLUI_EDITTEXT_TEXT, szDir,		IAddWid(FILE_DIR),		&CGlutWin::_GluiCB_static);
		pcEditText_Filename = PCGetGluiSubwin()->add_edittext_to_panel(pcFilePanel, "Filename", GLUI_EDITTEXT_TEXT, szFilename, IAddWid(FILE_FILENAME), &CGlutWin::_GluiCB_static);
		#if	0	// MOD-BY-LEETEN 2008/08/19-FROM:
			_AddButton("Save", FILE_SAVE, pcFilePanel);
			_AddButton("Open", FILE_OPEN, pcFilePanel);
		#else	// MOD-BY-LEETEN 2008/08/19-TO:
		PCGetGluiSubwin()->add_button_to_panel(pcFilePanel, "Save", IAddWid(FILE_SAVE), &CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcFilePanel, "Open", IAddWid(FILE_OPEN), &CGlutWin::_GluiCB_static);
		#endif	// MOD-BY-LEETEN 2008/08/19-END

									// by default, use current director and *.* as the filename
	char szDir[1024+1];
	#if defined(WIN32)
		// MOD-BY-TLEE 2008/08/23-FROM:
			// GetCurrentDirectoryA((DWORD)strlen(szDir), szDir);
		// TO:
		GetCurrentDirectoryA((DWORD)sizeof(szDir), szDir);
		// MOD-BY-TLEE 2008/08/23-END
	#else
		getcwd(szDir, strlen(szDir));
	#endif
	_SetDir(szDir);
	_SetFilename("*.*");

	// ADD-BY-LEETEN 08/15/2008-END

											// add a panel to group the radio group and buttons
	#if	0	// MOD-BY-LEETEN 08/15/2008-FROM:
		GLUI_Panel *pcChannelPanel = PCGetGluiSubwin()->add_panel("Channel");
		GLUI_RadioGroup* pcRadioGroup = PCGetGluiSubwin()->add_radiogroup_to_panel(pcChannelPanel, &iEditingChannel, CTransFunc::COLOR_R);
			PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup, "R");
			PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup, "G");
			PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup, "B");
			PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup, "A");
			pcRadioGroup->set_int_val(iEditingChannel);
	#else	// MOD-BY-LEETEN 08/15/2008-TO:
	// MOD-BY-LEETEN 08/16/2008-FROM:
		// GLUI_Panel *pcEditPanel = PCGetGluiSubwin()->add_panel("Edit");
	// TO:
	GLUI_Rollout* pcEditPanel  = PCGetGluiSubwin()->add_rollout("Edit");
	// MOD-BY-LEETEN 08/16/2008-END

	GLUI_Panel *pcChannelPanel = PCGetGluiSubwin()->add_panel_to_panel(pcEditPanel, "Channel");
	GLUI_RadioGroup *pcRadioGroup_Channel = PCGetGluiSubwin()->add_radiogroup_to_panel(pcChannelPanel, &iEditingChannel);
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup_Channel, "R");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup_Channel, "G");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup_Channel, "B");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup_Channel, "A");
		pcRadioGroup_Channel->set_int_val(iEditingChannel);

		#if	0	// MOD-BY-LEETEN 08/19/2008-FROM:
			_AddButton("Redo", EDIT_REDO, pcEditPanel);
			_AddButton("Undo", EDIT_UNDO, pcEditPanel);
			_AddButton("Clear", EDIT_CLEAR, pcEditPanel);
			// ADD-BY-LEETEN 2008/08/17-BEGIN
			_AddButton("Delete", EDIT_DELETE, pcEditPanel);
			// ADD-BY-LEETEN 2008/08/17-END
		#else	// MOD-BY-LEETEN 08/19/2008-TO:
		PCGetGluiSubwin()->add_button_to_panel(pcEditPanel, "Redo",		IAddWid(EDIT_REDO),		&CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcEditPanel, "Undo",		IAddWid(EDIT_UNDO),		&CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcEditPanel, "Clear",	IAddWid(EDIT_CLEAR),	&CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcEditPanel, "Delete",	IAddWid(EDIT_DELETE),	&CGlutWin::_GluiCB_static);
		#endif	// MOD-BY-LEETEN 08/19/2008-END
	#endif	// MOD-BY-LEETEN 08/15/2008-END

	#if	0	// DEL-BY-LEETEN 08/15/2008-BEGIN
		_AddButton("Redo", EDIT_REDO);
		_AddButton("Undo", EDIT_UNDO);
		_AddButton("Update", EDIT_UPDATE);
		// ADD-BY-LEETEN 08/14/2008-BEGIN
		_AddButton("Clear", EDIT_CLEAR);
		// ADD-BY-LEETEN 08/14/2008-END
	#endif	// DEL-BY-LEETEN 08/15/2008-END

	// ADD-BY-TLEE 2008/08/21-BEGIN
	GLUI_Rollout* pcTfPanel = PCGetGluiSubwin()->add_rollout("TF Setting", false);
	GLUI_Panel* pcPanel;
	GLUI_Spinner *pcSpinner;
		pcPanel = PCGetGluiSubwin()->add_panel_to_panel(pcTfPanel, "Data Value");
			pcSpinner = PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Min", GLUI_SPINNER_FLOAT, &fHistogramMin);
			pcSpinner->disable();
			pcSpinner = PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Max", GLUI_SPINNER_FLOAT, &fHistogramMax);
			pcSpinner->disable();
		pcPanel = PCGetGluiSubwin()->add_panel_to_panel(pcTfPanel, "TF Domain");
			PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Min", GLUI_SPINNER_FLOAT, &pcTransFunc->fDomainMin);	
			PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Max", GLUI_SPINNER_FLOAT, &pcTransFunc->fDomainMax);	
	// ADD-BY-TLEE 2008/08/21-END

											// move the File panel to above the Edit panel
	#if	0	// DEL-BY-LEETEN 08/15/2008-BEGIN
											// add new control for accessing TF as files
		// ADD-BY-LEETEN 08/15/2008-BEGIN
		GLUI_Panel *pcFilePanel = PCGetGluiSubwin()->add_panel("File");
			pcEditText_Dir		= PCGetGluiSubwin()->add_edittext_to_panel(pcFilePanel, "Dir",		GLUI_EDITTEXT_TEXT, szDir,		IAddWid(FILE_DIR),		&CGlutWin::_GluiCB_static);
			pcEditText_Filename = PCGetGluiSubwin()->add_edittext_to_panel(pcFilePanel, "Filename", GLUI_EDITTEXT_TEXT, szFilename, IAddWid(FILE_FILENAME), &CGlutWin::_GluiCB_static);
			_AddButton("Save", FILE_SAVE, pcFilePanel);
			_AddButton("Open", FILE_OPEN, pcFilePanel);
		// ADD-BY-LEETEN 08/15/2008-END

	#endif	// DEL-BY-LEETEN 08/15/2008-BEGIN

	#if	0	// MOD-BY-LEETEN 08/19/2008-FROM:
		_AddButton("Update", EDIT_UPDATE);
		_AddButton("Exit", EDIT_EXIT);
	#else	// MOD-BY-LEETEN 08/19/2008-TO:
	PCGetGluiSubwin()->add_button("Update",	IAddWid(EDIT_UPDATE),	&CGlutWin::_GluiCB_static);
	PCGetGluiSubwin()->add_button("Exit",	IAddWid(EDIT_EXIT),		&CGlutWin::_GluiCB_static);
	#endif	// MOD-BY-LEETEN 08/19/2008-END

							// specify a default window size for the TF editor
	// ADD-BY-LEETEN 08/15/2008-BEGIN
	_Reshape(512, 384);
	// ADD-BY-LEETEN 08/15/2008-END
}

void 
CTfUi::_DisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		// normalize the coordinate
		glTranslatef(-1.0f, -1.0f, -1.0f);
		glScalef(2.0f, 2.0f, 2.0f);

		// plot the histogram as backgroun
		glPushMatrix();
		// MOD-BY-TLEE 2008/08/21-FROM:
			// glScalef(1.0f / (float)pfHistogram.num, 1.0f, 1.0f);
		// TO:
		glScalef(1.0f / (float)(pcTransFunc->fDomainMax - pcTransFunc->fDomainMin), 1.0f, 1.0f);
		glTranslatef(-pcTransFunc->fDomainMin, 0.0f, 0.0f);
		// MOD-BY-TLEE 2008/08/21-END
		
		glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			for(int i = 0; i < pfHistogram.num; i++)
			{
				float l, r, b, t;
				#if	0	// MOD-BY-TLEE 2008/08/21-FROM:
					l = (float)i;
					r = (float)i + 1;
				#else	// MOD-BY-TLEE 2008/08/21-TO:
				l = (float) i / (float) pfHistogram.num * (float)(fHistogramMax - fHistogramMin);
				r = (float) (i + 1) / (float) pfHistogram.num * (float)(fHistogramMax - fHistogramMin);
				#endif	// MOD-BY-TLEE 2008/08/21-END

				b = 0.0f;
				t = pfHistogram[i];
				glVertex2f(l, b);
				glVertex2f(r, b);
				glVertex2f(r, t);
				glVertex2f(l, t);
			}
			glEnd();

		glPopMatrix();

		#if	0	// MOD-BY-TLEE 2008/08/21-FROM:
			// ADD-BY-LEETEN 2008/08/17-BEGIN
			// plot the range
			glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
			_DrawString(SZSprintf("%.2e", fHistogramMin), 0, 0, false);
			_DrawString(SZSprintf("%.2e", fHistogramMax), -1, 0, true);
			// ADD-BY-LEETEN 2008/08/17-END
		#else	// MOD-BY-TLEE 2008/08/21-TO:
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		_DrawString(SZSprintf("%.2e", pcTransFunc->fDomainMin), 0, 0, false);
		_DrawString(SZSprintf("%.2e", pcTransFunc->fDomainMax), -1, 0, true);
		#endif	// MOD-BY-TLEE 2008/08/21-END

		// plot the transfer func. as lines
		for(int c = 0; c < CTransFunc::NR_OF_COLORS; c++)	
		{
										// plot the spline
			_PlotSpline(c);
		}

										// plot the editing spline in thicker line
		_PlotSpline(iEditingChannel, true);


									// plot the select region if the left mouse is being pressed
		if( false == vcMouseEvents.empty() && GLUT_LEFT_BUTTON == vcMouseEvents.begin()->iButton )
		{
			vector<CMouseEvent>::iterator vcFirstEvevt = vcMouseEvents.begin();
			vector<CMouseEvent>::iterator vcLastEvevt = vcMouseEvents.end() - 1;
			if( GLUT_LEFT_BUTTON == vcFirstEvevt->iButton )
			{
				glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
				glBegin(GL_LINE_LOOP);
					float l, b, r, t;
					l = (float)vcFirstEvevt->iX / (float)piViewport[2];
					r = (float)vcLastEvevt->iX	/ (float)piViewport[2];
					b = (float)vcFirstEvevt->iY / (float)piViewport[3];
					t = (float)vcLastEvevt->iY	/ (float)piViewport[3];
					glVertex2f(l, b);
					glVertex2f(r, b);
					glVertex2f(r, t);
					glVertex2f(l, t);
				glEnd();
			}
		}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////
CTfUi::CTfUi()
{
	iEditingChannel = CTransFunc::COLOR_A;
	pcTransFunc = NULL;
	_UpdateFunc = NULL;
	_AddGluiSubwin(GLUI_SUBWINDOW_LEFT);

	// ADD-BY-LEETEN 08/15/2008-BEGIN
	szFilename[0] = '\0';
	szDir[0] = '\0';
	szPath[0] = '\0'; 
	pcEditText_Filename = NULL;
	pcEditText_Dir = NULL;
	// ADD-BY-LEETEN 08/15/2008-END

	// ADD-BY-LEETEN 2008/08/17-BEGIN
	fHistogramMin = 0.0f;
	fHistogramMax = 1.0f;
	// ADD-BY-LEETEN 2008/08/17-END
}

CTfUi::~CTfUi(void)
{
}

/*

$Log: not supported by cvs2svn $
Revision 1.8  2008/08/21 14:53:47  leeten

[2008/08/21]
1. [ADD] Display the range of the data value as disabled spinners
2. [ADD] Add two spinners to control the TF domain.
3. [CHANGE] When plot the TF, draw the support of TF other than data range.

Revision 1.7  2008/08/20 19:55:20  leeten

[2008/08/20]
1. [CHANGE] Use GLUI::add_button to replace CGlutWin::_AddButton.
2. [CHANGE] Change the precision of the printed data range.

Revision 1.6  2008/08/18 00:41:59  leeten

[2008/08/17]
1. [DEBUG] Clear the selected knots after any of the edit buttons are pressed.

Revision 1.5  2008/08/17 23:53:28  leeten

[2008/08/17]
1. [ADD] Add a button to delete selected knots. To handle this event, a new event index EDIT_DELETE is defined.
2. [ADD] Add a hotkey 'D' to delete selected knots.
3. [CHANGE] Use rollouts other than panels to group user control.
4. [ADD] When plot the histogram, also display the range of the histogram. Two variables fHistogramMin and fHistogramMax are thus defined.

Revision 1.4  2008/08/16 16:36:34  leeten

[2008/08/14]
1. [ADD] When the window is created, use current directory and *.* as the path, and initialize thw window size as 512x384.
2. [ADD] When select the knots, the first and last knots can be selected too.

Revision 1.3  2008/08/15 14:47:44  leeten

[2008/08/15]
1. [ADD] Add new functionalities to open/save the TF as files. The use can specify the dir. and path of the file via edit texts, and tow button are added to load/save the TF.

Revision 1.2  2008/08/14 23:00:25  leeten

[2008/08/14]
1. [DEBUG] Clear the selected knots after the spline has been cleared.

Revision 1.1.1.1  2008/08/14 14:44:02  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN]. This library defines classes for trasnfer functions, including editing and displaying.

Revision 1.1  2008/08/13 21:17:21  leeten

[2008/08/13]
1. First time checkin. This class can create a GLUT window for editing transfer function.


*/
