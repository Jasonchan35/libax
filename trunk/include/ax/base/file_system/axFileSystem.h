#pragma once
#ifndef __axFileSystem_h__
#define __axFileSystem_h__

#include "axFilePath.h"

//! \ingroup data_file_system
//@{

class axFileSystem : public axNonCopyable {
public:

	static	axStatus	getCurrentDir		( axIStringW	 &out );
	static	axStatus	setCurrentDir		( const wchar_t* path );

	static	axStatus	getExecutablePath	( axFilePathW	 &out );

	enum { kPathMax = 512 };

};


#if axOS_WIN

inline
axStatus	axFileSystem::setCurrentDir ( const wchar_t* path ) {
	return ::_wchdir( path );
}

inline 
axStatus	axFileSystem::getCurrentDir ( axIStringW	&out ) {
	axStatus st;
	DWORD	n = kPathMax;
	st = out.resize( n, false );					if( !st ) return st;
	wchar_t* p = out.getInternalBufferPtr();		if( !p ) return -1;
	n = ::GetCurrentDirectory( n, p );
	if( n==0 ) { out.clear(); return -1; }
	return out.resize(n);
}

inline
axStatus	axFileSystem::getExecutablePath ( axFilePathW	&out ) {
	out.clear();
	axStatus st;
	axTempStringW	tmp;

	DWORD n = kPathMax;
	st = tmp.resize( n, false );				if( !st ) return st;
	wchar_t *p = tmp.getInternalBufferPtr();	if( !p  ) return -1;
	n = ::GetModuleFileName( NULL, p, n );
	if( n==0 ) return -1;
	st = tmp.resize(n);							if( !st ) return st;
	st = out.set( tmp );						if( !st ) return st;
	return 0;
}

#endif //axOS_WIN

//@}


#endif //__axFileSystem_h__
