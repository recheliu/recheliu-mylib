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

// ADD-BY-LEETEN 02/22/2008-BEGIN
void _OPTAlign(const char* szArgName, const char* szNewArgName);
// ADD-BY-LEETEN 02/22/2008-END

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

// ADD-BY-LEETEN 07/02/2008-BEGIN
void _OPTAddEnum(const char* szArgName, int *piParameter, int iDefault, int iNrOfEnumValues, ...);

void _OPTAddComment(const char* szArgName, const char *szComment);

void _OptPrintComment();
// ADD-BY-LEETEN 07/02/2008-END

// initialize the parse of arguments
void _OPTInit(bool bCaseInsentive=true);

// parse the arguments
bool BOPTParse(char* argv[], int argc, int iBegin, int *piEnd = NULL);

#endif // __LIB__OPT__H__

/*

$Log: not supported by cvs2svn $
Revision 1.4  2008/07/07 15:00:32  leeten

[07/07/2008]
1. [ADD] Declare new function _OPTAddEnum() to parse a command argument from string to predefined constants.
2. [ADD] Declare new function _OPTAddComment() to define comment with the arguments.
3. [ADD] Declare new function _OptPrintComment() to print out comments.

Revision 1.3  2008/02/23 05:16:38  leeten

[02/23/2008]
1. Declare a new function _OPTAlign() to align an argument to an existed one.

Revision 1.2  2007/04/12 19:03:16  leeten

[04/12/2007]
1. [ADDED] Declare new function OptAddBoolean().


*/
