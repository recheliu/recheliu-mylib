#include "stdafx.h"

#define CHECK_ARGUMENT_PREFIX(szArg)	assert( szArg[0] == '-' );

#define ADD_ENTRY(arg_name, nr_of_parameters, parameter_list, opt_type)\
	{\
		CHECK_ARGUMENT_PREFIX(arg_name);	\
		COptEntry cEntry;	\
		cEntry.eType = opt_type;	\
		cEntry.szArgName = arg_name;	\
		cEntry.iNrOfParameters = nr_of_parameters;	\
		cEntry.ppParameters = (void**)parameter_list;	\
		vcOptTable.push_back(cEntry);	\
	}

#define ADD_VECTOR_PARAMETERS(arg_name, addr_type, value_type, nr_of_parameters, add_entry_func)	\
	{\
		addr_type **parameter_list;	\
		if( nr_of_parameters )	\
			assert( parameter_list = (addr_type**)calloc(sizeof(addr_type*), nr_of_parameters) );	\
		va_list ap; \
		va_start(ap, nr_of_parameters);	\
		for(int p=0; p<nr_of_parameters; p++) \
		{	\
			addr_type *parameter_addr = va_arg(ap, addr_type*);	\
			value_type default_value = va_arg(ap, value_type);	\
			*parameter_addr = (addr_type)default_value;	\
			parameter_list[p] = parameter_addr;	\
		}	\
		va_end(ap);	\
		add_entry_func(arg_name, nr_of_parameters, parameter_list);	\
	}

//////////////////////////////////////////////////////////////////////////
// MOD-BY-LEETEN 04/11/2007-FROM: 
// typedef enum {OPT_TYPE_FLAG, OPT_TYPE_INT, OPT_TYPE_FLOAT, OPT_TYPE_STRING} EOptType;
// MOD-BY-LEETEN 04/11/2007-TO: 
typedef enum {OPT_TYPE_FLAG, OPT_TYPE_INT, OPT_TYPE_FLOAT, OPT_TYPE_STRING, OPT_TYPE_BOOL} EOptType;
// MOD-BY-LEETEN 04/11/2007-END

typedef struct COptEntry 
{
	const char* szArgName;
	EOptType eType;
	int iNrOfParameters;
	void **ppParameters;
	int *piFlagParameter;
	int iFlagValue;

	COptEntry()
	{
		iNrOfParameters = 0;
		ppParameters = NULL;
		piFlagParameter = NULL;
		iFlagValue = OPT_FALSE;
	}
} COptEntry;

vector <COptEntry> vcOptTable;
vector <COptEntry>::iterator pvcOptEntry;

////////////////////////////////////////////////////////////////////////////////////////////
// the function to compare the string
int (*strcmp_func)(const char*, const char*);

int my_stricmp(const char* sz1, const char* sz2)
{
	char c1, c2;
	for(const char *cp1 = sz1, *cp2 = sz2;
		*cp1 && *cp2; cp1++, cp2++) 
	{
		if( *cp1 == *cp2 )
			continue;

		if( ('A' <= *cp1 && *cp1 <= 'Z') || ('A' <= *cp2 && *cp2 <= 'Z') )
		{
			c1 = *cp1;
			if( 'A' <= *cp1 && *cp1 <= 'Z' ) 
				c1 = *cp1 - 'A' + 'a';

			c2 = *cp2;
			if( 'A' <= *cp2 && *cp2 <= 'Z' ) 
				c2 = *cp2 - 'A' + 'a';

			if( c1 == c2 )
				continue;
		}

		if( *cp1 < *cp2 )
			return -1;

		if( *cp1 > *cp2 )
			return 1;
	}
	return 0;
}

// ADD-BY-LEETEN 04/11/2007-BEGIN
// declare a boolean variable
void 
_OPTAddBoolean(const char* szArgName, int *piParameter, int iDefaultValue)
{
	CHECK_ARGUMENT_PREFIX(szArgName);

	*piParameter = iDefaultValue;

	int** ppiParameters = (int**)calloc(sizeof(int**), 1);
	assert(ppiParameters);
	ppiParameters[0] = piParameter;

	ADD_ENTRY(szArgName, 1, ppiParameters, OPT_TYPE_BOOL);
}
// ADD-BY-LEETEN 04/11/2007-END

// ADD-BY-LEETEN 02/22/2008-BEGIN
void 
_OPTAlign(const char* szArgName, const char* szNewArgName)
{
	CHECK_ARGUMENT_PREFIX(szNewArgName);

	// allocate the space to hold the address of the variable
	// search if the argument is already added
	for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
		if( 0 == strcmp_func(szArgName, pvcOptEntry->szArgName) )
			break;

	// if the entry w/ the old argument name is found, add a new one w/ the different name
	if( vcOptTable.end() != pvcOptEntry )
	{
		COptEntry cEntry = *pvcOptEntry;
		cEntry.szArgName = szNewArgName;
		cEntry.ppParameters = (void**)calloc(sizeof(void*), cEntry.iNrOfParameters);
		assert(cEntry.ppParameters);
		memcpy(cEntry.ppParameters, pvcOptEntry->ppParameters, sizeof(void*) * cEntry.iNrOfParameters);
		vcOptTable.push_back(cEntry);	
	} 
	else 
	{
		fprintf(stderr, "LIBOPT:_OPTAlign(): argument %s does not exist\n", szArgName);
	}
}
// ADD-BY-LEETEN 02/22/2008-END

