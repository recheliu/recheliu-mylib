#pragma once
#include "GlutWin.h"

	// ADD-BY-TLEE 08/14/2008-BEGIN
	#include "TransFunc.h"
	#include "libbuf.h"
	// ADD-BY-TLEE 08/14/2008-END

class CTfWin :
	public CGlutWin
{
	// inhered interfaces
	void _DisplayFunc();
	void _InitFunc();

	// MOD-BY-TLEE 08/14/2008-FROM:
		// GLuint iLid;
	// TO:
	CTransFunc *pcTransFunc;
	TBuffer<float> pfColorMap;
	int iNrOfEntries;
	// MOD-BY-TLEE 08/14/2008-END

public:
	// MOD-BY-TLEE 08/14/2008-FROM:
		// void _SetTransferFunc(const float *pfTf, int iNrOfTfEntries);
	// TO:
	void _SetTransFunc(CTransFunc *pcTransFunc);
	void _SetNrOfEntries(int iNrOfEntries);
	// MOD-BY-TLEE 08/14/2008-END

public:
	CTfWin(void);
public:
	~CTfWin(void);
};

/*

$Log: not supported by cvs2svn $
Revision 1.1.1.1  2008/08/12 16:58:49  leeten

[2008/08/12]
1. First time checkin. This is my DVR renderer for 3D regular volume. For testing, the dataset HIPIP is checkin too.


*/

