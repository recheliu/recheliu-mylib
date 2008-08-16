#include "stdafx.h"

typedef enum {OPT_TYPE_ARGUMENT_FLAG, OPT_TYPE_ARGUMENT_INT, OPT_TYPE_ARGUMENT_FLOAT, OPT_TYPE_ARGUMENT_STRING} EOptType;

typedef struct COptEntry {
	const char* szArgName;
	EOptType eType;
	int iNrOfParameters;
	void **ppParameters;
	bool bFlagValue;
} COptEntry;

vector <COptEntry> vcOptTable;
vector <COptEntry>::iterator pvcOptEntry;

////////////////////////////////////////////////////////////////////////////////////////////
void 
_OPTAddFlag(const char* szArgName, bool *pbParameter, bool bValue, bool bDefault)
{
	assert( szArgName[0] == '-' );

	*pbParameter = bDefault;

	COptEntry cEntry;
	cEntry.eType = OPT_TYPE_ARGUMENT_FLAG;
	cEntry.szArgName = szArgName;
	cEntry.iNrOfParameters = 1;
	assert( cEntry.ppParameters = (void**)calloc(sizeof(bool*), 1) );
	cEntry.ppParameters[0] = pbParameter;
	cEntry.bFlagValue = bValue;

	vcOptTable.push_back(cEntry);
}

///////////////////////////////////////////////////////////////////////////////////////////
void _OPTAddIV(const char* szArgName, int iNrOfParameters, int* ppiParameters[])
{
	assert( szArgName[0] == '-' );

	COptEntry cEntry;
	cEntry.eType = OPT_TYPE_ARGUMENT_INT;
	cEntry.szArgName = szArgName;
	cEntry.iNrOfParameters = iNrOfParameters;
	cEntry.ppParameters = (void**)ppiParameters;

	vcOptTable.push_back(cEntry);
}

void 
_OPTAdd1I(const char* szArgName, int *piParameter, int iDefault)
{
	int **ppiParameters;
	assert( ppiParameters = (int**)calloc(sizeof(int*), 1) );
	*piParameter = iDefault;	ppiParameters[0] = piParameter;
	_OPTAddIV(szArgName, 1, ppiParameters);
}

void _OPTAdd2I(const char* szArgName, 
	int *piParameter0, int *piParameter1, 
	int iDefault0, int iDefault1) 
{
	int **ppiParameters;
	assert( ppiParameters = (int**)calloc(sizeof(int*), 2) );
	*piParameter0 = iDefault0;	ppiParameters[0] = piParameter0;
	*piParameter1 = iDefault1;	ppiParameters[1] = piParameter1;
	_OPTAddIV(szArgName, 2, ppiParameters);
}

void _OPTAdd3I(const char* szArgName, 
	int *piParameter0, int *piParameter1, int *piParameter2, 
	int iDefault0, int iDefault1, int iDefault2) 
{
	int **ppiParameters;
	assert( ppiParameters = (int**)calloc(sizeof(int*), 3) );
	*piParameter0 = iDefault0;	ppiParameters[0] = piParameter0;
	*piParameter1 = iDefault1;	ppiParameters[1] = piParameter1;
	*piParameter2 = iDefault2;	ppiParameters[2] = piParameter2;
	_OPTAddIV(szArgName, 3, ppiParameters);
}

void _OPTAdd4I(const char* szArgName, 
	int *piParameter0, int *piParameter1, int *piParameter2, int *piParameter3, 
	int iDefault0, int iDefault1, int iDefault2, int iDefault3) 
{
	int **ppiParameters;
	assert( ppiParameters = (int**)calloc(sizeof(int*), 4) );
	*piParameter0 = iDefault0;	ppiParameters[0] = piParameter0;
	*piParameter1 = iDefault1;	ppiParameters[1] = piParameter1;
	*piParameter2 = iDefault2;	ppiParameters[2] = piParameter2;
	*piParameter3 = iDefault3;	ppiParameters[3] = piParameter3;
	_OPTAddIV(szArgName, 4, ppiParameters);
}

////////////////////////////////////////////////////////////////////////////////////
void _OPTAddFV(const char* szArgName, int iNrOfParameters, float* ppfParameters[])
{
	assert( szArgName[0] == '-' );

	COptEntry cEntry;
	cEntry.eType = OPT_TYPE_ARGUMENT_FLOAT;
	cEntry.szArgName = szArgName;
	cEntry.iNrOfParameters = iNrOfParameters;
	cEntry.ppParameters = (void**)ppfParameters;

	vcOptTable.push_back(cEntry);
}

void _OPTAdd1F(const char* szArgName, float *pfParameter, float fDefault)
{
	float **ppfParameters;
	assert( ppfParameters = (float**)calloc(sizeof(float*), 1) );
	*pfParameter = fDefault;	ppfParameters[0] = pfParameter;
	_OPTAddFV(szArgName, 1, ppfParameters);
}

