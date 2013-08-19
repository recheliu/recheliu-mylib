#pragma once

#include "NrrdIO.h"

#ifdef WIN32
	#if		defined(_DEBUG)
	#pragma comment (lib, "ITKNrrdIOd.lib")      
	#pragma comment (lib, "zlibd.lib")      // link with my own library libfps
	#else	// #if	defined(_DEBUG)
	#pragma comment (lib, "ITKNrrdIO.lib")     
	#pragma comment (lib, "zlib.lib")      // link with my own library libfps
	#endif	// #if	defined(_DEBUG)
#endif	// #ifdef	WIN32

namespace NrrdIOUtil
{
  #if !WITH_BOOST
  enum
  {
    ENUM_BASE,
    FILENAME_MAX_LENGTH = 1024,
    ENUM_END
  };
  #endif // #if !WITH_BOOST

  template <class DT, class NT>
    DT*
    PConvertNrrd
    (
     const Nrrd *nin
     )
    {
      NT *data = (NT*)nin->data;

      size_t uDataLength = 1;
      for(size_t d = 0; d < nin->dim; d++)
        uDataLength *= (size_t)nin->axis[d].size;

      DT* pData = new DT[uDataLength];
      for(size_t d = 0; d < uDataLength; d++)
        pData[d] = (DT)data[d];

      return pData;
    }

  template <class DT, class NT, int nrrdType>
    void
    _SaveNrrd
    (
     const vector<size_t>& vuDimLengths,
     const DT* pData,
     const char* szFilepath
     )
    {
      #if WITH_BOOST

      path pathNhdr(szFilepath);
      if( pathNhdr.extension() != ".nhdr" )
        pathNhdr += ".nhdr";
      path pathRaw(pathNhdr);      
	  pathRaw.replace_extension("raw");
      path pathLeafNhdr(pathNhdr.leaf());
      path pathLeafRaw(pathLeafNhdr);      pathLeafRaw.replace_extension("raw");
      const char *szLocalNhdr = pathLeafNhdr.string().c_str();

      #else // #if WITH_BOOST

      // create the .nhdr filename
      char pathNhdr[FILENAME_MAX_LENGTH];
      strcpy(pathNhdr, szFilepath);
      char *szExtension = strrchr(pathNhdr, '.');
      if( 0 != strcmp(szExtension, ".nhdr" ) )
        strcat(pathNhdr, ".nhdr");

      // generate the corresponding .raw filename
      char pathRaw[FILENAME_MAX_LENGTH];
      strcpy(pathRaw, pathNhdr);
      strcpy(strrchr(pathRaw, '.'), ".raw");

      // generate the base filename for .nhdr
      char *szFilename = strrchr(pathNhdr, '/');
      #if defined(WIN32)
      if( !szFilename )
          szFilename = strrchr(pathNhdr, '\\');
      #endif // #if defined(WIN32)
      if( szFilename )
        szFilename++;
      else
        szFilename = &pathNhdr[0];
      char pathLeafNhdr[FILENAME_MAX_LENGTH];  strcpy(pathLeafNhdr, szFilename);
      const char *szLocalNhdr = &pathLeafNhdr[0];

      // generate the base filename for .raw
      char pathLeafRaw[FILENAME_MAX_LENGTH];   strcpy(pathLeafRaw, pathLeafNhdr);
      strcpy(strrchr(pathLeafRaw, '.'), ".raw");

      #endif // #if WITH_BOOST

      size_t puSize[NRRD_DIM_MAX];
      size_t uDataLength = 1;
      for(size_t d = 0; d < vuDimLengths.size(); d++)
        {
          puSize[d] = vuDimLengths[d];
          uDataLength *= puSize[d];
        }

      NT *pNrrdData = new NT[uDataLength];
      for(size_t d = 0; d < uDataLength; d++)
        pNrrdData[d] = (NT)pData[d];

      Nrrd *nrrdOut = nrrdNew();
      nrrdWrap_nva(nrrdOut, &pNrrdData[0], nrrdType, (unsigned int)vuDimLengths.size(), &puSize[0]);
      nrrdSave(szLocalNhdr, nrrdOut, NULL);

      // nrrdIoStateNix(nioOut);
      nrrdNix(nrrdOut);
      delete [] pNrrdData;

		#if WITH_BOOST
		if( pathLeafNhdr != pathNhdr )
		#else // #if WITH_BOOST
        if( 0 != strcmp( pathLeafNhdr, pathNhdr ) )
		#endif // #if WITH_BOOST
        {
          remove(pathNhdr);
          remove(pathRaw);
          rename(pathLeafNhdr, pathNhdr);
          rename(pathLeafRaw, pathRaw);
        }
    }
};
