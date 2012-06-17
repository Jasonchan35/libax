//
//  axTinyString.h
//  ax_core
//
//  Created by Jason Chan on 2012-06-11.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ax_core_axTinyString_h
#define ax_core_axTinyString_h

#include "ax_utf8.h"
#include "axStringFormat.h"

template<class T, size_t LOCAL_BUF_SIZE>
class axTinyString_ : public axNonCopyable {
public:
	axTinyString_() { p_ = NULL; if( LOCAL_BUF_SIZE ) buf_[0] = 0; }
	axStatus	set		( const T* sz );
	void		clear	();	
	const T* 	c_str	() const;

	operator const T* () const { return c_str(); }	
	
	axStatus	toStringFormat( axStringFormat & f ) const { return axStringFormat_out( f, c_str() ); }
	axStatus	onTake( axTinyString_ & src ) {
		axStatus st = set( src.c_str() );	if( !st ) return st;
		src.clear();
		return 0;
	}
private:
	T*	p_;
	T 	buf_[LOCAL_BUF_SIZE];
};

template< size_t LOCAL_BUF_SIZE >
class axTinyStringA : public axTinyString_<char, LOCAL_BUF_SIZE> {
};

template< size_t LOCAL_BUF_SIZE >
class axTinyStringW : public axTinyString_<wchar_t, LOCAL_BUF_SIZE> {
};

//-----------

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axTinyString_<T,LOCAL_BUF_SIZE>::set( const T* sz )	{
	clear();
	size_t	n = ax_strlen(sz);
	if( n == 0 ) return 0;
	n++; //for zero-end
	if( n > LOCAL_BUF_SIZE ) {
		p_ = new T[n];
		memcpy( p_,   sz, sizeof(T) * n );
	}else{
		p_ = buf_;
		memcpy( buf_, sz, sizeof(T) * n);
	}
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
void		axTinyString_<T,LOCAL_BUF_SIZE>::clear() {
	if( p_ != buf_ ) delete[] p_; 
	if( LOCAL_BUF_SIZE > 0 ) {
		p_ = buf_;
		buf_[0] = 0;
	}else{
		p_ = NULL;
	}
}

template<class T, size_t LOCAL_BUF_SIZE> inline
const T* 	axTinyString_<T,LOCAL_BUF_SIZE>::c_str() const {
	if( p_ ) return p_;
	return "";
}

#endif
