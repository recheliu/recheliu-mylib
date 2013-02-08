#pragma once

#include <vector_functions.h>
#include <NrrdIO.h>

#include "libclip/ClipVolume.h"
#include "libdvr2/DvrWin2.h"
#include "libtfw/TfWin.h"
#include "libtfw/TfUi.h"	
#include "libtfw/TransFunc.h"

struct 
CDvrSuiteWin:	
	virtual	public CTfUi::CReceiver,
	virtual public CClipVolume,		
	virtual public CDvrWin2
{
protected:
	enum {
		// event for GLUI
		GLUI_EVENT_BASE = 0x901,
		GLUI_EVENTS_MAX_ID	
	};

	enum {
		DEFAULT_NR_OF_HISTGROAM_BINS = 32,
		DEFAULT_NR_OF_TF_ENTRIES = 128,
		_RESERVED
	};


	//////////////////////////////////////////////////////
	// fields for volume rendering
	Nrrd *nin;

	double dValueMin;
	double dValueMax;

	// histogram
	vector<float> vfHist;
	float fMaxCount; // the maximal count of the histogram	

	GLuint pidRayIntegral;

	template<typename T>
	void
	_ConvertDataToTexture
	(
		Nrrd *nin
	);
public:
	vector<float4> vf4TransFunc;
	CTransFunc cTransFunc;

	CTfWin	cTfWin;
	CTfUi	cTfUi;

	virtual 
	void
	_Receive(
		CTransFunc *pcTransFunc
	);

	void 
	_CreateTfWins
	(
	);

	void
	_LoadData
	(
		char* szFilepath,
		void* _Reserved = NULL
	);

	//! Set the #Entries of the histogram. 
	/* 
	This also influence the resolution of the transfer function. 
	It must be called before _LoadData, or the #bins will be inconsistent
	*/
	void
	_SetHistogramSize
	(
		size_t uNrOfBins = DEFAULT_NR_OF_HISTGROAM_BINS
	);

	void
	_SetTfSize
	(
		size_t uNrOfTfEntries = DEFAULT_NR_OF_TF_ENTRIES
	);

	void _InitFunc();
	void _ReshapeFunc(int w, int h);

	////////// volume rendering methods //////////////
	void _BeginDisplay();
	void _EndDisplay();
	void _RenderSlab(
		int iSlab, int iNrOfSlabs,
		double pdModelviewMatrix[], double pdProjectionMatrix[], int piViewport[],
		double dMinX, double dMaxX, 
		double dMinY, double dMaxY, 
		double dMinZ, double dMaxZ);

	//! The method to setup advanved OpenGL features
	/*!
	This method is defined such that it can be called after _InitFunc();
	*/
	void _InitGl();

	CDvrSuiteWin(void);
	~CDvrSuiteWin(void);
};

