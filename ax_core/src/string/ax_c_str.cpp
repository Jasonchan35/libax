//
//  ax_c_str.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/string/ax_c_str.h>

template<class T>
bool	_ax_str_has_prefix( const T* sz, const T* prefix ) {
	size_t	prefix_len = ax_strlen( prefix );
	return 0 == ax_strncmp( prefix, sz, prefix_len );
}

bool ax_str_has_prefix( const char*    sz, const char*    prefix ) { return _ax_str_has_prefix(sz,prefix); }
bool ax_str_has_prefix( const wchar_t* sz, const wchar_t* prefix ) { return _ax_str_has_prefix(sz,prefix); }

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

size_t ax_strnlen( const char*    s, size_t len ) { return _ax_strnlen(s,len); }
size_t ax_strnlen( const wchar_t* s, size_t len ) { return _ax_strnlen(s,len); }


template<class T> inline
size_t _ax_strlen( const T* s ) {
	if( !s ) return 0;
	size_t len = 0;
	for( ; *s; s++, len++ ) {
	}
	return len;
}

size_t ax_strlen( const char*    s ) { return _ax_strlen(s); }
size_t ax_strlen( const wchar_t* s ) { return _ax_strlen(s); }

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
      char*    ax_strchr_list(       char*    sz, const char*    chrs ) { return _ax_strchrs(sz,(char*   )chrs); }
      wchar_t* ax_strchr_list(       wchar_t* sz, const wchar_t* chrs ) { return _ax_strchrs(sz,(wchar_t*)chrs); }

const char*    ax_strchr_list( const char*    sz, const char*    chrs ) { return _ax_strchrs(sz,chrs); }
const wchar_t* ax_strchr_list( const wchar_t* sz, const wchar_t* chrs ) { return _ax_strchrs(sz,chrs); }


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

      char*    ax_strrchrs(       char*    sz, const char*    chrs ) { return _ax_strrchrs(sz,(char*   )chrs); }
      wchar_t* ax_strrchrs(       wchar_t* sz, const wchar_t* chrs ) { return _ax_strrchrs(sz,(wchar_t*)chrs); }

const char*    ax_strrchrs( const char*    sz, const char*    chrs ) { return _ax_strrchrs(sz,chrs); }
const wchar_t* ax_strrchrs( const wchar_t* sz, const wchar_t* chrs ) { return _ax_strrchrs(sz,chrs); }


template<class T> inline
int	 _ax_strcasecmp ( const T* s1, const T* s2 ) {
	for( ;; s1++, s2++ ) {
		T c1 = ax_toupper(*s1);
		T c2 = ax_toupper(*s2);
		if( c1 != c2 ) return (c1-c2);
		if( c1 == 0 || c2 == 0 ) break;
	}
	return 0;
}

int	 ax_strcasecmp ( const char*    s1, const char*    s2 ) { return _ax_strcasecmp(s1,s2); }
int	 ax_strcasecmp ( const wchar_t* s1, const wchar_t* s2 ) { return _ax_strcasecmp(s1,s2); }


template<class T> inline
int	 _ax_strncasecmp( const T* s1, const T* s2, size_t n ) {
	size_t i;
	for( i=0; i<n; s1++, s2++, i++ ) {
		T c1 = ax_toupper(*s1);
		T c2 = ax_toupper(*s2);
		if( c1 != c2 ) return (c1-c2);
		if( c1 == 0 || c2 == 0 ) break;
	}
	return 0;
}

int	 ax_strncasecmp( const char*    s1, const char*    s2, size_t n ) { return _ax_strncasecmp(s1,s2,n); }
int	 ax_strncasecmp( const wchar_t* s1, const wchar_t* s2, size_t n ) { return _ax_strncasecmp(s1,s2,n); }

template<class T> inline
T* _ax_strcasechr( T* sz, T ch ) {
	T c = ax_toupper(ch);
	if( ! sz ) return NULL;
	for(;*sz;sz++) {
		if( ax_toupper(*sz) == c ) return sz;
	}
	return NULL;
}

      char* 	ax_strcasechr(       char*    sz, char    ch ) { return _ax_strcasechr(sz,ch); }
      wchar_t* 	ax_strcasechr(       wchar_t* sz, wchar_t ch ) { return _ax_strcasechr(sz,ch); }

const char* 	ax_strcasechr( const char*    sz, char    ch ) { return _ax_strcasechr((char*   )sz,ch); }
const wchar_t* 	ax_strcasechr( const wchar_t* sz, wchar_t ch ) { return _ax_strcasechr((wchar_t*)sz,ch); }


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

      char*    ax_strcasestr(       char*    big, const char*    little )	{ return _ax_strcasestr(big,(char*   )little); }
      wchar_t* ax_strcasestr(       wchar_t* big, const wchar_t* little )	{ return _ax_strcasestr(big,(wchar_t*)little); }

