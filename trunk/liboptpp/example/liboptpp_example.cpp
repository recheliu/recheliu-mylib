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

	// ADD-BY-LEETEN 2014/12/06-BEGIN
	int iIntRequired= 0;
	cOptParser._AddOptEntry(new COptIntVector("--int-required", 1, &iIntRequired, iIntRequired));
	cOptParser._SetRequired("--int-required", true);
	// ADD-BY-LEETEN 2014/12/06-END

	// ADD-BY-LEETEN 2014/12/06-BEGIN
	float fFloat = 0.0f;
	cOptParser._AddOptEntry(new COptFloatVector("--float", 1, &fFloat, fFloat));

	float pfFloats[3];
	// MOD-BY-LEETEN 2014/12/06-FROM:
	/*
	cOptParser._AddOptEntry(new COptFloatVector("--float-vec", sizeof(pfFloats)/sizeof(pfFloats[0]), 
			&pfFloats[0], 0.1f,
			&pfFloats[1], 0.2f,
			&pfFloats[2], 0.3f
			)
		);
	*/
	// MOD-BY-LEETEN 2014/12/06-TO:
	cOptParser._AddOptEntry( (new COptFloatVector("--float-vec", sizeof(pfFloats)/sizeof(pfFloats[0]), 
			&pfFloats[0], 0.1f,
			&pfFloats[1], 0.2f,
			&pfFloats[2], 0.3f
			))
			->withDescription("A vector of floating-point values.") 
		);
	// MOD-BY-LEETEN 2014/12/06-END

	double dDouble = 0.0;
	cOptParser._AddOptEntry(new COptPrimitiveTypeVector<double>("--double", 1, &dDouble, dDouble));

	double pdDoubles[3];
	cOptParser._AddOptEntry(new COptPrimitiveTypeVector<double>("--double-vec", sizeof(pdDoubles)/sizeof(pdDoubles[0]), 
			&pdDoubles[0], 0.2,
			&pdDoubles[1], 0.4,
			&pdDoubles[2], 0.8
			)
		);
	// ADD-BY-LEETEN 2014/12/06-END

	// Now string is supported.
	string pszStrings[3];
	cOptParser._AddOptEntry(new COptStringVector("--string-vec", sizeof(pszStrings)/sizeof(pszStrings[0]), 
			#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
			&pszStrings[0], "str0",
			&pszStrings[1], "str1",
			&pszStrings[2], "str2"
			#else	// MOD-BY-LEETEN 2014/12/06-TO:
			// string cannot be initialized by NULL.
			// A workaround is used empty ("").
			&pszStrings[0], "",
			&pszStrings[1], "str1",
			&pszStrings[2], "str2"
			#endif	// MOD-BY-LEETEN 2014/12/06-END
			)
		);

	// Meanwhile C-style char* is supported too. 
	// Note that it only supports const char*.
	// In the long run, chart* should be deprecated.
	const char* pszCharPtrs[3];
	cOptParser._AddOptEntry(new COptCharPtrVector("--char-ptr-vec", sizeof(pszCharPtrs)/sizeof(pszCharPtrs[0]), 
			#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
			&pszCharPtrs[0], "char_ptr0",
			#else	// MOD-BY-LEETEN 2014/12/06-TO:
			&pszCharPtrs[0], NULL,
			#endif	// MOD-BY-LEETEN 2014/12/06-END
			&pszCharPtrs[1], "char_ptr1",
			&pszCharPtrs[2], "char_ptr2"
			)
		);

	// MOD-BY-LEETEN 2014/12/06:	cOptParser.BOPTParse(argc, argv, 1);
	ASSERT_OR_LOG( cOptParser.BOPTParse(argc, argv, 1), "");
	// MOD-BY-LEETEN 2014/12/06-END

	LOG_VAR(bBool);
	LOG_VAR(iEnum);
	LOG_VAR(iInt);
	for(size_t i = 0; i < sizeof(piInts)/sizeof(piInts[0]); i++) 
	{
		LOG_VAR(piInts[i]);
	}
	for(size_t i = 0; i < sizeof(pszCharPtrs)/sizeof(pszCharPtrs[0]); i++) 
	{
		#if	0	// MOD-BY-LEETEN 2014/12/06-FROM:
		LOG_VAR(pszCharPtrs[i]);
		#else	// MOD-BY-LEETEN 2014/12/06-TO:
		// const char* can be NULL,
		// and thus require additional check.
		if( pszCharPtrs[i] )
		{
			LOG_VAR(pszCharPtrs[i]);
		} else {
			LOG(cerr<<"pszCharPtrs[i] is NULL");
		}
		#endif	// MOD-BY-LEETEN 2014/12/06-END
	}
	for(size_t i = 0; i < sizeof(pszStrings)/sizeof(pszStrings[0]); i++) 
	{
		LOG_VAR(pszStrings[i]);
	}

	// ADD-BY-LEETEN 2014/12/06-BEGIN
	LOG_VAR(fFloat);
	for(size_t i = 0; i < sizeof(pfFloats)/sizeof(pfFloats[0]); i++) 
	{
		LOG_VAR(pfFloats[i]);
	}
	LOG_VAR(dDouble);
	for(size_t i = 0; i < sizeof(pdDoubles)/sizeof(pdDoubles[0]); i++) 
	{
		LOG_VAR(pdDoubles[i]);
	}
	// ADD-BY-LEETEN 2014/12/06-END

	return 0;
}
