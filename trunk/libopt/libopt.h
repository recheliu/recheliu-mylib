/*

LIBOPT: a libraries used to parse the input arguments

Usage:
	To use this library, _OPTInit() should be called first. If a case-sensitve comparison 
is desired, parameter could be set as "false."
	
	After _OPTInit is called, several functions could be called to 
specify the name of arguments, and the number, type, default value 
of the parameters for each argument if necessary. Note the the name 
of the arguments must begin with character '-'.

	For arguments with parameters, the format is 

		_OPTAdd<Type>Vector(arg_name, number_of_parameters, 
			<parameter0_address>, <parameter0_default_value>,
			<parameter1_address>, <parameter1_default_value>,
			...)

	If some arguments could triger the change of value of some variable, 
they could be specified by using function _OPTAddFlag(). The format is:

		_OPTAddFlag(arg_name, parameter_address, value, default_value)

which mean that if the argumen <arg_name> is given, the value is assigned to the 
variable in address <parameter_address>; otherwise the value <default_value> is assigned.
This function could be combine with function _OPTAdd<Type>Vector so a argument could 
set up parameter and change the value of some flag.

	After specifying the arguments, function BOPTParse could be called to parse the arguments.
This function is returned if no valid argument is found. The index of the 
successor of the last traversed argument is returned. TRUE is returned If the 
parsing is succesfull.

*/
#ifndef __LIB__OPT__H__
#define __LIB__OPT__H__

#include <stdarg.h>

#define OPT_FALSE	0
#define OPT_TRUE	1

// ADD-BY-LEETEN 04/11/2007-BEGIN
// declare a boolean variable
void _OPTAddBoolean(const char* szArgName, int *piParameter, int iDefaultValue = OPT_FALSE);
// ADD-BY-LEETEN 04/11/2007-END

// specify a flag. If the flag is given the variable *pbParameter is set as bValue (otherwise it is set as the default balue bDefault)
void _OPTAddFlag(const char* szArgName, int *piParameter, int iValue, int iDefault);

// add an option to specify a flag whose parameter's type are floating point
void _OPTAddFloatingVector(const char *szArgName, int iNrOfParameters, ...);

// add an option to specify a flag whose parameter's type are integer
void _OPTAddIntegerVector(const char *szArgName, int iNrOfParameters, ...);

// add an option to specify a flag whose parameter's type are string
// note that there is no memory allocation for the string. 
// All of them will be pointed to the pointers in the argument list
void _OPTAddStringVector(const char *szArgName, int iNrOfParameters, ...);

// initialize the parse of arguments
void _OPTInit(bool bCaseInsentive=true);

// parse the arguments
bool BOPTParse(char* argv[], int argc, int iBegin, int *piEnd = NULL);

#endif // __LIB__OPT__H__

/*

$Log: not supported by cvs2svn $

*/

	