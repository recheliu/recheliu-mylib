/*
This header defines macros to log
 */

#ifndef __LOG_MPI_H__
#define __LOG_MPI_H__

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdarg.h>
#include "liblog.h"

// The macro for NetCDF
#define ASSERT_MPI(mpi_stmt)  \
  {			      \
  int iError;						\
  if( MPI_SUCCESS != (iError = mpi_stmt) )		\
    {							\
      char* szMpiStmt = # mpi_stmt;			\
      LOG_VAR_TO_ERROR(szMpiStmt);			\
      char szError[MPI_MAX_ERROR_STRING];		\
      int iErrorStrLen;					\
      MPI_Error_string(iError, szError, &iErrorStrLen);	\
      LOG_VAR_TO_ERROR(szError);			\
    }							\
  }

#endif	// __LOG_MPI_H__

/*

$Log: not supported by cvs2svn $

*/
