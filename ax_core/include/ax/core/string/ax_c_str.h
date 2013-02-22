#pragma once
#ifndef __ax_c_str_h__
#define __ax_c_str_h__

#include "../common/ax_common.h"

//! \ingroup base_string
//@{

typedef	uint16_t	axUTF16;

inline const char * ax_HEX_chars() { return "0123456789ABCDEF"; }
inline const char * ax_hex_chars() { return "0123456789abcdef"; }

template<class T> inline
bool ax_ishex( T ch ) {
	if( ch >= '0' && ch <='9' ) return true;
	if( ch >= 'A' && ch <='F' ) return true;
	if( ch >= 'a' && ch <='f' ) return true;
	return false;
}

template<class T> inline
T ax_char_escape( T c ) {
	switch( c ) {
		case 'n': return '\n';
		case 'r': return '\r';
		case 'b': return '\b';
		case 't': return '\t';
	}
	return c;
}

template<class T> inline bool	ax_isupper( T  ch ) { return ( ch>='A' && ch <= 'Z' ); }
template<class T> inline bool	ax_islower( T  ch ) { return ( ch>='a' && ch <= 'z' ); }

template<class T> inline bool	ax_isalpha( T  ch ) { return ax_isupper(ch) || ax_islower(ch); }
template<class T> inline bool	ax_isdigit( T  ch ) { return ( ch >= '0' && ch <='9' ); }

template<class T> inline T		ax_toupper( T  ch ) { return ax_islower(ch) ? (ch-('a'-'A')) : ch; }
template<class T> inline T		ax_tolower( T  ch ) { return ax_isupper(ch) ? (ch+('a'-'A')) : ch; }

template<class T> inline const T* ax_empty_c_str( const T* foo ) { return (T*)L""; }


size_t ax_strnlen( const char*    s, size_t len );
size_t ax_strnlen( const wchar_t* s, size_t len );

size_t ax_strlen( const char*    s );
size_t ax_strlen( const wchar_t* s );

      char*    ax_strchr_list(       char*    sz, const char*    chrs );
      wchar_t* ax_strchr_list(       wchar_t* sz, const wchar_t* chrs );

const char*    ax_strchr_list( const char*    sz, const char*    chrs );
const wchar_t* ax_strchr_list( const wchar_t* sz, const wchar_t* chrs );

      char*    ax_strrchrs(       char*    sz, const char*    chrs );
      wchar_t* ax_strrchrs(       wchar_t* sz, const wchar_t* chrs );

const char*    ax_strrchrs( const char*    sz, const char*    chrs );
const wchar_t* ax_strrchrs( const wchar_t* sz, const wchar_t* chrs );

int	 ax_strcasecmp ( const char*    s1, const char*    s2 );
int	 ax_strcasecmp ( const wchar_t* s1, const wchar_t* s2 );

int	 ax_strncasecmp( const char*    s1, const char*    s2, size_t n );
int	 ax_strncasecmp( const wchar_t* s1, const wchar_t* s2, size_t n );

      char* 	ax_strcasechr(       char*    sz, char    ch );
      wchar_t* 	ax_strcasechr(       wchar_t* sz, wchar_t ch );

const char* 	ax_strcasechr( const char*    sz, char    ch );
const wchar_t* 	ax_strcasechr( const wchar_t* sz, wchar_t ch );

      char*    ax_strcasestr(       char*    big, const char*    little );
      wchar_t* ax_strcasestr(       wchar_t* big, const wchar_t* little );

 const char*    ax_strcasestr( const char*    big, const char*    little );
 const wchar_t* ax_strcasestr( const wchar_t* big, const wchar_t* little );

       char*	ax_strchr(       char*    sz, char    ch );
       wchar_t* ax_strchr(       wchar_t* sz, wchar_t ch );

 const char*	  ax_strchr( const char*    sz, char    ch );
 const wchar_t* ax_strchr( const wchar_t* sz, wchar_t ch );
 
 int	 ax_strcmp ( const char*    s1, const char*    s2 );
 int	 ax_strcmp ( const wchar_t* s1, const wchar_t* s2 );
 
inline bool	ax_str_equals( const char*    s1, const char*    s2 ) { return 0 == ax_strcmp(s1,s2); }
inline bool	ax_str_equals( const wchar_t* s1, const wchar_t* s2 ) { return 0 == ax_strcmp(s1,s2); }

 int	 ax_strncmp( const char*    s1, const char*    s2, size_t n );
 int	 ax_strncmp( const wchar_t* s1, const wchar_t* s2, size_t n );

 char*    ax_strcpy( char*    dst, const char*    src );
 wchar_t* ax_strcpy( wchar_t* dst, const wchar_t* src );

 char*    ax_strncpy( char*    dst, const char*    src, size_t n );
 wchar_t* ax_strncpy( wchar_t* dst, const wchar_t* src, size_t n );

       char*    ax_strrchr(       char*    sz, char    ch );
       wchar_t* ax_strrchr(       wchar_t* sz, wchar_t ch );

 const char*    ax_strrchr( const char*    sz, char    ch );
 const wchar_t* ax_strrchr( const wchar_t* sz, wchar_t ch );

       char*    ax_strstr(       char*    a, const char*    b );
       wchar_t* ax_strstr(       wchar_t* a, const wchar_t* b );

 const char*    ax_strstr( const char*    a, const char*    b );
 const wchar_t* ax_strstr( const wchar_t* a, const wchar_t* b );

       char*    ax_strrstr(       char*    a, const char*    b );
       wchar_t* ax_strrstr(       wchar_t* a, const wchar_t* b );

 const char*    ax_strrstr( const char*    a, const char*    b );
 const wchar_t* ax_strrstr( const wchar_t* a, const wchar_t* b );


bool ax_str_has_prefix( const char*    sz, const char*    prefix );
bool ax_str_has_prefix( const wchar_t* sz, const wchar_t* prefix );

//@}

#endif //__ax_c_str_h__

