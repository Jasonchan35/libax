#ifndef __axString_cout_support_h__
#define __axString_cout_support_h__

#include "axString.h"
#include "axConstString.h"
#include "axLocalString.h"

//axIString
inline	std::ostream&   operator <<( std::ostream  &o, const axIStringA &v ) { o << v.c_str(); return o; }
inline	std::ostream&   operator <<( std::ostream  &o, const axIStringW &v ) { o << v.c_str(); return o; }
inline	std::wostream&  operator <<( std::wostream &o, const axIStringA &v ) { o << v.c_str(); return o; }
inline	std::wostream&  operator <<( std::wostream &o, const axIStringW &v ) { o << v.c_str(); return o; }

//axString
template< size_t LOCAL_BUF_SIZE > inline	std::ostream&   operator <<( std::ostream  &o, const axStringA_<LOCAL_BUF_SIZE> &v ) { o << v.c_str(); return o; }
template< size_t LOCAL_BUF_SIZE > inline	std::ostream&   operator <<( std::ostream  &o, const axStringW_<LOCAL_BUF_SIZE> &v ) { o << v.c_str(); return o; }
template< size_t LOCAL_BUF_SIZE > inline	std::wostream&  operator <<( std::wostream &o, const axStringA_<LOCAL_BUF_SIZE> &v ) { o << v.c_str(); return o; }
template< size_t LOCAL_BUF_SIZE > inline	std::wostream&  operator <<( std::wostream &o, const axStringW_<LOCAL_BUF_SIZE> &v ) { o << v.c_str(); return o; }
template< class T, size_t LOCAL_BUF_SIZE > inline	std::ostream&   operator <<( std::ostream  &o, const axString_<T,LOCAL_BUF_SIZE> &v ) { o << v.c_str(); return o; }
template< class T, size_t LOCAL_BUF_SIZE > inline	std::wostream&  operator <<( std::wostream &o, const axString_<T,LOCAL_BUF_SIZE> &v ) { o << v.c_str(); return o; }

//axLocalString
template< class T, size_t LOCAL_BUF_SIZE > inline	std::ostream&   operator <<( std::ostream  &o, const axLocalString_<T,LOCAL_BUF_SIZE> &v ) { o << v.c_str(); return o; }
template< class T, size_t LOCAL_BUF_SIZE > inline	std::wostream&  operator <<( std::wostream &o, const axLocalString_<T,LOCAL_BUF_SIZE> &v ) { o << v.c_str(); return o; }

//axExternalString
template< class T > inline	std::ostream&   operator <<( std::ostream  &o, const axExternalString_<T> &v ) { o << v.c_str(); return o; }
template< class T > inline	std::wostream&  operator <<( std::wostream &o, const axExternalString_<T> &v ) { o << v.c_str(); return o; }


//axConstString
template< class T > inline	std::ostream&   operator <<( std::ostream  &o, const axConstString_<T> &v ) { o << v.c_str(); return o; }
template< class T > inline	std::wostream&  operator <<( std::wostream &o, const axConstString_<T> &v ) { o << v.c_str(); return o; }







#endif //__axString_cout_support_h__


