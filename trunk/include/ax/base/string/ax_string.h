#pragma once
#ifndef __ax_string_h__
#define __ax_string_h__

#include "ax_utf8.h"

//! \ingroup base_string
//@{

#ifdef axOS_WIN
	inline int	 strncasecmp( const char* s1, const char* s2, axSize n )			{ return _strnicmp(s1,s2,n); }
	inline int	 strcasecmp ( const char* s1, const char* s2 )					{ return _stricmp(s1,s2); }
	inline int	 wcsncasecmp( const wchar_t* s1, const wchar_t* s2, axSize n )	{ return _wcsnicmp(s1,s2,n); }
	inline int	 wcscasecmp ( const wchar_t* s1, const wchar_t* s2 )			{ return _wcsicmp(s1,s2); }
#endif //axOS_WIN

inline size_t ax_strlen( const char*    sz )	{ return strlen(sz); }
inline size_t ax_strlen( const wchar_t* sz )	{ return wcslen(sz); }

inline int ax_strcmp( const char*    a, const char* b )	{ return strcmp(a,b); }
inline int ax_strcmp( const wchar_t* a, const wchar_t* b )	{ return wcscmp(a,b); }

inline int ax_strncmp( const char*    a, const char* b, axSize n )		{ return strncmp(a,b,n); }
inline int ax_strncmp( const wchar_t* a, const wchar_t* b, axSize n )		{ return wcsncmp(a,b,n); }

inline int ax_strcasecmp( const char*    a, const char* b )				{ return strcasecmp(a,b); }
inline int ax_strcasecmp( const wchar_t* a, const wchar_t* b );

inline int ax_strncasecmp( const char*    a, const char* b, axSize n )	{ return strncasecmp(a,b,n); }
inline int ax_strncasecmp( const wchar_t* a, const wchar_t* b, axSize n );

inline char*	ax_strncpy( char*    dest, const char*    src, axSize n ) { return strncpy(dest,src,n); }
inline wchar_t* ax_strncpy( wchar_t* dest, const wchar_t* src, axSize n ) { return wcsncpy(dest,src,n); }

inline const char*	  ax_strchr( const char*    sz, const char    ch )			{ if(!sz || !ch  ) return NULL; return strchr(sz,ch); }
inline const wchar_t* ax_strchr( const wchar_t* sz, const wchar_t ch )			{ if(!sz || !ch  ) return NULL; return wcschr(sz,ch); }

inline const char*	  ax_strrchr( const char*    sz, const char    ch )		{ if(!sz || !ch  ) return NULL; return strrchr(sz,ch); }
inline const wchar_t* ax_strrchr( const wchar_t* sz, const wchar_t ch )		{ if(!sz || !ch  ) return NULL; return wcsrchr(sz,ch); }

inline const char*	  ax_strstr( const char*    a, const char*    b )			{ if(!a || !b) return NULL; return strstr(a,b); }
inline const wchar_t* ax_strstr( const wchar_t* a, const wchar_t* b )			{ if(!a || !b) return NULL; return wcsstr(a,b); }

inline int ax_isalpha( char     ch ) { return isalpha(ch); }
inline int ax_isalpha( wchar_t  ch ) { return iswalpha(ch); }

inline int ax_isdigit( char     ch ) { return isdigit(ch); }
inline int ax_isdigit( wchar_t  ch ) { return iswdigit(ch); }

inline
wchar_t* ax_strcasestr( const wchar_t *big, const wchar_t* little ) {
	size_t big_len    = ax_strlen( big );
	size_t little_len = ax_strlen( little );
	
	if( little_len > big_len ) return NULL;
	
	size_t n = big_len - little_len;
	size_t i;
	for( i=0; i<=n; i++ ) {
		if( ax_strncasecmp( big+i, little, little_len ) == 0 )
			return (wchar_t*) big+i;
	}
	return NULL;
}


//! search char(s) in string
inline
const char* ax_strchrs( const char* sz, const char* chrs ) {
	const char *c;
	for( ; *sz;	sz++ ) {
		for( c=chrs; *c; c++ ) {
			if( *sz == *c ) return sz;
		}
	}
	return NULL;
}

//! search char(s) in string from right side
inline
const char* ax_strrchrs( const char* sz, const char* chrs ) {
	if( !*sz ) return NULL;
	const char* start = sz;
	for( ; *sz; sz++ ) {
		//go to the end, so do nothing
	}
	sz--;

	const char *c;
	for( ; sz >= start; sz-- ) {
		for( c=chrs; *c; c++ ) {
			if( *sz == *c ) return sz;
		}
	}
	return NULL;
}

//! search wchar(s) in string
inline
const wchar_t* ax_strchrs( const wchar_t* sz, const wchar_t* chrs ) {
	const wchar_t *c;
	for( ; *sz;	sz++ ) {
		for( c=chrs; *c; c++ ) {
			if( *sz == *c ) return sz;
		}
	}
	return NULL;
}

//! search wchar(s) in string from right side
inline
const wchar_t* ax_strrchrs( const wchar_t* sz, const wchar_t* chrs ) {
	if( !*sz ) return NULL;
	const wchar_t* start = sz;
	for( ; *sz; sz++ ) {
		//go to the end, so do nothing
	}
	sz--;

	const wchar_t *c;
	for( ; sz >= start; sz-- ) {
		for( c=chrs; *c; c++ ) {
			if( *sz == *c ) return sz;
		}
	}
	return NULL;
}


inline
char* ax_strcasestr( const char* big, const char* little ) {
	size_t big_len    = ax_strlen( big );
	size_t little_len = ax_strlen( little );
	
	if( little_len > big_len ) return NULL;
	
	size_t n = big_len - little_len;
	size_t i;
	for( i=0; i<n; i++ ) {
		if( ax_strncasecmp( big+i, little, little_len ) == 0 )
			return (char*) big+i;
	}
	return NULL;
}

inline
int	 ax_strcasecmp ( const wchar_t* s1, const wchar_t* s2 ) {
	for( ;; s1++, s2++ ) {
		if( towupper(*s1) != towupper(*s2) ) return (*s1 - *s2);
		if( *s1 == 0 || *s2 == 0 ) break;
	}
	return 0;
}

inline
int	 ax_strncasecmp( const wchar_t* s1, const wchar_t* s2, axSize n ) {
	axSize i;
	for( i=0; i<n; s1++, s2++, i++ ) {
		if( towupper(*s1) != towupper(*s2) ) return (*s1 - *s2);
		if( *s1 == 0 || *s2 == 0 ) break;
	}
	return 0;
}

inline	const char*		ax_empty_c_str( const char*	  foo ) { return ""; }
inline	const wchar_t*	ax_empty_c_str( const wchar_t* foo ) { return L""; }


inline
axSize ax_strnlen( const char* s, axSize len ) {
	axSize l=0;
	for(;;) {
		if( len <= l ) return len;
		if( *s == 0 ) return l;
		l++;
		s++;
	}
	return len;
}

inline
axSize ax_strnlen( const wchar_t* s, axSize len ) {
	axSize l=0;
	for(;;) {
		if( len <= l ) return len;
		if( *s == 0 ) return l;
		l++;
		s++;
	}
	return len;
}

//@}

#include "ax_str_to.h"

#endif //__ax_string_h__

