#ifndef __axFilePath_h__
#define __axFilePath_h__

#include "../string/ax_string.h"

class axFilePath : public axNonCopyable {
public:
	static axStatus	getBaseName	( axIStringA &out, const char*    path, bool with_ext = true ) { return _getBaseName(out,path,with_ext); }
	static axStatus	getBaseName	( axIStringW &out, const wchar_t* path, bool with_ext = true ) { return _getBaseName(out,path,with_ext); }
	
	static axStatus	getDirName	( axIStringA &out, const char*    path )		{ return _getDirName(out,path); }
	static axStatus	getDirName	( axIStringW &out, const wchar_t* path )		{ return _getDirName(out,path); }

	static axStatus	getFileExt	( axIStringA &out, const char*	  path )		{ return _getFileExt(out,path); }
	static axStatus	getFileExt	( axIStringW &out, const wchar_t* path )		{ return _getFileExt(out,path); }
	
	static bool		isAbsolute	( const char*    path )  { return _isAbsolute(path); }
	static bool		isAbsolute	( const wchar_t* path )  { return _isAbsolute(path); }

private:
	template<class T> static axStatus	_getFileExt	( axIString_<T>	&out, const T* path );
	template<class T> static axStatus	_getBaseName( axIString_<T>	&out, const T* path, bool with_ext = true );
	template<class T> static axStatus	_getDirName	( axIString_<T>	&out, const T* path );
	template<class T> static bool		_isAbsolute	( const T* path );
};

//=== inline ====

template<class T> inline
axStatus	axFilePath::_getFileExt( axIString_<T> &out, const T* path ) {
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
axStatus	axFilePath::_getBaseName( axIString_<T> &out, const T* path, bool with_ext ) {
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
axStatus	axFilePath::_getDirName( axIString_<T> &out, const T* path ) {
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
bool	axFilePath::_isAbsolute( const T* path ) {
	if( !path ) return false;

#if axOS_UNIX //Cygwin also count as Unix
	if( path[0] == '/' ) return true;
	return false;

#elif axOS_WIN
	if( ax_isalpha(path[0]) && path[1] == ':' ) return true;
	return false;

#else
	#error
#endif
}


#endif //__axFilePath_h__