void _OPTAdd2F(const char* szArgName, 
	float *pfParameter0, float *pfParameter1, 
	float fDefault0, float fDefault1) 
{
	float **ppfParameters;
	assert( ppfParameters = (float**)calloc(sizeof(float*), 2) );
	*pfParameter0 = fDefault0;	ppfParameters[0] = pfParameter0;
	*pfParameter1 = fDefault1;	ppfParameters[1] = pfParameter1;
	_OPTAddFV(szArgName, 2, ppfParameters);
}

void _OPTAdd3F(const char* szArgName, 
	float *pfParameter0, float *pfParameter1, float *pfParameter2, 
	float fDefault0, float fDefault1, float fDefault2)
{
	float **ppfParameters;
	assert( ppfParameters = (float**)calloc(sizeof(float*), 3) );
	*pfParameter0 = fDefault0;	ppfParameters[0] = pfParameter0;
	*pfParameter1 = fDefault1;	ppfParameters[1] = pfParameter1;
	*pfParameter2 = fDefault2;	ppfParameters[2] = pfParameter2;
	_OPTAddFV(szArgName, 3, ppfParameters);
}

void _OPTAdd4F(const char* szArgName, 
	float *pfParameter0, float *pfParameter1, float *pfParameter2, float *pfParameter3, 
	float fDefault0, float fDefault1, float fDefault2, float fDefault3)
{
	float **ppfParameters;
	assert( ppfParameters = (float**)calloc(sizeof(float*), 4) );
	*pfParameter0 = fDefault0;	ppfParameters[0] = pfParameter0;
	*pfParameter1 = fDefault1;	ppfParameters[1] = pfParameter1;
	*pfParameter2 = fDefault2;	ppfParameters[2] = pfParameter2;
	*pfParameter3 = fDefault3;	ppfParameters[3] = pfParameter3;
	_OPTAddFV(szArgName, 4, ppfParameters);
}

///////////////////////////////////////////////////////////////////////////////////
void 
_OPTAddSV(const char* szArgName, int iNrOfParameters, char** ppszParameters[])
{
}

void 
_OPTAdd1S(const char* szArgName, char** pszParameter, char* szDefault)
{
	char*** ppszParameters;
	assert( ppszParameters = (char***)calloc(sizeof(char**), 1) );
	*pszParameter = szDefault;	ppszParameters[0] = pszParameter;
	_OPTAddSV(szArgName, 1, ppszParameters);
}

void 
_OPTAdd2S(const char* szArgName, char** pszParameter0, char** pszParameter1, char* szDefault0, char* szDefault1)
{
	char*** ppszParameters;
	assert( ppszParameters = (char***)calloc(sizeof(char**), 2) );
	*pszParameter0 = szDefault0;	ppszParameters[0] = pszParameter0;
	*pszParameter1 = szDefault0;	ppszParameters[1] = pszParameter1;
	_OPTAddSV(szArgName, 2, ppszParameters);
}

///////////////////////////////////////////////////////////////////////////////////
// initialize the data structure
void _OPTInit()
{
	vcOptTable.clear();
}

bool BOPTParse(char* argv[], int argc, int iBegin, int *piEnd)
{
	bool bResult;
	int iA;

	bResult = true;
	for(iA = iBegin; iA<argc; iA++)
	{
		for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
			if( !strcmp(argv[iA], pvcOptEntry->szArgName) )
			{
				if( iA+pvcOptEntry->iNrOfParameters >= argc )
				{
					bResult = false;
					break;
				}

				if( OPT_TYPE_ARGUMENT_FLAG == pvcOptEntry->eType )
				{
					*(bool*)pvcOptEntry->ppParameters[0] = pvcOptEntry->bFlagValue;
				}
				else
				{
					for(int iP=0; iP<pvcOptEntry->iNrOfParameters; iP++)
						switch( pvcOptEntry->eType )
						{
						case OPT_TYPE_ARGUMENT_INT:	
							*(int*)pvcOptEntry->ppParameters[iP] = atoi(argv[++iA]);	
							break;
						case OPT_TYPE_ARGUMENT_FLOAT:
							*(float*)pvcOptEntry->ppParameters[iP] = (float)atof(argv[++iA]);	
							break;
						case OPT_TYPE_ARGUMENT_STRING:
							*(char**)pvcOptEntry->ppParameters[iP] = argv[++iA];	
							break;
						}
				}
				break;
			}

		if( pvcOptEntry == vcOptTable.end() )
			break;
	}

	if( piEnd )
		*piEnd = iA;

	// clear the entry 
	for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
	{
		if( pvcOptEntry->ppParameters )
		{
			delete [] pvcOptEntry->ppParameters;
			pvcOptEntry->ppParameters = NULL;
		}
	}

	return bResult;
}

/*

$Log: not supported by cvs2svn $

*/
