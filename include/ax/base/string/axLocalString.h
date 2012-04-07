#ifndef __axLocalString_h__
#define __axLocalString_h__

#include "axIString.h"

//! \ingroup base_string
//@{

template< class T, size_t LOCAL_BUF_SIZE >
class axLocalString_ : public axIString_<T> {
	typedef	axIString_<T>	B;
public:
	axLocalString_()	: axIString_<T>(buf_) {}
private:
	axLocalArray<T, LOCAL_BUF_SIZE>	buf_;
};

//-----------
template< size_t LOCAL_BUF_SIZE >
class axLocalStringA : public axIString_<char> {
	typedef	axIString_<char>	B;
public:
	axLocalStringA()	: axIString_<char>(buf_) {}
private:
	axLocalArray<char, LOCAL_BUF_SIZE>	buf_;
};

//-----------
template< size_t LOCAL_BUF_SIZE >
class axLocalStringW : public axIString_<wchar_t> {
	typedef	axIString_<wchar_t>	B;
public:
	axLocalStringW()	: axIString_<wchar_t>(buf_) {}
private:
	axLocalArray<wchar_t, LOCAL_BUF_SIZE>	buf_;
};

//-----------
template< size_t LOCAL_BUF_SIZE >
class axLocalStringU : public axIString_<axUChar> {
	typedef	axIString_<axUChar>	B;
public:
	axLocalStringU()	: axIString_<axUChar>(buf_) {}
private:
	axLocalArray<axUChar, LOCAL_BUF_SIZE>	buf_;
};


//@}

#endif //__axLocalString_h__
