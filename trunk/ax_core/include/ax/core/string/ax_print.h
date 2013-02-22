#ifndef __ax_print_h__
#define __ax_print_h__

#include "axString.h"
#include "ax_c_str.h"

//! \ingroup base_string
//@{

axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat_ArgList &list );
axStatus ax_print_ArgList( const char*    fmt, const axStringFormat_ArgList &list );

/*! sting format print
	\b Features
	\li Unicode support
	\li Type Safe
	\li formatting with position   
		just like 'gettext' - autosprintf("%2$d %1$d"), 
		but we using {<pos>} such as ax_print( "{2}" ) for the 3th parameter ( since the <pos> start from 0 )

	int a	  = 1;
	int64_t b = 2;
	ax_print( "{1} {?}", a, b );


*/
//!macro - axStatus	ax_print( const char* fmt, ... );
axExpandArgList1( axStatus, ax_print,	const char*,	const axStringFormat_Arg&, axStringFormat_ArgList )

//!macro - axStatus	ax_print( const char* fmt, ... );
axExpandArgList1( axStatus, ax_print,	const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )

axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat_ArgList &list );
axStatus ax_print_ArgList( const char*	  fmt, const axStringFormat_ArgList &list );

void ax_dump_hex( const void* buf, axSize len, FILE *ax_dump_hex = stdout );

template<class T> inline
void ax_dump_hex( const axIArray<T> & buf ) { ax_dump_hex( buf.ptr(), buf.byteSize() ); }

template<class T> inline
void ax_dump_hex( const T* obj ) { ax_dump_hex( obj, sizeof(T) ); }

inline void		ax_dump_string_hex( const char*    sz, FILE* file = stdout ) { ax_dump_hex( sz, ax_strlen(sz), file ); }
inline void		ax_dump_string_hex( const wchar_t* sz, FILE* file = stdout ) { ax_dump_hex( sz, ax_strlen(sz)*sizeof(wchar_t), file ); }

axStatus	ax_convert_dump_hex_string( axIStringA & out, const void* ptr, axSize len );


//@}

#endif //__ax_print_h__

