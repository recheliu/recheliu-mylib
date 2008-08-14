/***********************************************

CTfUi: 

This file defines a class for editing transfer functions. 
It is a subclass of CGlutWin, a class for creating GLUT/GLUI-based
display window. 

The proecduer to use this class is as below:

1. declare a object of class CTfUi.
2. Set up the editing transfunc function of class CTransFunc via method _SetTransFunc. 
3. Invoke function CGlutWin::_AddWin() to create the window. If the transfer function is not
	set yet, this application will be terminated.
4. (Optional) Setup a update callback via method _SetUpdateFunc. This is useful to acknowledge other objects
	that the transfer function is updated.
5. (Optional) A histogram can be input vai method _SetHistogramAsBackground to draw a histogram 
	as the background. The occurences in the hostgoram are assumed to be in [0, 1].

************************************************/

#include <list>

#pragma once
#include "glutwin.h"

	#include "libbuf.h"

	// ADD-BY-LEETEN 08/14/2008-BEGIN
	#include "knot.h"
	#include "TransFunc.h"
	// ADD-BY-LEETEN 08/14/2008-END

class CTfUi :
	public CGlutWin
{
	///////////////////////////////////////////////
public:
	enum
	{
		EDIT_REDO,
		EDIT_UNDO,
		EDIT_CLEAR,		// clear the spline
		EDIT_RESTORE,
		EDIT_UPDATE,
		EDIT_EXIT
	};

	///////////////////////////////////////////////
protected:
	TBuffer<float> pfHistogram;

public:
	void _SetHistogramAsBackground(int iNrOfEntries, float pfHistogram[])
	{
		this->pfHistogram.alloc(iNrOfEntries);
		memcpy(&this->pfHistogram[0], pfHistogram, sizeof(pfHistogram[0]) * iNrOfEntries);
	}

	///////////////////////////////////////////////
protected:
													// the channel that is being edited
	int iEditingChannel;

	///////////////////////////////////////////////
protected:
						// define the splines
	CTransFunc *pcTransFunc;

	void 
		// MOD-BY-TLEE 08/14/2008-FROM:
			// CTfUi::_PlotSpline(int c, bool bPlotKnots = false);
		// TO:
		CTfUi::_PlotSpline(int c, bool bEnhance = false);
		// MOD-BY-TLEE 08/14/2008-END

	vector<list<CKnot>::iterator>	
		pviSelectedKnots[CTransFunc::NR_OF_COLORS];				// the iterators pointed to the selected knots

													// select knots in the region
	void _SelectKnots(int c, float l, float r, float b, float t);
	void _ClearSelectedKnots(int c);				// clear the selected knots in the specified channel
	void _DeleteSelectedKnots(int c);				// delete the selected knots from the spline in the specified channel

	void _Undo(int c);
	void _Redo(int c);

	void _AddKnot(int c, float x, float y, bool bAddToHistory = true);			// add a new knot to the spline
	void _DelKnot(int c, float x, float y, bool bAddToHistory = true);			// delete a knot in the specified spline

public:
	void _SetTransFunc(CTransFunc* pcTransFunc)
	{
		this->pcTransFunc = pcTransFunc;
	}

	// ADD-BY-LEETEN 2008/08/14-BEGIN
	void _ClearChannel(int channel);
	// ADD-BY-LEETEN 2008/08/14-END

	///////////////////////////////////////////////
protected:						

	class CEditHistory {
	public:
		enum {
			EDIT_ACTION_MARKER,
			EDIT_ACTION_DEL,
			EDIT_ACTION_ADD
		};

		struct CEditAction {
			int iType;
			CKnot cKnot;
		};

		list<CEditAction>::iterator liCurrentAction;
		list<CEditAction> lcEditActions;

		CEditHistory()
		{
			liCurrentAction = lcEditActions.end();
		}

		void 
		_AddMarker()
		{
			lcEditActions.erase(liCurrentAction, lcEditActions.end());

			CEditAction cNewAction;
			cNewAction.iType = EDIT_ACTION_MARKER;
			lcEditActions.push_back(cNewAction);

			liCurrentAction = lcEditActions.end();
		}

		void 
		_AddAction(int iType, CKnot& cKnot)
		{
			CEditAction cNewAction;
			cNewAction.iType = iType;
			cNewAction.cKnot = cKnot;
			lcEditActions.push_back(cNewAction);

			liCurrentAction = lcEditActions.end();
		}

		void
		_CurrentAction(int& iType, CKnot& cKnot)
		{
			if( lcEditActions.end() == liCurrentAction || EDIT_ACTION_MARKER == liCurrentAction->iType )
			{
				iType = EDIT_ACTION_MARKER;
			}
			else
			{
				iType = liCurrentAction->iType;
				cKnot = liCurrentAction->cKnot;
			}
		}

		bool 
		BIsBegin()	{
			return ( lcEditActions.begin() == liCurrentAction ) ? true : false;
		}

		bool 
		BIsEnd()
		{
			return ( lcEditActions.end() == liCurrentAction ) ? true : false;
		}

		void
		_PrevAction()
		{
			if( false == BIsBegin() )
				liCurrentAction--;
		}

		void
		_NextAction()
		{
			if( false == BIsEnd() )
				liCurrentAction++;
		}

	} plcEditHistorys[CTransFunc::NR_OF_COLORS];

	///////////////////////////////////////////////
protected:						

	struct CMouseEvent {
		int iButton;
		int iState;
		int iX;
		int iY;
	};

	vector<CMouseEvent> vcMouseEvents;

	///////////////////////////////////////////////
protected:
	void (*_UpdateFunc)(void);

public:
	// DEL-BY-LEETEN 08/14/2008-BEGIN
		// void _CreateTransferFunc(int iNrOfEntries, float pfTransferFunc[]);
	// DEL-BY-LEETEN 08/14/2008-END

	void _SetUpdateFunc(void (*_UpdateFunc)(void))	
	{
		this->_UpdateFunc = _UpdateFunc;
	}

public:						// callbacks
	void _InitFunc();
	void _DisplayFunc();

	void _MotionCB(int x, int y);
	void _MouseCB(int button, int state, int x, int y);
	void _IdleCB();

	void _KeyboardCB(unsigned char key, int x, int y);

	void _GluiFunc(unsigned short usValue);

	///////////////////////////////////////////////
public:

	CTfUi(void);
public:

	virtual ~CTfUi(void);
};

/*

$Log: not supported by cvs2svn $
Revision 1.1  2008/08/13 21:17:21  leeten

[2008/08/13]
1. First time checkin. This class can create a GLUT window for editing transfer function.


*/
