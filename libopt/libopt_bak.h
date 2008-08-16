/*

LIBOPT: a libraries used to parse the input arguments

*/
#ifndef __LIB__OPT__H__
#define __LIB__OPT__H__

// specify a flag. If the flag is given the variable *pbParameter is set as bValue (otherwise it is set as the default balue bDefault)
void _OPTAddFlag(const char* szArgName, bool *pbParameter, bool bValue, bool bDefault);

// add an option to specify a flag whose parameter's type are floating point
void _OPTAddFV(const char* szArgName, int iNrOfParameters, float* ppfParameters[]);
void _OPTAdd1F(const char* szArgName, float *pfParameter, float fDefault);
void _OPTAdd2F(const char* szArgName, float *pfParameter0, float *pfParameter1, float fDefault0, float fDefault1);
void _OPTAdd3F(const char* szArgName, float *pfParameter0, float *pfParameter1, float *pfParameter2, float fDefault0, float fDefault1, float fDefault2);
void _OPTAdd4F(const char* szArgName, float *pfParameter0, float *pfParameter1, float *pfParameter2, float *pfParameter3, float fDefault0, float fDefault1, float fDefault2, float fDefault3);

// add an option to specify a flag whose parameter's type are integer
void _OPTAddIV(const char* szArgName, int iNrOfParameters, int* ppiParameters[]);
void _OPTAdd1I(const char* szArgName, int *piParameter, int iDefault);
void _OPTAdd2I(const char* szArgName, int *piParameter0, int *piParameter1, int iDefault0, int iDefault1);
void _OPTAdd3I(const char* szArgName, int *piParameter0, int *piParameter1, int *piParameter2, int iDefault0, int iDefault1, int iDefault2) ;
void _OPTAdd4I(const char* szArgName, int *piParameter0, int *piParameter1, int *piParameter2, int *piParameter3, int iDefault0, int iDefault1, int iDefault2, int iDefault3);

// add an option to specify a flag whose parameter's type are string
// note that there is no memory allocation for the string. 
// All of them will be pointed to the pointers in the argument list
void _OPTAddSV(const char* szArgName, int iNrOfParameters, char** ppszParameters[]);
void _OPTAdd1S(const char* szArgName, char** pszParameter, char* szDefault);
void _OPTAdd2S(const char* szArgName, char** pszParameter0, char** pszParameter1, char* szDefault0, char* szDefault1);

void _OPTInit();
bool BOPTParse(char* argv[], int argc, int iBegin, int *piEnd = NULL);

#endif // __LIB__OPT__H__

/*

$Log: not supported by cvs2svn $

*/

	