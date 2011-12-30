/*
This header defines macros to log
 */

#ifndef __LOG_NETCDF_H__
#define __LOG_NETCDF_H__

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdarg.h>
#include "liblog.h"

// The macro for NetCDF
#define ASSERT_NETCDF(nc_stmt)\
  {			      \
    int iNcError;	      \
    ASSERT_OR_LOG(				\
		  NC_NOERR == (iNcError = nc_stmt),	\
		  fprintf(stderr, "NetCDF Error: %s.", nc_strerror(iNcError)));	\
  }

// The macro for Parallel NetCDF
#define ASSERT_NCMPI(nc_stmt)\
  {			      \
    int iNcError;	      \
    ASSERT_OR_LOG(				\
		  NC_NOERR == (iNcError = nc_stmt),	\
		  fprintf(stderr, "NetCDF Error: %s.", ncmpi_strerror(iNcError)));	\
  }

#endif	// __LOG_NETCDF_H__

/*

$Log: not supported by cvs2svn $

*/
