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
#include "GlutWin.h"

	#include "libbuf.h"

	#include "knot.h"
	#include "TransFunc.h"

	#include "FloatValue.h"

class CTfUi :
	public CGlutWin
{
	///////////////////////////////////////////////
public:
	class CReceiver
	{
	public:
		virtual void _Receive(CTransFunc *pcTransFunc){}
	};
	// a default receiver. This is mainly for the compilation purpose.
	CReceiver cDefaultReceiver;	

	// pointer to the receiver.
	CReceiver *pcReceiver;
	
	// a function to setup the receivier.
	void
	_SetReceiver(CReceiver *pcReceiver);

	enum
	{
		EDIT_REDO,
		EDIT_UNDO,
		EDIT_CLEAR,		// clear the spline
		EDIT_RESTORE,
		EDIT_UPDATE,
		EDIT_DELETE,

						// define new constants
		FILE_OPEN,		// open file of the TF
		FILE_SAVE,		// save file of the TF
		FILE_DIR,		// specify the dir. of the TF
		FILE_FILENAME,	// specify the filename of the TF

		EDIT_EXIT
	};

	///////////////////////////////////////////////
protected:
	TBuffer<float> pfHistogram;

	CFloatValue cHistogramMin;
	CFloatValue cHistogramMax;



public:
	void _SetHistogramAsBackground(float *pfHistogram, int iNrOfEntries, double fHistogramMin, double fHistogramMax)
	{
		if( pfHistogram )
		{
														// store the histogram 
			this->pfHistogram.alloc(iNrOfEntries);
			memcpy(&this->pfHistogram[0], pfHistogram, sizeof(pfHistogram[0]) * iNrOfEntries);
		}

													// store the range
		this->cHistogramMin._SetValue(fHistogramMin);
		this->cHistogramMax._SetValue(fHistogramMax);
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
		_PlotSpline(int c, bool bEnhance = false);

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

	void _ClearChannel(int channel);

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

											// thie method clear the history and reset the pointer to current action
		void
		_Clear()
		{
			lcEditActions.clear();
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
	void _SetUpdateFunc(void (*_UpdateFunc)(void))	
	{
		this->_UpdateFunc = _UpdateFunc;
	}

									// variables and methods for specifying the path of the TF

protected:						
	char szFilename[1024+1];		// a buffer for the EditText control to define the filename
	char szDir[1024+1];				// a buffer for the EditText control to define the path
	char szPath[2048+1];			// a buffer to store the combination of the path and filename

									// struct for GLUI control
	GLUI_EditText	*pcEditText_Filename;
	GLUI_EditText	*pcEditText_Dir;

public:

	void					// combine the filename and dir to form the path. The path is stored in szPath
	_GetPath()
	{
		strcpy(szPath, pcEditText_Dir->get_text());
		#if defined(WIN32)
			strcat(szPath, "\\\\");
		#else
			strcat(szPath, "//");
		#endif
		strcat(szPath, pcEditText_Filename->get_text());
	}

							// set the directory to be displayed in the edit text
	void 
	_SetDir(const char *szDir)
	{
		if( pcEditText_Dir )
			pcEditText_Dir->set_text(szDir);
		else
		{
			_AddToLog("pcEditText_Dir hasn't been created yet.");
		}

	}

							// set the filename to be displayed in the edit text
	void 
	_SetFilename(const char *szFilename)
	{
		if( pcEditText_Filename )
			pcEditText_Filename->set_text(szFilename);
		else
		{
			_AddToLog("pcEditText_Filename hasn't been created yet.");
		}
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

