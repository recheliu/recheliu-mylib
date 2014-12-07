#include "liblog.h"
#include "liboptpp/liboptpp.h"

using namespace OptExt;

int
main(int argc, char* argv[])
{
	COptParser cOptParser;

	// Now boolean variables are supported.
	bool bBool = false;
	cOptParser._AddOptEntry(new COptBoolean<bool>("--bool", &bBool, bBool));

	// boolean option can be used for integers too.
	int iBool = 0;
	cOptParser._AddOptEntry(new COptBoolean<int>("--int-bool", &iBool, iBool));

	int iEnum = 0;
	cOptParser._AddOptEntry(new COptEnum<>("--int-enum", &iEnum, iEnum, 4,
		"value0", 0,
		"value1", 1,
		"value2", 2,
		"value3", 3
		));

	enum EEnum {
		ENUM0, 
		ENUM1,
		ENUM2,
		ENUM3,
		NR_OF_ENUMS
	} ;

	// Now it can also support enum.
	EEnum eEnum = ENUM0;
	cOptParser._AddOptEntry(new COptEnum<EEnum>("--enum", &eEnum, eEnum, 
		4,
		"enum0", ENUM0,
		"enum1", ENUM1,
		"enum2", ENUM2,
		"enum3", ENUM3
		));

	int iInt= 0;
	cOptParser._AddOptEntry(new COptIntVector("--int", 1, &iInt, iInt));

	int piInts[3];
	cOptParser._AddOptEntry(new COptIntVector("--int-vec", sizeof(piInts)/sizeof(piInts[0]), 
			&piInts[0], 0,
			&piInts[1], 0,
			&piInts[2], 0
			)
		);

	// Now string is supported.
	string pszStrings[3];
	cOptParser._AddOptEntry(new COptStringVector("--string-vec", sizeof(pszStrings)/sizeof(pszStrings[0]), 
			&pszStrings[0], "str0",
			&pszStrings[1], "str1",
			&pszStrings[2], "str2"
			)
		);

	// Meanwhile C-style char* is supported too. 
	// Note that it only supports const char*.
	// In the long run, chart* should be deprecated.
	const char* pszCharPtrs[3];
	cOptParser._AddOptEntry(new COptCharPtrVector("--char-ptr-vec", sizeof(pszCharPtrs)/sizeof(pszCharPtrs[0]), 
			&pszCharPtrs[0], "char_ptr0",
			&pszCharPtrs[1], "char_ptr1",
			&pszCharPtrs[2], "char_ptr2"
			)
		);

	cOptParser.BOPTParse(argc, argv, 1);

	LOG_VAR(bBool);
	LOG_VAR(iEnum);
	LOG_VAR(iInt);
	for(size_t i = 0; i < sizeof(piInts)/sizeof(piInts[0]); i++) 
	{
		LOG_VAR(piInts[i]);
	}
	for(size_t i = 0; i < sizeof(pszCharPtrs)/sizeof(pszCharPtrs[0]); i++) 
	{
		LOG_VAR(pszCharPtrs[i]);
	}
	for(size_t i = 0; i < sizeof(pszStrings)/sizeof(pszStrings[0]); i++) 
	{
		LOG_VAR(pszStrings[i]);
	}

	return 0;
}
