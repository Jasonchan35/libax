#pragma once
#ifndef __ax_c_str_h__
#define __ax_c_str_h__

#include "../common/ax_common.h"

//! \ingroup base_string
//@{

typedef	uint16_t	axUChar;

inline const char * ax_HEX_chars() { return "0123456789ABCDEF"; }
inline const char * ax_hex_chars() { return "0123456789abcdef"; }

template<class T> inline
bool ax_ishex( T ch ) {
	if( ch >= '0' && ch <='9' ) return true;
	if( ch >= 'A' && ch <='F' ) return true;
	if( ch >= 'a' && ch <='f' ) return true;
	return false;
}

template<class T> inline bool	ax_isupper( T  ch ) { return ( ch>='A' && ch <= 'Z' ); }
template<class T> inline bool	ax_islower( T  ch ) { return ( ch>='a' && ch <= 'z' ); }

template<class T> inline bool	ax_isalpha( T  ch ) { return ax_isupper(ch) || ax_islower(ch); }
template<class T> inline bool	ax_isdigit( T  ch ) { return ( ch >= '0' && ch <='9' ); }

template<class T> inline int	ax_toupper( T  ch ) { return ax_islower(ch) ? (ch-('a'-'A')) : ch; }
template<class T> inline int	ax_tolower( T  ch ) { return ax_isupper(ch) ? (ch+('a'-'A')) : ch; }

template<class T> inline const T* ax_empty_c_str( const T* foo ) { return (T*)L""; }

template<class T> inline
size_t _ax_strnlen( const T* s, size_t len ) {
	if( !s ) return 0;
	size_t l=0;
	for(;;) {
		if( len <= l ) return len;
		if( *s == 0 ) return l;
		l++;
		s++;
	}
	return len;
}

inline size_t ax_strnlen( const char*    s, size_t len ) { return _ax_strnlen(s,len); }
inline size_t ax_strnlen( const wchar_t* s, size_t len ) { return _ax_strnlen(s,len); }
inline size_t ax_strnlen( const axUChar* s, size_t len ) { return _ax_strnlen(s,len); }


template<class T> inline
size_t _ax_strlen( const T* s ) {
	if( !s ) return 0;
	size_t len = 0;
	for( ; *s; s++, len++ ) {
	}
	return len;
}

inline size_t ax_strlen( const char*    s ) { return _ax_strlen(s); }
inline size_t ax_strlen( const wchar_t* s ) { return _ax_strlen(s); }
inline size_t ax_strlen( const axUChar* s ) { return _ax_strlen(s); }

//! search char(s) in string
template<class T> inline
T* _ax_strchrs( T* sz, T* chrs ) {
	const T *c;
	for( ; *sz;	sz++ ) {
		for( c=chrs; *c; c++ ) {
			if( *sz == *c ) return sz;
		}
	}
	return NULL;
}
inline       char*    ax_strchrs(       char*    sz, const char*    chrs ) { return _ax_strchrs(sz,(char*   )chrs); }
inline       wchar_t* ax_strchrs(       wchar_t* sz, const wchar_t* chrs ) { return _ax_strchrs(sz,(wchar_t*)chrs); }
inline       axUChar* ax_strchrs(       axUChar* sz, const axUChar* chrs ) { return _ax_strchrs(sz,(axUChar*)chrs); }

inline const char*    ax_strchrs( const char*    sz, const char*    chrs ) { return _ax_strchrs(sz,chrs); }
inline const wchar_t* ax_strchrs( const wchar_t* sz, const wchar_t* chrs ) { return _ax_strchrs(sz,chrs); }
inline const axUChar* ax_strchrs( const axUChar* sz, const axUChar* chrs ) { return _ax_strchrs(sz,chrs); }


//! search char(s) in string from right side
template<class T> inline
T* _ax_strrchrs( T* sz, T* chrs ) {
	if( !*sz ) return NULL;
	T* start = sz;
	for( ; *sz; sz++ ) {
		//go to the end, so do nothing
	}
	sz--;

	T *c;
	for( ; sz >= start; sz-- ) {
		for( c=chrs; *c; c++ ) {
			if( *sz == *c ) return sz;
		}
	}
	return NULL;
}

inline       char*    ax_strrchrs(       char*    sz, const char*    chrs ) { return _ax_strrchrs(sz,(char*   )chrs); }
inline       wchar_t* ax_strrchrs(       wchar_t* sz, const wchar_t* chrs ) { return _ax_strrchrs(sz,(wchar_t*)chrs); }
inline       axUChar* ax_strrchrs(       axUChar* sz, const axUChar* chrs ) { return _ax_strrchrs(sz,(axUChar*)chrs); }

inline const char*    ax_strrchrs( const char*    sz, const char*    chrs ) { return _ax_strrchrs(sz,chrs); }
inline const wchar_t* ax_strrchrs( const wchar_t* sz, const wchar_t* chrs ) { return _ax_strrchrs(sz,chrs); }
inline const axUChar* ax_strrchrs( const axUChar* sz, const axUChar* chrs ) { return _ax_strrchrs(sz,chrs); }


