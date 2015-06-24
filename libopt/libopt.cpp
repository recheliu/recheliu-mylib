	#include <vector>
	#include <iostream>	

	using namespace std;

	#include <assert.h>
	#include <stdarg.h>

	#include <stdio.h>
	#include <stdlib.h>
	#include <memory.h>
	#include <string.h>

	#include "libopt.h"

#define CHECK_ARGUMENT_PREFIX(szArg)	assert( szArg[0] == '-' );

#define ADD_ENTRY(arg_name, nr_of_parameters, parameter_list, opt_type)\
	{\
		CHECK_ARGUMENT_PREFIX(arg_name);	\
		COptEntry cEntry;	\
		cEntry.eType = opt_type;	\
		cEntry.szArgName = arg_name;	\
		cEntry.iNrOfParameters = nr_of_parameters;	\
		cEntry.ppParameters = (void**)parameter_list;	\
		cEntry.szComment = "";	\
		cEntry.pszParamNames = (char**)calloc(sizeof(char*), nr_of_parameters);	\
		for(int p = 0; p < nr_of_parameters; p++)		\
		{	\
			char szName[16];							\
			sprintf(szName, "var%02d", p);				\
			cEntry.pszParamNames[p] = strdup(szName);	\
		}	\
		vcOptTable.push_back(cEntry);	\
	}

	#define ADD_VECTOR_PARAMETERS(arg_name, addr_type, value_type, nr_of_parameters, add_entry_func)	\
	{\
		addr_type **parameter_list;	\
		if( nr_of_parameters )	\
		{	\
			parameter_list = (addr_type**)calloc(sizeof(addr_type*), nr_of_parameters);	\
			assert( parameter_list );	\
		}	\
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

	#define ADD_VECTOR_PARAM_WITH_NAMES(arg_name, addr_type, value_type, nr_of_parameters, add_entry_func)	\
	{	\
		addr_type **parameter_list;									\
		char* *name_list;											\
		if( nr_of_parameters )										\
		{															\
			parameter_list = (addr_type**)calloc(sizeof(addr_type*), nr_of_parameters);	\
			assert( parameter_list );								\
			name_list = (char**)calloc(sizeof(addr_type*), nr_of_parameters);	\
			assert( name_list );									\
		}															\
		va_list ap;													\
		va_start(ap, nr_of_parameters);								\
		for(int p=0; p<nr_of_parameters; p++)						\
		{															\
			addr_type *parameter_addr = va_arg(ap, addr_type*);		\
			char* name = va_arg(ap, char*);							\
			value_type default_value = va_arg(ap, value_type);		\
			*parameter_addr = (addr_type)default_value;				\
			parameter_list[p] = parameter_addr;						\
			name_list[p] = strdup(name);									\
		}															\
		va_end(ap);													\
		add_entry_func(arg_name, nr_of_parameters, parameter_list);	\
		\
		vector<COptEntry>::reverse_iterator rivcOptEntry = vcOptTable.rbegin();	\
		if( rivcOptEntry->pszParamNames )	\
		{	\
			for(int p = 0; p < rivcOptEntry->iNrOfParameters; p++)	\
				free(rivcOptEntry->pszParamNames[p]);				\
			free(rivcOptEntry->pszParamNames);						\
		}	\
		rivcOptEntry->pszParamNames = name_list;					\
	}

//////////////////////////////////////////////////////////////////////////
// typedef enum {OPT_TYPE_FLAG, OPT_TYPE_INT, OPT_TYPE_FLOAT, OPT_TYPE_STRING, OPT_TYPE_BOOL} EOptType;
typedef enum {
	OPT_TYPE_FLAG, 
	OPT_TYPE_INT, 
	OPT_TYPE_FLOAT, 
	OPT_TYPE_STRING, 
	OPT_TYPE_BOOL,
	OPT_TYPE_ENUM,
} EOptType;

typedef struct CEnumEntry 
{
	const char *szEnumName;
	int iEnumValue;
}CEnumEntry ;

typedef struct COptEntry 
{
	const char* szArgName;
	EOptType eType;
	int iNrOfParameters;
	char* *pszParamNames;
	void **ppParameters;
	int *piFlagParameter;
	int iFlagValue;

	int iNrOfEnumValues;
	vector<CEnumEntry> vcEnumTable;

	const char *szComment;

	COptEntry()
	{
		iNrOfParameters = 0;
		pszParamNames = NULL;
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
	const char *cp1 = sz1, *cp2 = sz2;
	for(;
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

		if( *cp1 !=  *cp2 )
			break;
	}

	if( *cp1 < *cp2 )
		return -1;

	if( *cp1 > *cp2 )
		return 1;

	return 0;
}

void
_AddParamName(
  char* pszNames[]
)
{
	vector<COptEntry>::reverse_iterator rivcOptEntry = vcOptTable.rbegin();
	if( rivcOptEntry != vcOptTable.rend() )
	{
		if( rivcOptEntry->pszParamNames )
		{
			for(int p = 0; p < rivcOptEntry->iNrOfParameters; p++)
				free(rivcOptEntry->pszParamNames[p]);
			free(rivcOptEntry->pszParamNames);
		}
		rivcOptEntry->pszParamNames = pszNames;
	}
}

// declare a boolean variable
void 
_OPTAddBoolean(const char* szArgName, int *piParameter, int iDefaultValue)
{
	_OPTAddEnum(szArgName, piParameter, iDefaultValue, 
		4,
		"0",		OPT_FALSE,
		"false",	OPT_FALSE,
		"1",		OPT_TRUE,
		"true",		OPT_TRUE
	);
	vector<COptEntry>::reverse_iterator rivcOptEntry = vcOptTable.rbegin();
	rivcOptEntry->eType = OPT_TYPE_BOOL;
}
	
void 
_OPTAddNames(const char* szArgName, int iNrOfParameters, ...)
{
	CHECK_ARGUMENT_PREFIX(szArgName);

	// allocate the space to hold the address of the variable
	// search if the argument is already added
	for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
		if( 0 == strcmp_func(szArgName, pvcOptEntry->szArgName) )
			break;

	// if the entry w/ the old argument name is found, add a new one w/ the different name
	if( vcOptTable.end() != pvcOptEntry )
	{
		assert(pvcOptEntry->iNrOfParameters == iNrOfParameters);
		COptEntry cEntry = *pvcOptEntry;
		va_list ap; 
		va_start(ap, iNrOfParameters);
		for(int p = 0; p < pvcOptEntry ->iNrOfParameters; p++) 
		{	
			char* szName = va_arg(ap, char*);	
			free(pvcOptEntry->pszParamNames[p]);
			pvcOptEntry->pszParamNames[p] = strdup(szName);
		}	
		va_end(ap);	
	} 
	else 
	{
		fprintf(stderr, "%s: argument %s does not exist.\n", __FUNCTION__, szArgName);
	}
}

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
		cEntry.szComment = pvcOptEntry->szComment;
		vcOptTable.push_back(cEntry);	
	} 
	else 
	{
		fprintf(stderr, "LIBOPT:_OPTAlign(): argument %s does not exist\n", szArgName);
	}
}

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
void 
_OPTAddEnum(const char* szArgName, int *piParameter, int iDefault, int iNrOfEnumValues, ...)
{
	CHECK_ARGUMENT_PREFIX(szArgName);

	*piParameter = iDefault;

	int** ppiParameters = (int**)calloc(sizeof(int**), 1);
	assert(ppiParameters);
	ppiParameters[0] = piParameter;

	ADD_ENTRY(szArgName, 1, ppiParameters, OPT_TYPE_ENUM);

	vector<COptEntry>::reverse_iterator rivcOptEntry = vcOptTable.rbegin();
	rivcOptEntry->iNrOfEnumValues = iNrOfEnumValues;

	va_list ap; 
	va_start(ap, iNrOfEnumValues);	
	for(int e=0; e<iNrOfEnumValues; e++) 
	{	
		CEnumEntry cEnum;
		cEnum.szEnumName = va_arg(ap, const char*);
		cEnum.iEnumValue = va_arg(ap, int);

		rivcOptEntry->vcEnumTable.push_back(cEnum);
	}	
	va_end(ap);	
}

