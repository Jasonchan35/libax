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
    wchar_t tmp[kPathMax+1];
    DWORD n;
    n = ::GetCurrentDirectory( kPathMax, tmp );
	if( n == 0 ) return -1;
	tmp[n] = 0;
	return out.set( tmp );
}

inline
axStatus	axFileSystem::getExecutablePath ( axFilePathW	&out ) {
    wchar_t tmp[kPathMax+1];
    DWORD n;
    n = ::GetModuleFileName( NULL, tmp, kPathMax );
	if( n==0 ) return -1;
	tmp[n] = 0;
	return out.set( tmp );
}
#else

inline
axStatus    axFileSystem::getCurrentDir( axIStringW &out ) {
	char  tmp[kPathMax+1];
	if( ! getcwd( tmp, kPathMax ) ) return -1;
	return out.set( tmp );
}

inline
axStatus	axFileSystem::getExecutablePath ( axFilePathW	&out ) {
    out.clear();
    return 0;
}

#endif // else axOS_WIN

//@}


#endif //__axFileSystem_h__
