#ifndef	__axString_h__
#define __axString_h__

#include "axIString.h"

//! \ingroup base_string
//@{

template< size_t LOCAL_BUF_SIZE >
class axStringA_ : public axIString_<char> {
	typedef	axIString_<char>	B;
public:
	axStringA_() : B(buf_) { buf_.setCapacityIncrement( kDefaultCapacityIncrement ); } 
	axStatus	onTake( axStringA_<LOCAL_BUF_SIZE> &src )	{  return ax_take( buf_, src.buf_ ); }
	void		setCapacityIncrement( axSize  n )			{ buf_.setCapacityIncrement(n); }
private:
	axArray<char, LOCAL_BUF_SIZE>	buf_;
};


template< size_t LOCAL_BUF_SIZE >
class axStringW_ : public axIString_<wchar_t> {
	typedef	axIString_<wchar_t>	B;
public:
	axStringW_() : B(buf_) { buf_.setCapacityIncrement( kDefaultCapacityIncrement ); } 
	axStatus	onTake( axStringW_<LOCAL_BUF_SIZE> &src )	{  return buf_.onTake( src.buf_ ); }
	void		setCapacityIncrement( axSize  n )			{ buf_.setCapacityIncrement(n); }
private:
	axArray<wchar_t, LOCAL_BUF_SIZE>	buf_;
};

template< size_t LOCAL_BUF_SIZE >
class axStringU_ : public axIString_<axUChar> {
	typedef	axIString_<axUChar>	B;
public:
	axStringU_() : B(buf_) { buf_.setCapacityIncrement( kDefaultCapacityIncrement ); } 
	axStatus	onTake( axStringU_<LOCAL_BUF_SIZE> &src )	{  return buf_.onTake( src.buf_ ); }
	void		setCapacityIncrement( axSize  n )			{ buf_.setCapacityIncrement(n); }
private:
	axArray<axUChar, LOCAL_BUF_SIZE>	buf_;
};

//------------------

template< class T, size_t LOCAL_BUF_SIZE = 0 >
class axString_ : public axIString_<T> {
	typedef	axIString_<T>	B;
public:
	axString_() : B(buf_) { buf_.setCapacityIncrement( B::kDefaultCapacityIncrement ); } 
	axStatus	onTake( axString_<T, LOCAL_BUF_SIZE>	&src )	{  return ax_take( buf_, src.buf_ ); }
	void		setCapacityIncrement( axSize  n )	{ buf_.setCapacityIncrement(n); }
private:
	axArray<T,LOCAL_BUF_SIZE>	buf_;
};

typedef	axString_<char>				axStringA;
typedef	axString_<wchar_t>			axStringW;
typedef	axString_<axUChar>			axStringU;

const size_t axTempString_localBufSize = 1024;
typedef axString_<char,		axTempString_localBufSize>		axTempStringA;
typedef axString_<wchar_t,	axTempString_localBufSize>		axTempStringW;
typedef axString_<axUChar,	axTempString_localBufSize>		axTempStringU;

//@}


template< class T > inline
axStatus	axIString_<T> :: replaceString ( const T* from, const T* to, axSize start_from, axSize count ) {
	axStatus st;

	if( start_from >= size() ) return -1;

	size_t c = 0;
	T* s = buf_.ptr() + start_from;
	size_t from_len = ax_strlen( from );
	size_t to_len   = ax_strlen( to );

	if( from_len == to_len ) {

		for( c=0; count == 0 || c < count ; c++ ) {
			s = ax_strstr( s, from );	if( !s ) break;
			ax_array_copy( s, to, to_len );
			s += from_len;
		}
		return 0;
	}

	for( c=0; count == 0 || c < count ; c++ ) {
		s = ax_strstr( s, from );		if( !s ) break;
		s += from_len;
	}

	axString_<T, 1024>	tmp;
	if( to_len > from_len ) {
		tmp.resize( size() + ( to_len - from_len ) * c );
	}else{
		tmp.resize( size() - ( from_len - to_len ) * c );
	}

	T* dst = tmp._getInternalBufferPtr();
	s = buf_.ptr() + start_from;
	T* last_s = s;


	for( c=0; count == 0 || c < count ; c++ ) {
		s = ax_strstr( last_s, from );		if( !s ) break;
		size_t n = s-last_s;
		ax_array_copy( dst, last_s, n );
		dst += n;
		s += from_len;

		ax_array_copy( dst, to, to_len );
		dst += to_len;

		last_s = s;
	}	

	ax_strcpy( dst, last_s );

	return set( tmp );
}


#include "axStringFormat_out_Imp.h"

#endif //__axString_h__