template<typename T>
void
_PrintOptionUsage(
	const vector<COptEntry>::iterator& vicOpt, 
	const char* szType,	// A high-level name of the type for printing
	bool bIsPrintingValue
)	
{
	fprintf(stderr, "%s ", vicOpt->szArgName);
	for(int e = 0; e < vicOpt->iNrOfParameters; e++)
	{									
		fprintf(stderr, "<%s> ", vicOpt->pszParamNames[e]);
	}
	fprintf(stderr, "\n");
	for(int e = 0; e < vicOpt->iNrOfParameters; e++)
	{
		fprintf(stderr, "\t%s <%s>\t", szType, vicOpt->pszParamNames[e]);
		if( bIsPrintingValue )
		{
			cerr<<"= ";
			if( OPT_TYPE_STRING == vicOpt->eType && NULL == *((char*)vicOpt->ppParameters[e]) )
				cerr<<"NULL";
			else
			if( OPT_TYPE_BOOL == vicOpt->eType )
				cerr<<(*((T*)vicOpt->ppParameters[e])?"true":"false");
			else
				cerr<<*((T*)vicOpt->ppParameters[e]);
		}
		cerr<<endl;
	}
}
	
void
_OptPrintComment()
{
	for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
	{
		switch( pvcOptEntry->eType )
		{
		case OPT_TYPE_FLAG:
			// PRINT_OPTION_USAGE(pvcOptEntry, bool, "flag", 0, false);
			_PrintOptionUsage<bool>(pvcOptEntry, "", false);
			break;

		case OPT_TYPE_BOOL:
			// PRINT_OPTION_USAGE(pvcOptEntry, bool, "bool", 1, true);
			_PrintOptionUsage<bool>(pvcOptEntry, "bool", true);
			break;

		case OPT_TYPE_INT:	
			// PRINT_OPTION_USAGE(pvcOptEntry, int, "integer", pvcOptEntry->iNrOfParameters, true);
			_PrintOptionUsage<int>(pvcOptEntry, "integer", true);
			break;

		case OPT_TYPE_FLOAT:
			// PRINT_OPTION_USAGE(pvcOptEntry, float, "float", pvcOptEntry->iNrOfParameters, true);
			_PrintOptionUsage<float>(pvcOptEntry, "float", true);
			break;

		case OPT_TYPE_STRING:
			// PRINT_OPTION_USAGE(pvcOptEntry, char*, "string", pvcOptEntry->iNrOfParameters, true);
			_PrintOptionUsage<char*>(pvcOptEntry, "string", true);
			break;

		case OPT_TYPE_ENUM:
			// PRINT_OPTION_USAGE(pvcOptEntry, int, "<enum>", 1, false);
			_PrintOptionUsage<int>(pvcOptEntry, "enum", false);

			fprintf(stderr, "\tThe <enum> can be as follows:\n");
			for(vector<CEnumEntry>::iterator 
					ivcEnum = pvcOptEntry->vcEnumTable.begin(); 
				ivcEnum != pvcOptEntry->vcEnumTable.end(); 
				ivcEnum ++)
			{
				fprintf(stderr, "\t\t%s\n", ivcEnum->szEnumName);
			}
			break;

		default:
			;
		}
		fprintf(stderr, "\n");
		fprintf(stderr, "\t%s\n", pvcOptEntry->szComment);
		fprintf(stderr, "\n");
	}
}

