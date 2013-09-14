//
//  axFilePath.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-23.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/file_system/axFilePath.h>
#include <ax/core/system/axLog.h>
#include <ax/core/common/ax_macro.h>

template<class T> const T* axFilePath_Seperators();

template<> const char*		axFilePath_Seperators<char>	  () { return  "/\\"; }
template<> const wchar_t*	axFilePath_Seperators<wchar_t>() { return L"/\\"; }

template<class T> inline
static axStatus	axFilePath_getFileExt( axIString_<T> &out, const T* path ) {
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
static axStatus	axFilePath_getBaseName( axIString_<T> &out, const T* path, bool with_ext ) {
	if( ! path ) { out.clear(); return 0; }
	axStatus st;
	
	const T* p = ax_strrchr_list( path, axFilePath_Seperators<T>() );
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
static axStatus	axFilePath_getDirName( axIString_<T> &out, const T* path ) {
	axStatus st;
	out.resize(0);
	if( ! path ) return 0;

	const T* p = ax_strrchr_list( path, axFilePath_Seperators<T>() );
	if( !p ) return 0;

	st = out.setWithLength( path, p-path+1 );		if( !st ) return st;
	return 0;
}

template<class T> inline
static bool	axFilePath_isAbsolute( const T* path ) {
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

axStatus	axFilePath::getBaseName	( axIStringA &out, const char*    path, bool with_ext ) { return axFilePath_getBaseName(out,path,with_ext); }
axStatus	axFilePath::getBaseName	( axIStringW &out, const wchar_t* path, bool with_ext ) { return axFilePath_getBaseName(out,path,with_ext); }

axStatus	axFilePath::getDirName	( axIStringA &out, const char*    path )	{ return axFilePath_getDirName(out,path); }
axStatus	axFilePath::getDirName	( axIStringW &out, const wchar_t* path )	{ return axFilePath_getDirName(out,path); }

axStatus	axFilePath::getFileExt	( axIStringA &out, const char*	  path )	{ return axFilePath_getFileExt(out,path); }
axStatus	axFilePath::getFileExt	( axIStringW &out, const wchar_t* path )	{ return axFilePath_getFileExt(out,path); }

bool		axFilePath::isAbsolute	( const char*    path )  { return axFilePath_isAbsolute(path); }
bool		axFilePath::isAbsolute	( const wchar_t* path )  { return axFilePath_isAbsolute(path); }

template<class T> inline
static axStatus	axFilePath_getNormalizePath( axIString_<T> &out, const T* path ) {
	out.resize(0);
	if( path == NULL ) return 0;
	size_t n = ax_strlen( path );
	T lastCh = path[n-1];

	axStatus st;
	axString_<T>	dot;	dot.set(".");
	axString_<T>	dot2;	dot2.set("..");

	axString_Array<T>	folders;
	axString_Array<T>	newPath;

	const T* sp = axFilePath_Seperators<T>();
	st = folders.split( path, sp );	if( !st ) return st;

	axForArray( axIString_<T>, p, folders ) {
		if( p->size() == 0 ) continue;
		if( p->equals( dot ) )	continue;
		if( p->equals( dot2 ) ) {
			if( newPath.size() != 0 ) {
				if( newPath.last().equals( dot2 ) ) return -1;
				newPath.decSize(1);
				continue;
			}
		}
		st = newPath.addString( *p );		if( !st ) return st;
	}

	size_t i=0;
	axForArray( axIString_<T>, p, newPath ) {
		if( i > 0 ) {
			st = out.append('/');		if( !st ) return st;
		}
		st = out.append( *p );			if( !st ) return st;
		i++;
	}

	if( ax_strchr( sp, lastCh ) ) {
		st = out.append('/');			if( !st ) return st;
	}

	return 0;
}

axStatus axFilePath::getNormalizePath( axIStringA &out, const char*		path ) { return axFilePath_getNormalizePath( out, path ); }
axStatus axFilePath::getNormalizePath( axIStringW &out, const wchar_t*	path ) { return axFilePath_getNormalizePath( out, path ); }

template<class T> inline
static axStatus	axFilePath_getParentDir( axIString_<T> &out, const T* path ) {
	axStatus st;
	axString_<T, 256>	tmp;

	out.resize(0);

	st = axFilePath_getNormalizePath( tmp, path );		if( !st ) return st;
	if( tmp.size() == 0 ) return -1;

	T ch = tmp.lastChar(0);
	const T* sp = axFilePath_Seperators<T>();

	if( ax_strchr( sp, ch ) ) {
		st = tmp.decSize(1);			if( !st ) return st;
	}

	axSize outPos;
	if( ! tmp.findAnyCharFromEnd( sp, outPos ) ) return 0; // not parent dir

	return out.setWithLength( tmp, outPos+1 );
}

axStatus axFilePath::getParentDir( axIStringA &out, const char*		path ) { return axFilePath_getParentDir( out, path ); }
axStatus axFilePath::getParentDir( axIStringW &out, const wchar_t*	path ) { return axFilePath_getParentDir( out, path ); }

#if 0
#pragma mark ================= Windows ====================
#endif
#if axOS_WIN

axStatus	axFilePath::getFullPath ( axIStringW &out, const wchar_t* path ) {
	wchar_t fullpath[MAX_PATH+1];

	DWORD ret = ::GetFullPathName( path, MAX_PATH, fullpath, NULL );
	if( ret == 0 || ret >= MAX_PATH ) {
		out.clear();
		return -1;
	}
	
	return out.set( fullpath );
}

axStatus	axFilePath::getFullPath ( axIStringA &out, const char* path ) {
	axTempStringW	outTmp;
	axTempStringW	pathTmp;
	axStatus st;
	st = pathTmp.set( path );				if( !st ) return st;
	st = getFullPath( outTmp, pathTmp );	if( !st ) return st;
	st = out.set( outTmp );
	return 0;
}

#endif //axOS_Win


#if 0
#pragma mark ================= UNIX ====================
#endif
#if axOS_UNIX

axStatus	axFilePath::getFullPath ( axIStringW &out, const wchar_t* path ) {
	axTempStringA	tmpOut;
	axTempStringA	tmpPath;
	axStatus st;
	st = tmpPath.set( path );				if( !st ) return st;
	st = getFullPath( tmpOut, tmpPath );	if( !st ) return st;
	return out.set( tmpOut );
}

axStatus	axFilePath::getFullPath ( axIStringA &out, const char* path ) {
	char tmp[PATH_MAX+1];
	if( realpath( path, tmp ) == NULL ) {
		out.clear();
		return -1;
	}
	return out.set( tmp );
}

#endif //axOS_UNIX



