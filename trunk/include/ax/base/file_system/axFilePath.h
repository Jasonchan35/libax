#ifndef __axFilePath_h__
#define __axFilePath_h__

#include "../string/ax_string.h"

template<class T>
class axFilePath_ : public axNonCopyable {
public:
	axStatus	set( const T* full_path );
	
	const T*	dir				() const; //!< directory
	const T*	filename		() const; //!< filename with extension but without directory part
	const T*	ext				() const; //!< file extension
	axStatus	getBasename		( axIStringW &out ) const; //!< filename without extension

	axStatus	getFullPath		( axIString_<T>	&out );

	axStatus	appendDir		( const T* sz );
	axStatus	removeDir		( axSize index );
	axStatus	removeLastDir	();

	axStatus	normalize		();

	axStatus	makeAbsolute	();
	axStatus	makeRelativeTo	( const T* sz );

	axStatus	setDir			( const T* sz );
	axStatus	setFilename		( const T* sz );
	axStatus	setBasename		( const T* sz ); //!< filename without extension
	axStatus	setExt			( const T* sz );

	void		clearDir		();
	void		clearFilename	();
	void		clearExt		();

	void		clear			();

	static	const	T*	dirSeparatorList;

#if axOS_WIN
	enum { kNativeSeparator = '\\' };
#else
	enum { kNativeSeparator = '/' };
#endif

private:
	axString_<T,64>		dir_;
	axString_<T,32>		filename_;
};


typedef axFilePath_<char>		axFilePathA;
typedef axFilePath_<wchar_t>	axFilePathW;

template<>	const char*		axFilePath_<char>	:: dirSeparatorList	= "/\\";
template<>	const wchar_t*	axFilePath_<wchar_t>:: dirSeparatorList	= L"/\\";


//------- inline ----
template<class T> inline
axStatus	axFilePath_<T>::set( const T* full_path ) {
	axStatus	st;
	axSize	len = ax_strlen( full_path );
	const T* sep = ax_strrchrs( full_path, dirSeparatorList );
	if( sep ) {
		st = dir_.set( full_path, sep - full_path );	if( !st ) return st;
		st = filename_.set( sep+1 );					if( !st ) return st;
	}else{
		dir_.clear();
		st = filename_.set( full_path );				if( !st ) return st;
	}
	return 0;
}

template<class T> inline	void axFilePath_<T>::clear()			{ dir_.clear(); filename_.clear(); }
template<class T> inline	void axFilePath_<T>::clearDir()			{ dir_.clear(); }
template<class T> inline	void axFilePath_<T>::clearFilename()	{ filename_.clear(); }

template<class T> inline	axStatus axFilePath_<T>::setDir		( const T* sz )	{ return dir_.set( sz ); }
template<class T> inline	axStatus axFilePath_<T>::setFilename( const T* sz )	{ return filename_.set( sz ); }


template<class T> inline
const T* axFilePath_<T>::dir() const {
	return dir_.c_str();
}

template<class T> inline
const T* axFilePath_<T>::filename() const {
	return filename_.c_str();
}


#endif //__axFilePath_h__
