/*
This file is used to test my own library LIBLOG.
*/

#include <iostream>
using namespace std;
#include "liblog.h"

int
main(int argn, char* argv[])
{
	// test the macro LOG
	LOG(printf("%d %d %d", 1, 2, 3));

	// test the macro LOG VAR, which display a variable's name and its value. 
	LOG_VAR(argn);
	LOG_VAR(argv);
	LOG_VAR(argv[0]);
	LOG_VAR(main);

	return 0;
}

/*

$Log: not supported by cvs2svn $

*/

