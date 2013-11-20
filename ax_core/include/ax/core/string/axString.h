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

			B & asInterface()		{ return (B&)*this; }
	const	B & asInterface() const	{ return (B&)*this; }

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

			B & asInterface()		{ return (B&)*this; }
	const	B & asInterface() const	{ return (B&)*this; }

private:
	axArray<wchar_t, LOCAL_BUF_SIZE>	buf_;
};

//------------------

template< class T, size_t LOCAL_BUF_SIZE = 64 >
class axString_ : public axIString_<T> {
	typedef	axIString_<T>	B;
public:
	axString_() : B(buf_) { buf_.setCapacityIncrement( B::kDefaultCapacityIncrement ); } 
	axStatus	onTake( axString_<T, LOCAL_BUF_SIZE>	&src )	{  return ax_take( buf_, src.buf_ ); }
	void		setCapacityIncrement( axSize  n )	{ buf_.setCapacityIncrement(n); }

			B & asInterface()		{ return (B&)*this; }
	const	B & asInterface() const	{ return (B&)*this; }

private:
	axArray<T,LOCAL_BUF_SIZE>	buf_;
};

typedef	axString_<char>				axStringA;
typedef	axString_<wchar_t>			axStringW;

const size_t axTempString_localBufSize = 128;
typedef axString_<char,		axTempString_localBufSize>		axTempStringA;
typedef axString_<wchar_t,	axTempString_localBufSize>		axTempStringW;

template< class T, size_t LOCAL_BUF_SIZE > inline uint32_t ax_hash_code( const axString_<T, LOCAL_BUF_SIZE> & v ) { return ax_hash_code( v.c_str() ); }
template< class T, size_t LOCAL_BUF_SIZE > inline uint32_t ax_hash_code(       axString_<T, LOCAL_BUF_SIZE> & v ) { return ax_hash_code( v.c_str() ); }

template< class T, size_t LOCAL_BUF_SIZE >inline axStatus ax_copy( axString_<T,LOCAL_BUF_SIZE> & dst, const axString_<T,LOCAL_BUF_SIZE> & src ) { return dst.copy( src ); }

//@}


#endif //__axString_h__
