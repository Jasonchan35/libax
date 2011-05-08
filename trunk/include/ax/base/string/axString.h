#ifndef	__axString_h__
#define __axString_h__

#include "axIString.h"

//! \ingroup base_string
//@{

template< size_t LOCAL_BUF_SIZE >
class axStringA_ : public axIString_<char> {
	typedef	axIString_<char>	B;
public:
	axStringA_() : B(buf_) { buf_.setChunkSize( defaultChunkSize ); } 
private:
	axArray<char, LOCAL_BUF_SIZE>	buf_;
};


template< size_t LOCAL_BUF_SIZE >
class axStringW_ : public axIString_<wchar_t> {
	typedef	axIString_<wchar_t>	B;
public:
	axStringW_() : B(buf_) { buf_.setChunkSize( defaultChunkSize ); } 
private:
	axArray<wchar_t, LOCAL_BUF_SIZE>	buf_;
};

//------------------

template< class T, size_t LOCAL_BUF_SIZE = 0 >
class axString_ : public axIString_<T> {
	typedef	axIString_<T>	B;
public:
	axString_() : B(buf_) { buf_.setChunkSize( B::defaultChunkSize ); } 

	axStatus	takeOwnership( axString_<T>	&src )	{  return buf_.takeOwnership( src.buf_ ); }
private:
	axArray<T,LOCAL_BUF_SIZE>	buf_;
};

typedef	axString_<char>				axStringA;
typedef	axString_<wchar_t>			axStringW;

const size_t axTempString_localBufSize = 1024;
typedef axString_<char,		axTempString_localBufSize>		axTempStringA;
typedef axString_<wchar_t,	axTempString_localBufSize>		axTempStringW;

//@}

#include "axStringFormat_out_Imp.h"

#endif //__axString_h__
