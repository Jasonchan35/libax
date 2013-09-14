//
//  axFilePath.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-23.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/file_system/axFilePath.h>

template<class T> inline
static axStatus	_getFileExt( axIString_<T> &out, const T* path ) {
	axStatus st;
	const T* p = ax_strrchr(path,(T)'.');
	if( !p ) {
		out.clear();
		return 0;
	}	
	st = out.set( p+1 );	if( !st ) return st;
	return 0;
}

template<class T> inline
static axStatus	_getBaseName( axIString_<T> &out, const T* path, bool with_ext ) {
	if( ! path ) { out.clear(); return 0; }
	axStatus st;
	
	T sp[3];
	sp[0] = (T)'/';
	sp[1] = (T)'\\';
	sp[2] = 0;
	
	const T* p = ax_strrchrs( path, sp );
	if( !p ) {
		p = path;
	}else{
		p++;
	}
	const T* e = NULL;
	if( ! with_ext ) {
		e = ax_strrchr( p, (T)'.' );		
	}
	if( e ) {
		st = out.setWithLength( p, e-p );
	}else{
		st = out.set( p );		
	}	
	if( ! st ) return st;
	return 0;
}

template<class T> inline
static axStatus	_getDirName( axIString_<T> &out, const T* path ) {
	axStatus st;
	if( ! path ) { out.clear(); return 0; }
	const T* p;
	p = ax_strrchr( path, (T)'/' );
	if( !p ) {
		p = ax_strrchr( path, (T)'\\' );
	}

	if( !p ) {
		out.clear();
	}else{
		st = out.setWithLength( path, p-path+1 );		if( !st ) return st;
	}
	return 0;
}

template<class T> inline
static bool	_isAbsolute( const T* path ) {
	if( !path ) return false;

#if axOS_UNIX //Cygwin also count as Unix
	if( path[0] == '/' ) return true;
	return false;

#elif axOS_WIN
	if( ax_isalpha(path[0]) && path[1] == ':' ) return true;
	if( path[0] == '\\' ) return true;
	if( path[0] == '/' ) return true;
	return false;

#else
	#error
#endif
}

axStatus	axFilePath::getBaseName	( axIStringA &out, const char*    path, bool with_ext ) { return _getBaseName(out,path,with_ext); }
axStatus	axFilePath::getBaseName	( axIStringW &out, const wchar_t* path, bool with_ext ) { return _getBaseName(out,path,with_ext); }

axStatus	axFilePath::getDirName	( axIStringA &out, const char*    path )	{ return _getDirName(out,path); }
axStatus	axFilePath::getDirName	( axIStringW &out, const wchar_t* path )	{ return _getDirName(out,path); }

axStatus	axFilePath::getFileExt	( axIStringA &out, const char*	  path )	{ return _getFileExt(out,path); }
axStatus	axFilePath::getFileExt	( axIStringW &out, const wchar_t* path )	{ return _getFileExt(out,path); }

bool		axFilePath::isAbsolute	( const char*    path )  { return _isAbsolute(path); }
bool		axFilePath::isAbsolute	( const wchar_t* path )  { return _isAbsolute(path); }



#if 0
#pragma mark ================= Windows ====================
#endif
#if axOS_WIN

axStatus	axFilePath::getAbsolute ( axIStringA &out, const wchar_t* path ) {
	wchar_t fullpath[MAX_PATH+1];

	DWORD ret = ::GetFullPathName( path, MAX_PATH, fullpath, NULL );
	if( ret == 0 || ret >= MAX_PATH ) {
		out.clear();
		return -1;
	}
	
	return out.set( fullpath );
}

#endif //axOS_Win


#if 0
#pragma mark ================= UNIX ====================
#endif
#if axOS_UNIX

axStatus	axFilePath::getAbsolute ( axIStringA &out, const char* path ) {
	char tmp[PATH_MAX+1];
	if( realpath( path, tmp ) == nullptr ) {
		out.clear();
		return -1;
	}
	return out.set( tmp );
}

#endif //axOS_UNIX