///////////////////////////////////////////////////////////////////////////////////
void
_OPTAddComment(
	const char* szArgName, 
	const char *szComment
)
{
	for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
		if( 0 == strcmp_func(szArgName, pvcOptEntry->szArgName) )
		{
			pvcOptEntry->szComment = szComment;
			return;
		}

	fprintf(stderr, "Warning in _OPTAddComment(): unrecognized option %s\n", szArgName);
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

	// check whether --help is specified. This check is applied first such that --help can always show the 
	// default value.
	for(iA = iBegin; iA<argc; iA++)
		for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
			if( 0 == strcmp_func(argv[iA], "--help") )
			{
				_OptPrintComment();
				exit(0);
			}

	bResult = true;
	for(iA = iBegin; iA<argc; iA++)
	{
		for(pvcOptEntry=vcOptTable.begin(); pvcOptEntry != vcOptTable.end(); pvcOptEntry++) 
			if( 0 == strcmp_func(argv[iA], "--help") )
			{
				_OptPrintComment();
				exit(0);
				break;
			}
			else
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
					case OPT_TYPE_INT:	
						*(int*)pvcOptEntry->ppParameters[iP] = atoi(argv[++iA]);	
						break;
					case OPT_TYPE_FLOAT:
						*(float*)pvcOptEntry->ppParameters[iP] = (float)atof(argv[++iA]);	
						break;
					case OPT_TYPE_STRING:
						*(char**)pvcOptEntry->ppParameters[iP] = argv[++iA];
						break;

					case OPT_TYPE_ENUM:	
					case OPT_TYPE_BOOL:
						{
							const char *szEnum = argv[++iA];
							for(vector<CEnumEntry>::iterator 
									ivcEnum = pvcOptEntry->vcEnumTable.begin();
								ivcEnum != pvcOptEntry->vcEnumTable.end();
								ivcEnum ++)

								if( 0 == strcmp_func(szEnum, ivcEnum->szEnumName) )
								{
									*(int*)pvcOptEntry->ppParameters[iP] = ivcEnum->iEnumValue;
									break;
								}
						}
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
			// clear the names
			for(int p = 0; p < pvcOptEntry->iNrOfParameters; p++)	
				free(pvcOptEntry->pszParamNames[p]);
			free(pvcOptEntry->pszParamNames);
		}
	}

	return bResult;
}

