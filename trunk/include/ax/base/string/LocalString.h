#ifndef __ax_LocalString_h__
#define __ax_LocalString_h__

#include "xString.h"

namespace ax {

template< class T, Size LOCAL_BUF_SIZE >
class LocalString_ : public xString<T> {
	typedef	xString<T>	B;
public:
	LocalString_()	: xString<T>(buf_) {} 
private:
	LocalArray<T, LOCAL_BUF_SIZE>	buf_;
};

//-----------
template< Size LOCAL_BUF_SIZE >
class LocalStringA : public xString<char> {
	typedef	xString<char>	B;
public:
	LocalStringA()	: xString<char>(buf_) {} 
private:
	LocalArray<char, LOCAL_BUF_SIZE>	buf_;
};

//-----------
template< Size LOCAL_BUF_SIZE >
class LocalStringW : public xString<wchar_t> {
	typedef	xString<wchar_t>	B;
public:
	LocalStringW()	: xString<wchar_t>(buf_) {} 
private:
	LocalArray<wchar_t, LOCAL_BUF_SIZE>	buf_;
};




} //namespace ax 

#endif //__ax_LocalString_h__
