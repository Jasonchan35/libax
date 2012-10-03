//
//  axTinyString.h
//  ax_core
//
//  Created by Jason Chan on 2012-06-11.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ax_core_axTinyString_h
#define ax_core_axTinyString_h

#include "axString.h"
#include "../other/axSerializer.h"

template<class T, size_t LOCAL_BUF_SIZE>
class axTinyString_ :private axLocalBuf< T, LOCAL_BUF_SIZE > {
	typedef	axLocalBuf< T, LOCAL_BUF_SIZE > BUF;
public:
	axTinyString_() 	{ p_ = NULL; if( LOCAL_BUF_SIZE ) BUF::_localBuf(0) = 0; }
	~axTinyString_() 	{ clear(); }
	
	axStatus	set				( const T* sz );
	axStatus	setWithLength	( const T* sz, size_t len );
	
	void		clear	();	
	const T* 	c_str	() const;

	operator const T* () const { return c_str(); }	
	
	axStatus	toStringFormat( axStringFormat & f ) const { return axStringFormat_out( f, c_str() ); }
	
	axStatus	onTake( axTinyString_ & src );
	axStatus	serialize_io( axSerializer 	 &se );
	axStatus	serialize_io( axDeserializer &se );	
	
private:
	T*	p_;
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
	return setWithLength( sz, ax_strlen(sz) );
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axTinyString_<T,LOCAL_BUF_SIZE>::setWithLength( const T* sz, size_t len )	{
	clear();
	if( len == 0 ) return 0;
	len++; //for zero-end
	if( len > LOCAL_BUF_SIZE ) {
		p_ = new T[len];
	}else{
		p_ = BUF::_localBufPtr();
	}
	memcpy( p_, sz, sizeof(T) * len );
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
void		axTinyString_<T,LOCAL_BUF_SIZE>::clear() {
	if( p_ != BUF::_localBufPtr() ) delete[] p_; 
	if( LOCAL_BUF_SIZE > 0 ) {
		p_ = BUF::_localBufPtr();
		BUF::_localBuf(0) = 0;
	}else{
		p_ = NULL;
	}
}

template<class T, size_t LOCAL_BUF_SIZE> inline
const T* 	axTinyString_<T,LOCAL_BUF_SIZE>::c_str() const {
	if( p_ ) return p_;
	return "";
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axTinyString_<T,LOCAL_BUF_SIZE>::onTake( axTinyString_ & src ) {
	axStatus st = set( src.c_str() );	if( !st ) return st;
	src.clear();
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axTinyString_<T,LOCAL_BUF_SIZE>::serialize_io( axSerializer 	 &se ) {
	axStatus st;
	axString_<T,256>	tmp;
	st = tmp.set( c_str() );	if( !st ) return st;
	st = se.io( tmp );			if( !st ) return st;
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axTinyString_<T,LOCAL_BUF_SIZE>::serialize_io( axDeserializer &se ) {
	axStatus st;
	axString_<T,256>	tmp;
	st = se.io( tmp );			if( !st ) return st;
	st = set( tmp.c_str() );	if( !st ) return st;
	return 0;
}



#endif
