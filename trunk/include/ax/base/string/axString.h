#ifndef	__axString_h__
#define __axString_h__

#include "axIString.h"

//! \addtogroup string
//@{

template< size_t LOCAL_BUF_SIZE >
class axStringA_ : public axIString<char> {
	typedef	axIString<char>	B;
public:
	axStringA_() : B(buf_) { buf_.setChunkSize( defaultChunkSize ); } 
private:
	axArray<char, LOCAL_BUF_SIZE>	buf_;
};

template< size_t LOCAL_BUF_SIZE >
class axStringW_ : public axIString<wchar_t> {
	typedef	axIString<wchar_t>	B;
public:
	axStringW_() : B(buf_) { buf_.setChunkSize( defaultChunkSize ); } 
private:
	axArray<wchar_t, LOCAL_BUF_SIZE>	buf_;
};

//------------------

template< class T >
class axString_ : public axIString<T> {
	typedef	axIString<T>	B;
public:
	axString_() : B(buf_) { buf_.setChunkSize( B::defaultChunkSize ); } 
private:
	axArray<T>	buf_;
};

typedef	axString_<char>				axStringA;
typedef	axString_<wchar_t>			axStringW;

//@}

#include "axStringFormat_out_Imp.h"

#endif //__axString_h__
