#pragma once
#ifndef __axFileSystem_h__
#define __axFileSystem_h__

#include "../string/axIString.h"

//! \ingroup data_file_system
//@{

class axFileSystem : public axNonCopyable {
public:

	static	axStatus	getCurrentDir		( axIStringW &out );
	static	axStatus	getExecuteDir		( axIStringW &out );
	static	axStatus	getExecuteFileName	( axIStringW &out );
	static	axStatus	changeDir			( const wchar_t* path );

	enum { kPathMax = 512 };

#if axOS_WIN
	enum { kNativePathSeparator = '\\' };
#else
	enum { kNativePathSeparator = '/' };
#endif
};


#if axOS_WIN

inline
axStatus	axFileSystem::changeDir( const wchar_t* path ) {
	return ::_wchdir( path );
}

inline 
axStatus	axFileSystem::getCurrentDir( axIStringW	&out ) {
	axStatus st;
	axIArray<wchar_t>	*buf;
	DWORD	n = kPathMax;
	st = out.resize( n, false );		if( !st ) return st;
	st = out.getBufferPtr( buf );		if( !st ) return st;
	n = ::GetCurrentDirectory( n, buf->ptr() );
	if( n==0 ) { out.clear(); return -1; }
	return out.resize(n);
}

inline
axStatus	axFileSystem::getExecuteDir( axIStringW	&out ) {
	axStatus st;
	st = getExecuteFileName( out );	if( !st ) return st;

	axSize	pos;
	if( out.findFromEnd( kNativePathSeparator, pos ) ) {
		out.resize( pos );
	}
	return 0;
}

inline
axStatus	axFileSystem::getExecuteFileName( axIStringW	&out ) {
	axStatus st;
	axIArray<wchar_t>	*buf = NULL;
	DWORD n = kPathMax;
	st = out.resize( kPathMax, false );	if( !st ) return st;
	st = out.getBufferPtr( buf );			if( !st ) return st;
	n = ::GetModuleFileName( NULL, buf->ptr(), n );
	if( n==0 ) { out.clear(); return -1; }
	return out.resize(n);
}

#endif //axOS_WIN

//@}


#endif //__axFileSystem_h__