const char*    ax_strcasestr( const char*    big, const char*    little )	{ return _ax_strcasestr(big,little); }
const wchar_t* ax_strcasestr( const wchar_t* big, const wchar_t* little )	{ return _ax_strcasestr(big,little); }


template<class T> inline
T*	_ax_strchr( T* sz, T ch ) {
	for( ; *sz; sz++ ) {
		if( *sz == ch ) return sz;
	}
	return NULL;
}

      char*	  ax_strchr(       char*    sz, char    ch ) { return _ax_strchr(sz,ch); }
      wchar_t* ax_strchr(       wchar_t* sz, wchar_t ch ) { return _ax_strchr(sz,ch); }

const char*	  ax_strchr( const char*    sz, char    ch ) { return _ax_strchr<const char   >(sz,ch); }
const wchar_t* ax_strchr( const wchar_t* sz, wchar_t ch ) { return _ax_strchr<const wchar_t>(sz,ch); }


template<class T> inline
int	 _ax_strcmp ( const T* s1, const T* s2 ) {
	for( ;; s1++, s2++ ) {
		if( *s1 != *s2 ) return (*s1-*s2);
		if( *s1 == 0 || *s2 == 0 ) break;
	}
	return 0;
}

int	 ax_strcmp ( const char*    s1, const char*    s2 ) { return _ax_strcmp(s1,s2); }
int	 ax_strcmp ( const wchar_t* s1, const wchar_t* s2 ) { return _ax_strcmp(s1,s2); }


template<class T> inline
int	 _ax_strncmp( const T* s1, const T* s2, size_t n ) {
	size_t i;
	for( i=0; i<n; s1++, s2++, i++ ) {
		if( *s1 != *s2 ) return (*s1-*s2);
		if( *s1 == 0 || *s2 == 0 ) break;
	}
	return 0;
}

int	 ax_strncmp( const char*    s1, const char*    s2, size_t n ) { return _ax_strncmp(s1,s2,n); }
int	 ax_strncmp( const wchar_t* s1, const wchar_t* s2, size_t n ) { return _ax_strncmp(s1,s2,n); }



template<class T> inline
T* _ax_strcpy( T* dst, const T* src ) {
  T *ret = dst;
  while ((*dst++ = *src++));
  return ret ;
}

char*    ax_strcpy( char*    dst, const char*    src ) { return _ax_strcpy(dst,src); }
wchar_t* ax_strcpy( wchar_t* dst, const wchar_t* src ) { return _ax_strcpy(dst,src); }


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

char*    ax_strncpy( char*    dst, const char*    src, size_t n ) { return _ax_strncpy(dst,src,n); }
wchar_t* ax_strncpy( wchar_t* dst, const wchar_t* src, size_t n ) { return _ax_strncpy(dst,src,n); }


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

      char*    ax_strrchr(       char*    sz, char    ch ) { return _ax_strrchr(sz,ch); }
      wchar_t* ax_strrchr(       wchar_t* sz, wchar_t ch ) { return _ax_strrchr(sz,ch); }

const char*    ax_strrchr( const char*    sz, char    ch ) { return _ax_strrchr<const char   >(sz,ch); }
const wchar_t* ax_strrchr( const wchar_t* sz, wchar_t ch ) { return _ax_strrchr<const wchar_t>(sz,ch); }


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

      char*    ax_strstr(       char*    a, const char*    b ) { return _ax_strstr(a,(char*   )b); }
      wchar_t* ax_strstr(       wchar_t* a, const wchar_t* b ) { return _ax_strstr(a,(wchar_t*)b); }

const char*    ax_strstr( const char*    a, const char*    b ) { return _ax_strstr(a,b); }
const wchar_t* ax_strstr( const wchar_t* a, const wchar_t* b ) { return _ax_strstr(a,b); }


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

      char*    ax_strrstr(       char*    a, const char*    b ) { return _ax_strrstr(a,(char*   )b); }
      wchar_t* ax_strrstr(       wchar_t* a, const wchar_t* b ) { return _ax_strrstr(a,(wchar_t*)b); }

const char*    ax_strrstr( const char*    a, const char*    b ) { return _ax_strrstr(a,b); }
const wchar_t* ax_strrstr( const wchar_t* a, const wchar_t* b ) { return _ax_strrstr(a,b); }

