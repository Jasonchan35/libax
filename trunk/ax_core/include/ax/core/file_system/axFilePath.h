#ifndef __axFilePath_h__
#define __axFilePath_h__

#include "../string/ax_string.h"

class axFilePath : public axNonCopyable {
	axFilePath() {}
public:
	static axStatus	getBaseName	( axIStringA &out, const char*    path, bool with_ext = true );
	static axStatus	getBaseName	( axIStringW &out, const wchar_t* path, bool with_ext = true );
	
	static axStatus	getDirName	( axIStringA &out, const char*    path );
	static axStatus	getDirName	( axIStringW &out, const wchar_t* path );

	static axStatus	getFileExt	( axIStringA &out, const char*	  path );
	static axStatus	getFileExt	( axIStringW &out, const wchar_t* path );
	
	static axStatus	getParentDir( axIStringA &out, const char*	  path );
	static axStatus	getParentDir( axIStringW &out, const wchar_t* path );
	
	static axStatus	getNormalizePath	( axIStringA &out, const char*	  path );
	static axStatus	getNormalizePath	( axIStringW &out, const wchar_t* path );

	static bool		isAbsolute	( const char*    path );
	static bool		isAbsolute	( const wchar_t* path );
	
	static axStatus	getFullPath ( axIStringA &out, const char*		path );
	static axStatus	getFullPath ( axIStringW &out, const wchar_t*	path );
};



#endif //__axFilePath_h__
