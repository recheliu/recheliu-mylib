							// include the header for accessing file system to obtain current path
	#if defined(WIN32)
		#include <windows.h>
	#else
		#include <unistd.h>
	#endif

#include "TfUi.h"

void
CTfUi::_SetReceiver(
	CReceiver *pcReceiver
)
{
	this->pcReceiver = pcReceiver;
}

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
	_ClearSelectedKnots(c);
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
	_ClearSelectedKnots(c);
}

/////////////////////////////////////////////////////////////////////////

void 
CTfUi::_ClearSelectedKnots(int c)
{
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

									// save thie operation to the history
		plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_DEL, *viKnot);

									// remove this knot
		pcTransFunc->plSplines[c].erase(viKnot);
	}

	_ClearSelectedKnots(c);
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
		if( l <= viKnot->fX && viKnot->fX <= r &&
			b <= viKnot->fY && viKnot->fY <= t )
		{
			pviSelectedKnots[c].push_back(viKnot);
		}
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

											// if this knot is the begining or ending of this spline, 
											// instead of erase this node, reset thie height to zero
			if( x == 0.0f || x == 1.0f )
				viKnot->fY = y;
			else

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
///////////////////////////////////////////////////////////////////////////
void 
CTfUi::_PlotSpline(int c, bool bEnhance)
{
_Begin();
	switch(c)
	{
	case 0:	glColor4f(1.0, 0.0f, 0.0f, 1.0f);	break;
	case 1:	glColor4f(0.0, 1.0f, 0.0f, 1.0f);	break;
	case 2:	glColor4f(0.0, 0.0f, 1.0f, 1.0f);	break;
	case 3:	glColor4f(0.3, 0.3f, 0.3f, 1.0f);	break;
	}

	glPushAttrib(GL_LINE_BIT);
	if( true == bEnhance )
		glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
		for(list<CKnot>::iterator 
				viKnot	= pcTransFunc->plSplines[c].begin();
				viKnot != pcTransFunc->plSplines[c].end(); 
				viKnot++)
			glVertex2f(viKnot->fX, viKnot->fY);
	glEnd();

	glPopAttrib();	//	glPushAttrib(GL_LINE_BIT);

	if( false == bEnhance )
		return;

	glPushAttrib(GL_POINT_BIT);
								// plot the selected knots

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

	case 'D': case 'd':	// D for 'delete'
	case 127:	// DEL
		this->_GluiFunc(EDIT_DELETE);
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

	_ClearSelectedKnots(channel);
}

void
CTfUi::_GluiFunc(unsigned short usValue)
{
	switch(usValue)
	{
	case EDIT_DELETE:
		_DeleteSelectedKnots(iEditingChannel);
		_Redisplay();
		break;

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

		pcReceiver->_Receive(pcTransFunc);	
		break;

	case EDIT_CLEAR:
							// save all current knots to the history
		_ClearChannel(iEditingChannel);
		_Redisplay();
		break;

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

	case EDIT_EXIT:
		exit(0);
	}
}

