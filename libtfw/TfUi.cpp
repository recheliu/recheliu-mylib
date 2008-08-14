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

									// save thie operation to the history
		plcEditHistorys[c]._AddAction(CEditHistory::EDIT_ACTION_DEL, *viKnot);

									// remove this knot
		pcTransFunc->plSplines[c].erase(viKnot);
	}

	pviSelectedKnots[c].clear();
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
							// skip the fist and last knot
		if( 0.0f == viKnot->fX || 1.0f == viKnot->fX )
			continue;

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

	case 127:	// DEL
		_DeleteSelectedKnots(iEditingChannel);
		_Redisplay();
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
/*
				if( false == pviSelectedKnots[iEditingChannel].empty() )
				{
					_DeleteSelectedKnots(iEditingChannel);
				}
				else
				{
											// add one knot
					_AddKnot(iEditingChannel, (float)vriMouseEvent->iX / (float)piViewport[2], (float)vriMouseEvent->iY / (float)piViewport[3]);
					_ClearSelectedKnots(iEditingChannel);
				}
*/
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
	pviSelectedKnots[channel].clear();
	// ADD-By-TLEE 08/14/2008-END
}

void
CTfUi::_GluiFunc(unsigned short usValue)
{
	switch(usValue)
	{
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

	case EDIT_EXIT:
		exit(0);
	}
}

void 
CTfUi::_InitFunc()
{
	assert(pcTransFunc);

#if	0
	GLUI_Listbox * pcListBox = PCGetGluiSubwin()->add_listbox("Channele", &iEditingChannel);
		pcListBox->add_item(CTransFunc::COLOR_R, "Red");
		pcListBox->add_item(CTransFunc::COLOR_G, "Green");
		pcListBox->add_item(CTransFunc::COLOR_B, "Blue");
		pcListBox->add_item(CTransFunc::COLOR_A, "Alpha");
		pcListBox->set_int_val(iEditingChannel);
#else
	
	GLUI_Panel *pcChannelPanel = PCGetGluiSubwin()->add_panel("Channel");
	GLUI_RadioGroup* pcRadioGroup = PCGetGluiSubwin()->add_radiogroup_to_panel(pcChannelPanel, &iEditingChannel, CTransFunc::COLOR_R);
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup, "R");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup, "G");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup, "B");
		PCGetGluiSubwin()->add_radiobutton_to_group(pcRadioGroup, "A");
		pcRadioGroup->set_int_val(iEditingChannel);
#endif

	_AddButton("Redo", EDIT_REDO);
	_AddButton("Undo", EDIT_UNDO);
	_AddButton("Update", EDIT_UPDATE);
	// ADD-BY-LEETEN 08/14/2008-BEGIN
	_AddButton("Clear", EDIT_CLEAR);
	// ADD-BY-LEETEN 08/14/2008-END
	_AddButton("Exit", EDIT_EXIT);
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
			glScalef(1.0f / (float)pfHistogram.num, 1.0f, 1.0f);
			glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			for(int i = 0; i < pfHistogram.num; i++)
			{
				float l, r, b, t;
				l = (float)i;
				r = (float)i + 1;
				b = 0.0f;
				t = pfHistogram[i];
				glVertex2f(l, b);
				glVertex2f(r, b);
				glVertex2f(r, t);
				glVertex2f(l, t);
			}
			glEnd();

		glPopMatrix();


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

}

CTfUi::~CTfUi(void)
{
}

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/14 14:44:02  leeten

[2008/08/14]
1. [FIRST TIME CHECKIN]. This library defines classes for trasnfer functions, including editing and displaying.

Revision 1.1  2008/08/13 21:17:21  leeten

[2008/08/13]
1. First time checkin. This class can create a GLUT window for editing transfer function.


*/
