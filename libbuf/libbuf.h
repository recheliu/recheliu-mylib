/*

This file defines several important macros for CUDA.

*/

#ifndef __LIB_BUFFER_H__
#define __LIB_BUFFER_H__

// ADD-BY-LEETEN 2011/01/10-BEGIN
#include <typeinfo>
using namespace std;
// ADD-BY-LEETEN 2011/01/10-END

// ADD-BY-LEETEN 2009/11/04-BEGIN
#include <stdlib.h>
// ADD-BY-LEETEN 2009/11/04-END

#include <memory.h>
#include <assert.h>

#include "liblog.h"

// a template for buffer
template <typename t> struct TBuffer 
{
	int num;
	t* data;

	TBuffer()	
	{	
		num = 0;
		data = NULL;	
	}

	// ADD-BY-TLEE 2009/02/14-BEGIN
	bool BIsAllocated()		{	return (NULL != data)?true:false;	}
	unsigned int USize()	{	return (unsigned int)num;			}
	// ADD-BY-TLEE 2009/02/14-END

	// ADD-BY-TLEE 2008/08/14-BEGIN
	void free()
	{
		if( data ) 
			delete [] data;
		data = NULL;
	}
	// ADD-BY-TLEE 2008/08/14-END

	// ADD-BY-LEETEN 09/19/2010-BEGIN
	void _Save(char *szFilenamePrefix)
	{
		if( !BIsAllocated() )
		{
			// MOD-BY-LEETEN 02/24/2011-FROM:
				// LOG("Non-initialized buffer");
			// TO:
			LOG(printf("Non-initialized buffer"));
			// MOD-BY-LEETEN 02/24/2011-END
			return;
		}

		char szFilename[1024];
		sprintf(szFilename, "%s.%s.buf", szFilenamePrefix, typeid(t).name());

		FILE *fp;
		fp = fopen(szFilename, "wb");
		if( !fp )
		{
			perror(szFilename);
			return;
		}

		fwrite(&num, sizeof(num), 1, fp);
		fwrite(data, sizeof(t), num, fp);

		fclose(fp);

	}
	// ADD-BY-LEETEN 09/19/2010-END

	~TBuffer()	
	{	
		#if	0	// MOD-BY-TLEE 2008/08/14-FROM:
			if( data ) 
				// free(data);	
				delete [] data;
			data = NULL;
		#else	// MOD-BY-TLEE 2008/08/14-TO:

		free();

		#endif	// MOD-BY-TLEE 2008/08/14-END
	}

	t* alloc(int n)
	{
		assert(n > 0);
		num = n;

		// ADD-BY-TLEE 2008/08/14-BEGIN
		free();
		// ADD-BY-TLEE 2008/08/14-END

		// data = (t*)calloc(num, sizeof(t));
		data = new t[num];
		assert(data);
		memset(data, 0, sizeof(t) * num);
		return data;
	}

	t& operator[](int index)
	{
		assert( index < num );
		return data[index];
	}

	const t& operator[](int index) const
	{
		assert( index < num );
		return data[index];
	}
};

// ADD-BY-LEETEN 09/19/2010-BEGIN
#ifdef WIN32
	#define SAVE_BUF(buf_name)	\
		{	\
			char szFilename[1024];	\
			strcpy(szFilename, strrchr(__FILE__, '\\') + 1);	\
			strcat(szFilename,  "." __FUNCTION__ "." #buf_name);	\
			for(char *	\
				szChar = strchr(szFilename, ':'); \
				szChar;	\
				szChar = strchr(szChar, ':')	) \
				*szChar = '_';	\
			buf_name._Save(szFilename); \
		}	\

#else
	#if	0	// MOD-BY-LEETEN 01/30/2011-FROM:
	#define SAVE_BUF(buf_name)	\
		{	\
			char szFilename[1024];	\
			strcpy(szFilename, __FILE__);	\
			strcat(szFilename,  "." __FUNCTION__ "." #buf_name);	\
			for(char *	\
				szChar = strchr(szFilename, ':'); \
				szChar;	\
				szChar = strchr(szChar, ':')	) \
				*szChar = '_';	\
			buf_name._Save(szFilename); \
		}	\

	#else	// MOD-BY-LEETEN 01/30/2011-TO:

	#define SAVE_BUF(buf_name)	\
		{	\
			char szFilename[1024];	\
			strcpy(szFilename, __FILE__);	\
			strcat(szFilename,  #buf_name);	\
			for(char *	\
				szChar = strchr(szFilename, ':'); \
				szChar;	\
				szChar = strchr(szChar, ':')	) \
				*szChar = '_';	\
			buf_name._Save(szFilename); \
		}	\

	#endif	// MOD-BY-LEETEN 01/30/2011-END

#endif
// ADD-BY-LEETEN 09/19/2010-END

#endif	// __LIB_BUFFER_H__

/*

$Log: not supported by cvs2svn $
Revision 1.7  2011/01/31 03:18:57  leeten

[01/30/2011]
1. [DEL] Remove the macro __FUNCTION__ in the macro SAVE_BUF.

Revision 1.6  2011/01/10 19:47:53  leeten

[01/09/2010]
1. [ADD] Include the header typeinfo.

Revision 1.5  2010/09/21 13:29:15  leeten

[09/21/2010]
1. [ADD] Define a new method _Save() to dump the data in the buffer.
2. [ADD] Define a new macro SAVE_BUF to dump the data into a file which is automatically named according to the variable's name and scope.

Revision 1.4  2009/11/04 20:43:31  leeten

[2009/11/04]
1. [ADD] Include the header stdlib.h.

Revision 1.3  2009/02/22 22:04:50  leeten

[2009/02/22]
1. [ADD] Add a new method BIsAllocated() to check whether the buffer is allocated.
2. [ADD] Add a new method USize() to check the length of the buffer.

Revision 1.2  2008/08/14 22:52:24  leeten

[2008/08/14]
1. [ADD] Add a new method free() to explictly free the allocated buffer.

Revision 1.1.1.1  2008/08/10 18:54:05  leeten

[2008/08/10]
1. First time checkin. This lib. is used to wrap a buffer in order to avoid the routine to check and free memory.


*/