void 
CTfUi::_InitFunc()
{
	assert(pcTransFunc);

											// move the File panel to above the Edit panel
											// add new control for accessing TF as files
	GLUI_Rollout* pcFilePanel = PCGetGluiSubwin()->add_rollout("File", false);
		pcEditText_Dir		= PCGetGluiSubwin()->add_edittext_to_panel(pcFilePanel, "Dir",		GLUI_EDITTEXT_TEXT, szDir,		IAddWid(FILE_DIR),		&CGlutWin::_GluiCB_static);
		pcEditText_Filename = PCGetGluiSubwin()->add_edittext_to_panel(pcFilePanel, "Filename", GLUI_EDITTEXT_TEXT, szFilename, IAddWid(FILE_FILENAME), &CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcFilePanel, "Save", IAddWid(FILE_SAVE), &CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcFilePanel, "Open", IAddWid(FILE_OPEN), &CGlutWin::_GluiCB_static);

									// by default, use current director and *.* as the filename
	char szDir[1024+1];
	#if defined(WIN32)
		GetCurrentDirectoryA((DWORD)sizeof(szDir), szDir);
	#else
		getcwd(szDir, sizeof(szDir));
	#endif
	_SetDir(szDir);
	_SetFilename("*.*");

											// add a panel to group the radio group and buttons
	GLUI_Rollout* pcEditPanel  = PCGetGluiSubwin()->add_rollout("Edit");

	GLUI_Panel *pcChannelPanel = PCGetGluiSubwin()->add_panel_to_panel(pcEditPanel, "Channel");
	GLUI_RadioGroup *pcRadioGroup_Channel = PCGetGluiSubwin()->add_radiogroup_to_panel(pcChannelPanel, &iEditingChannel);
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup_Channel, "R");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup_Channel, "G");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup_Channel, "B");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup_Channel, "A");
		pcRadioGroup_Channel->set_int_val(iEditingChannel);

		PCGetGluiSubwin()->add_button_to_panel(pcEditPanel, "Redo",		IAddWid(EDIT_REDO),		&CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcEditPanel, "Undo",		IAddWid(EDIT_UNDO),		&CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcEditPanel, "Clear",	IAddWid(EDIT_CLEAR),	&CGlutWin::_GluiCB_static);
		PCGetGluiSubwin()->add_button_to_panel(pcEditPanel, "Delete",	IAddWid(EDIT_DELETE),	&CGlutWin::_GluiCB_static);

	GLUI_Rollout* pcTfPanel = PCGetGluiSubwin()->add_rollout("TF Setting", false);
	GLUI_Panel* pcPanel;
	GLUI_Spinner *pcSpinner;
		pcPanel = PCGetGluiSubwin()->add_panel_to_panel(pcTfPanel, "Data Value");
			pcSpinner = PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Min (Significand)",	GLUI_SPINNER_FLOAT, &cHistogramMin.fSignificand);
			pcSpinner->disable();
			pcSpinner = PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Min (Exponent)",		GLUI_SPINNER_FLOAT, &cHistogramMin.fExponent);
			pcSpinner->disable();

			pcSpinner = PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Max (Significand)",	GLUI_SPINNER_FLOAT, &cHistogramMax.fSignificand);
			pcSpinner->disable();
			pcSpinner = PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Max (Exponent)",		GLUI_SPINNER_FLOAT, &cHistogramMax.fExponent);
			pcSpinner->disable();

		pcPanel = PCGetGluiSubwin()->add_panel_to_panel(pcTfPanel, "TF Domain");

			PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Min (Significand)",	GLUI_SPINNER_FLOAT, &pcTransFunc->cDomainMin.fSignificand);	
			PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Min (Exponent)",		GLUI_SPINNER_FLOAT, &pcTransFunc->cDomainMin.fExponent);	

			PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Max (Significand)",	GLUI_SPINNER_FLOAT, &pcTransFunc->cDomainMax.fSignificand);	
			PCGetGluiSubwin()->add_spinner_to_panel(pcPanel, "Max (Exponent)",		GLUI_SPINNER_FLOAT, &pcTransFunc->cDomainMax.fExponent);	

											// move the File panel to above the Edit panel
	PCGetGluiSubwin()->add_button("Update",	IAddWid(EDIT_UPDATE),	&CGlutWin::_GluiCB_static);
	PCGetGluiSubwin()->add_button("Exit",	IAddWid(EDIT_EXIT),		&CGlutWin::_GluiCB_static);

							// specify a default window size for the TF editor
	_Reshape(512, 384);
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
		float fDomainMin  = pcTransFunc->cDomainMin.FGetValue();
		float fDomainMax  = pcTransFunc->cDomainMax.FGetValue();
		float fHistogramMin = cHistogramMin.FGetValue();
		float fHistogramMax = cHistogramMax.FGetValue();

		glScalef(1.0f / (float)(fDomainMax - fDomainMin), 1.0f, 1.0f);
		glTranslatef(-fDomainMin, 0.0f, 0.0f);
		
		glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			for(int i = 0; i < pfHistogram.num; i++)
			{
				float l, r, b, t;
				l = (float) i / (float) pfHistogram.num * (float)(fHistogramMax - fHistogramMin) + fHistogramMin;
				r = (float) (i + 1) / (float) pfHistogram.num * (float)(fHistogramMax - fHistogramMin) + fHistogramMin;

				b = 0.0f;
				t = pfHistogram[i];
				glVertex2f(l, b);
				glVertex2f(r, b);
				glVertex2f(r, t);
				glVertex2f(l, t);
			}
			glEnd();

		glPopMatrix();

		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		_DrawString(SZSprintf("%.2e", pcTransFunc->cDomainMin.FGetValue()), 0, 0, false);
		_DrawString(SZSprintf("%.2e", pcTransFunc->cDomainMax.FGetValue()), -1, 0, true);

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
	pcReceiver = &cDefaultReceiver;	
	iEditingChannel = CTransFunc::COLOR_A;
	pcTransFunc = NULL;
	_UpdateFunc = NULL;
	_AddGluiSubwin(GLUI_SUBWINDOW_LEFT);

	szFilename[0] = '\0';
	szDir[0] = '\0';
	szPath[0] = '\0'; 
	pcEditText_Filename = NULL;
	pcEditText_Dir = NULL;
}

CTfUi::~CTfUi(void)
{
}

