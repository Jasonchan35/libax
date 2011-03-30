#ifndef	__ax_String_h__
#define __ax_String_h__

#include "xString.h"

namespace ax {
//! \addtogroup string
//@{

template< Size LOCAL_BUF_SIZE >
class StringA_ : public xString<char> {
	typedef	xString<char>	B;
public:
	StringA_() : xString<char>(buf_) { buf_.setChunkSize( defaultChunkSize ); } 
private:
	Array<char, LOCAL_BUF_SIZE>	buf_;
};

template< Size LOCAL_BUF_SIZE >
class StringW_ : public xString<wchar_t> {
	typedef	xString<wchar_t>	B;
public:
	StringW_() : xString<wchar_t>(buf_) { buf_.setChunkSize( defaultChunkSize ); } 
private:
	Array<wchar_t, LOCAL_BUF_SIZE>	buf_;
};

//------------------

template< class T >
class String_ : public xString<T> {
	typedef	xString<T>	B;
public:
	String_() : xString<T>(buf_) { buf_.setChunkSize( B::defaultChunkSize ); } 
private:
	Array<T>	buf_;
};

typedef	String_<char>				StringA;
typedef	String_<wchar_t>			StringW;


//@}
} //namespace ax

#include "StringFormat_out_Imp.h"

#endif //__ax_String_h__
