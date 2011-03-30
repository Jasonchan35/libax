#ifndef __axLocalString_h__
#define __axLocalString_h__

#include "axIString.h"


template< class T, axSize LOCAL_BUF_SIZE >
class axLocalString_ : public axIString<T> {
	typedef	axIString<T>	B;
public:
	axLocalString_()	: axIString<T>(buf_) {} 
private:
	axLocalArray<T, LOCAL_BUF_SIZE>	buf_;
};

//-----------
template< axSize LOCAL_BUF_SIZE >
class axLocalStringA : public axIString<char> {
	typedef	axIString<char>	B;
public:
	axLocalStringA()	: axIString<char>(buf_) {} 
private:
	axLocalArray<char, LOCAL_BUF_SIZE>	buf_;
};

//-----------
template< axSize LOCAL_BUF_SIZE >
class axLocalStringW : public axIString<wchar_t> {
	typedef	axIString<wchar_t>	B;
public:
	axLocalStringW()	: axIString<wchar_t>(buf_) {} 
private:
	axLocalArray<wchar_t, LOCAL_BUF_SIZE>	buf_;
};

#endif //__axLocalString_h__