template<class T> inline
int	 _ax_strcasecmp ( const T* s1, const T* s2 ) {
	for( ;; s1++, s2++ ) {
		char c1 = ax_toupper(*s1);
		char c2 = ax_toupper(*s2);
		if( c1 != c2 ) return (c1-c2);
		if( c1 == 0 || c2 == 0 ) break;
	}
	return 0;
}

inline int	 ax_strcasecmp ( const char*    s1, const char*    s2 ) { return _ax_strcasecmp(s1,s2); }
inline int	 ax_strcasecmp ( const wchar_t* s1, const wchar_t* s2 ) { return _ax_strcasecmp(s1,s2); }
inline int	 ax_strcasecmp ( const axUChar* s1, const axUChar* s2 ) { return _ax_strcasecmp(s1,s2); }


template<class T> inline
int	 _ax_strncasecmp( const T* s1, const T* s2, size_t n ) {
	size_t i;
	for( i=0; i<n; s1++, s2++, i++ ) {
		char c1 = ax_toupper(*s1);
		char c2 = ax_toupper(*s2);
		if( c1 != c2 ) return (c1-c2);
		if( c1 == 0 || c2 == 0 ) break;
	}
	return 0;
}

inline int	 ax_strncasecmp( const char*    s1, const char*    s2, size_t n ) { return _ax_strncasecmp(s1,s2,n); }
inline int	 ax_strncasecmp( const wchar_t* s1, const wchar_t* s2, size_t n ) { return _ax_strncasecmp(s1,s2,n); }
inline int	 ax_strncasecmp( const axUChar* s1, const axUChar* s2, size_t n ) { return _ax_strncasecmp(s1,s2,n); }

template<class T> inline
T* _ax_strcasestr( T* big, T* little ) {
	size_t big_len    = ax_strlen( big );
	size_t little_len = ax_strlen( little );
	
	if( little_len > big_len ) return NULL;
	
	size_t n = big_len - little_len;
	size_t i;
	for( i=0; i<=n; i++ ) {
		if( ax_strncasecmp( big+i, little, little_len ) == 0 )
			return big+i;
	}
	return NULL;
}

inline       char*    ax_strcasestr(       char*    big, const char*    little )	{ return _ax_strcasestr(big,(char*   )little); }
inline       wchar_t* ax_strcasestr(       wchar_t* big, const wchar_t* little )	{ return _ax_strcasestr(big,(wchar_t*)little); }
inline       axUChar* ax_strcasestr(       axUChar* big, const axUChar* little )	{ return _ax_strcasestr(big,(axUChar*)little); }

inline const char*    ax_strcasestr( const char*    big, const char*    little )	{ return _ax_strcasestr(big,little); }
inline const wchar_t* ax_strcasestr( const wchar_t* big, const wchar_t* little )	{ return _ax_strcasestr(big,little); }
inline const axUChar* ax_strcasestr( const axUChar* big, const axUChar* little )	{ return _ax_strcasestr(big,little); }


template<class T> inline
T*	_ax_strchr( T* sz, T ch ) {
	for( ; *sz; sz++ ) {
		if( *sz == ch ) return sz;
	}
	return NULL;
}

inline       char*	  ax_strchr(       char*    sz, char    ch ) { return _ax_strchr(sz,ch); }
inline       wchar_t* ax_strchr(       wchar_t* sz, wchar_t ch ) { return _ax_strchr(sz,ch); }
inline       axUChar* ax_strchr(       axUChar* sz, axUChar ch ) { return _ax_strchr(sz,ch); }

inline const char*	  ax_strchr( const char*    sz, char    ch ) { return _ax_strchr<const char   >(sz,ch); }
inline const wchar_t* ax_strchr( const wchar_t* sz, wchar_t ch ) { return _ax_strchr<const wchar_t>(sz,ch); }
inline const axUChar* ax_strchr( const axUChar* sz, axUChar ch ) { return _ax_strchr<const axUChar>(sz,ch); }


template<class T> inline
int	 _ax_strcmp ( const T* s1, const T* s2 ) {
	for( ;; s1++, s2++ ) {
		if( *s1 != *s2 ) return (*s1-*s2);
		if( *s1 == 0 || *s2 == 0 ) break;
	}
	return 0;
}

inline int	 ax_strcmp ( const char*    s1, const char*    s2 ) { return _ax_strcmp(s1,s2); }
inline int	 ax_strcmp ( const wchar_t* s1, const wchar_t* s2 ) { return _ax_strcmp(s1,s2); }
inline int	 ax_strcmp ( const axUChar* s1, const axUChar* s2 ) { return _ax_strcmp(s1,s2); }


template<class T> inline
int	 _ax_strncmp( const T* s1, const T* s2, size_t n ) {
	size_t i;
	for( i=0; i<n; s1++, s2++, i++ ) {
		if( *s1 != *s2 ) return (*s1-*s2);
		if( *s1 == 0 || *s2 == 0 ) break;
	}
	return 0;
}