////////////////////////////////////////////////////////////////////////////////////////////
void 
_OPTAddFlag(const char* szArgName, int *piParameter, int iValue, int iDefault)
{
	CHECK_ARGUMENT_PREFIX(szArgName);

	// set the default value
	*piParameter = iDefault;

	// allocate the space to hold the address of the variable
	// search if the argument is already added
	for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
		if( 0 == strcmp_func(szArgName, pvcOptEntry->szArgName) )
			break;

	// no such an entry is found, so add a new one
	if( vcOptTable.end() == pvcOptEntry )
	{
		int** ppiParameters = NULL;
		ADD_ENTRY(szArgName, 0, ppiParameters, OPT_TYPE_FLAG);

		pvcOptEntry = vcOptTable.end(); 
		pvcOptEntry--;
	} 

	pvcOptEntry->piFlagParameter = piParameter;
	pvcOptEntry->iFlagValue = iValue;
}

///////////////////////////////////////////////////////////////////////////////////////////
void _OPTAddIV(const char* szArgName, int iNrOfParameters, int* ppiParameters[])
{
	ADD_ENTRY(szArgName, iNrOfParameters, ppiParameters, OPT_TYPE_INT);
}

void 
_OPTAddIntegerVector(const char *szArgName, int iNrOfParameters, ...)
{
	ADD_VECTOR_PARAMETERS(szArgName, int, int, iNrOfParameters, _OPTAddIV);
}

////////////////////////////////////////////////////////////////////////////////////
void _OPTAddFV(const char* szArgName, int iNrOfParameters, float* ppfParameters[])
{
	ADD_ENTRY(szArgName, iNrOfParameters, ppfParameters, OPT_TYPE_FLOAT);
}

void 
_OPTAddFloatingVector(const char *szArgName, int iNrOfParameters, ...)
{
	// I guess that when pass a value whose type is float, the 
	// compilier automaticall convert it into double. As a result,
	// the value's type should be double 
	ADD_VECTOR_PARAMETERS(szArgName, float, double, iNrOfParameters, _OPTAddFV);
}

///////////////////////////////////////////////////////////////////////////////////
void 
_OPTAddSV(const char* szArgName, int iNrOfParameters, char** ppszParameters[])
{
	ADD_ENTRY(szArgName, iNrOfParameters, ppszParameters, OPT_TYPE_STRING);
}

void 
_OPTAddStringVector(const char *szArgName, int iNrOfParameters, ...)
{
	ADD_VECTOR_PARAMETERS(szArgName, char*, char*, iNrOfParameters, _OPTAddSV);
}

///////////////////////////////////////////////////////////////////////////////////
// initialize the data structure

void _OPTInit(bool bCaseInsentive)
{
	if( bCaseInsentive )
		strcmp_func = my_stricmp;
	else
		strcmp_func = strcmp;

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
			if( 0 == strcmp_func(argv[iA], pvcOptEntry->szArgName) )
			{
				// check the number of parameters
				if( iA+pvcOptEntry->iNrOfParameters >= argc )
				{
					bResult = false;
					break;
				}
				// set the parameters
				for(int iP=0; iP<pvcOptEntry->iNrOfParameters; iP++)
					switch( pvcOptEntry->eType )
					{
					// ADD-BY-LEETEN 04/11/2007-BEGIN
					case OPT_TYPE_BOOL:
						*(int*)pvcOptEntry->ppParameters[iP] = (!strcmp_func(argv[++iA], "true"))?OPT_TRUE:OPT_FALSE;	
						break;
					// ADD-BY-LEETEN 04/11/2007-END
					case OPT_TYPE_INT:	
						*(int*)pvcOptEntry->ppParameters[iP] = atoi(argv[++iA]);	
						break;
					case OPT_TYPE_FLOAT:
						*(float*)pvcOptEntry->ppParameters[iP] = (float)atof(argv[++iA]);	
						break;
					case OPT_TYPE_STRING:
						*(char**)pvcOptEntry->ppParameters[iP] = argv[++iA];
						break;
					}

				// assign the flag if it is set
				if( pvcOptEntry->piFlagParameter )
					*pvcOptEntry->piFlagParameter = pvcOptEntry->iFlagValue;
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
Revision 1.3  2007/04/12 19:03:02  leeten

[04/12/2007]
1. [ADDED] Define new type of option: OPT_TYPE_BOOLEAN and new function OptAddBoolean().

Revision 1.2  2007/02/20 18:01:35  leeten

[02/20/2007]
1. Debug the error to comparte the string.


*/
