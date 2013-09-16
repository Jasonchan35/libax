#ifndef __axFilePath_h__
#define __axFilePath_h__

#include "../string/ax_string.h"

class axFilePath : public axNonCopyable {
	axFilePath() {}
public:
	static axStatus	baseName	( axIStringA &out, const char*    path, bool with_ext = true );
	static axStatus	baseName	( axIStringW &out, const wchar_t* path, bool with_ext = true );
	
	static axStatus	dirName		( axIStringA &out, const char*    path );
	static axStatus	dirName		( axIStringW &out, const wchar_t* path );

	static axStatus	fileExt		( axIStringA &out, const char*	  path );
	static axStatus	fileExt		( axIStringW &out, const wchar_t* path );

	static axStatus	changeExt	( axIStringA &out, const char* 	  path, const char*    newExt );
	static axStatus	changeExt	( axIStringW &out, const wchar_t* path, const wchar_t* newExt );
	
	static axStatus	parentDir	( axIStringA &out, const char*	  path );
	static axStatus	parentDir	( axIStringW &out, const wchar_t* path );
	
	static axStatus	normalize	( axIStringA &out, const char*	  path );
	static axStatus	normalize	( axIStringW &out, const wchar_t* path );
	
	static bool		isAbsolute	( const char*    path );
	static bool		isAbsolute	( const wchar_t* path );
	
	static axStatus	fullPath 	( axIStringA &out, const char*		path );
	static axStatus	fullPath 	( axIStringW &out, const wchar_t*	path );
};



#endif //__axFilePath_h__