inline int	 ax_strncmp( const char*    s1, const char*    s2, size_t n ) { return _ax_strncmp(s1,s2,n); }
inline int	 ax_strncmp( const wchar_t* s1, const wchar_t* s2, size_t n ) { return _ax_strncmp(s1,s2,n); }
inline int	 ax_strncmp( const axUChar* s1, const axUChar* s2, size_t n ) { return _ax_strncmp(s1,s2,n); }



template<class T> inline
T* _ax_strcpy( T* dst, const T* src ) {
  T *ret = dst;
  while ((*dst++ = *src++));
  return ret ;
}

inline char*    ax_strcpy( char*    dst, const char*    src ) { return _ax_strcpy(dst,src); }
inline wchar_t* ax_strcpy( wchar_t* dst, const wchar_t* src ) { return _ax_strcpy(dst,src); }
inline axUChar* ax_strcpy( axUChar* dst, const axUChar* src ) { return _ax_strcpy(dst,src); }


template<class T> inline
T* _ax_strncpy( T* dst, const T* src, size_t n ) {
	size_t i;
	T* ret = dst;
	for( i=0; i<n; i++ ) {
		if( *src == 0 ) break;
		*dst = *src;
		dst++; src++;
	}	
	*dst = 0;
	return ret;
}

inline char*    ax_strncpy( char*    dst, const char*    src, size_t n ) { return _ax_strncpy(dst,src,n); }
inline wchar_t* ax_strncpy( wchar_t* dst, const wchar_t* src, size_t n ) { return _ax_strncpy(dst,src,n); }
inline axUChar* ax_strncpy( axUChar* dst, const axUChar* src, size_t n ) { return _ax_strncpy(dst,src,n); }


template<class T> inline
T* _ax_strrchr( T* sz, T ch ) {
	size_t len = ax_strlen( sz );
	if( len == 0 ) return NULL;
	T *s = &sz[ len-1 ];
	for( ; s > sz ; s-- ) {
		if( *s == ch ) return s;
	}
	return NULL;
}

inline       char*    ax_strrchr(       char*    sz, char    ch ) { return _ax_strrchr(sz,ch); }
inline       wchar_t* ax_strrchr(       wchar_t* sz, wchar_t ch ) { return _ax_strrchr(sz,ch); }
inline       axUChar* ax_strrchr(       axUChar* sz, axUChar ch ) { return _ax_strrchr(sz,ch); }

inline const char*    ax_strrchr( const char*    sz, char    ch ) { return _ax_strrchr<const char   >(sz,ch); }
inline const wchar_t* ax_strrchr( const wchar_t* sz, wchar_t ch ) { return _ax_strrchr<const wchar_t>(sz,ch); }
inline const axUChar* ax_strrchr( const axUChar* sz, axUChar ch ) { return _ax_strrchr<const axUChar>(sz,ch); }


template<class T> inline
T* _ax_strstr( T* a, T* b ) {
	T* sa = a;
	T* sb = b;
	if( *b == 0 ) return NULL;
	for( ; *a ; a++ ) {
		sa = a;
		sb = b;
		for(;;) {
			if( *sb == 0   ) return a; //found
			if( *sb != *sa ) break;
			sa++; sb++;
		}
	}
	return NULL;
}

inline       char*    ax_strstr(       char*    a, const char*    b ) { return _ax_strstr(a,(char*   )b); }
inline       wchar_t* ax_strstr(       wchar_t* a, const wchar_t* b ) { return _ax_strstr(a,(wchar_t*)b); }
inline       axUChar* ax_strstr(       axUChar* a, const axUChar* b ) { return _ax_strstr(a,(axUChar*)b); }

inline const char*    ax_strstr( const char*    a, const char*    b ) { return _ax_strstr(a,b); }
inline const wchar_t* ax_strstr( const wchar_t* a, const wchar_t* b ) { return _ax_strstr(a,b); }
inline const axUChar* ax_strstr( const axUChar* a, const axUChar* b ) { return _ax_strstr(a,b); }


template<class T> inline
T* _ax_strrstr( T* a, T* b ) {

	axSize alen, blen;

	T *p;

	alen = ax_strlen( a );
	blen = ax_strlen( b );
	
	if ( blen > alen ) return NULL;

	for ( p = a + alen - blen; p >= a; p-- ) {
		if ( ax_strncmp( p, b, blen ) == 0 ) {
			return p;
		}
	}

	return NULL;
}

inline       char*    ax_strrstr(       char*    a, const char*    b ) { return _ax_strrstr(a,(char*   )b); }
inline       wchar_t* ax_strrstr(       wchar_t* a, const wchar_t* b ) { return _ax_strrstr(a,(wchar_t*)b); }
inline       axUChar* ax_strrstr(       axUChar* a, const axUChar* b ) { return _ax_strrstr(a,(axUChar*)b); }

inline const char*    ax_strrstr( const char*    a, const char*    b ) { return _ax_strrstr(a,b); }
inline const wchar_t* ax_strrstr( const wchar_t* a, const wchar_t* b ) { return _ax_strrstr(a,b); }
inline const axUChar* ax_strrstr( const axUChar* a, const axUChar* b ) { return _ax_strrstr(a,b); }


//@}

#endif //__ax_c_str_h__

