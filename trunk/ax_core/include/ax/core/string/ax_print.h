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
axExpandArgList1( inline axStatus, ax_print,	const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, ax_print_ArgList )

//!macro - axStatus	ax_print( const char* fmt, ... );
axExpandArgList1( inline axStatus, ax_print,	const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList, ax_print_ArgList )

axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat_ArgList &list );
axStatus ax_print_ArgList( const char*	  fmt, const axStringFormat_ArgList &list );

		void ax_dump_hex_mem( const void* buf, axSize len, FILE *stream = stdout );
inline 	void ax_dump_hex_mem( const axIByteArray & buf ) { ax_dump_hex_mem( buf.ptr(), buf.byteSize() ); }

template< class T > inline
void ax_dump_hex_raw( const T& o, FILE *s = stdout ) { ax_dump_hex_mem( &o, sizeof(o), s ); }

inline void		ax_dump_hex_string( const char*    sz, FILE* file = stdout ) { ax_dump_hex_mem( sz, ax_strlen(sz), file ); }
inline void		ax_dump_hex_string( const wchar_t* sz, FILE* file = stdout ) { ax_dump_hex_mem( sz, ax_strlen(sz)*sizeof(wchar_t), file ); }


//@}

#endif //__ax_print_h__

