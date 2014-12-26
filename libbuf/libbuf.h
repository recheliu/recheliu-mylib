#ifndef __LIB_BUFFER_H__
#define __LIB_BUFFER_H__

#include <typeinfo>
using namespace std;

#include <stdlib.h>

#include <memory.h>
#include <assert.h>

#include "liblog.h"

// a template for buffer
template <typename t> struct TBuffer 
{
	size_t num;
	t* data;

	TBuffer()	
	{	
		num = 0;
		data = NULL;	
	}

	bool BIsAllocated()		{	return (NULL != data)?true:false;	}
	size_t USize()	const	{	return num;			}

	void free()
	{
		if( data ) 
			delete [] data;
		data = NULL;
	}

	void _Save(const char *szFilenamePrefix)
	{
		if( !BIsAllocated() )
		{
			LOG(printf("Non-initialized buffer"));
			return;
		}

		char szFilename[1024];
		sprintf(szFilename, "%s.%s.buf", szFilenamePrefix, typeid(t).name());

		FILE *fp;
		fp = fopen(szFilename, "wb");
		ASSERT_OR_LOG(fp, perror(szFilename));

		fwrite(&num, sizeof(num), 1, fp);
		fwrite(data, sizeof(t), num, fp);

		fclose(fp);

	}

	~TBuffer()	
	{	
		free();
	}

	t* New(size_t n)
	{
		assert(n > 0);
		num = n;
		free();
		data = new t[num];
		assert(data);
		return data;
	}

	t* alloc(size_t n)
	{
		New(n);
		memset(data, 0, sizeof(t) * num);
		return data;
	}

	t& operator[](size_t index)
	{
		assert( index < num );
		return data[index];
	}

	const t& operator[](size_t index) const
	{
		assert( index < num );
		return data[index];
	}
};

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

#endif

#endif	// __LIB_BUFFER_H__

