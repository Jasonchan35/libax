#ifndef __ax_print_h__
#define __ax_print_h__

#include "axString.h"
#include "ax_c_str.h"

//! \ingroup base_string
//@{

axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat::ArgList &list );
axStatus ax_print_ArgList( const char*    fmt, const axStringFormat::ArgList &list );

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

inline
axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
#ifdef axOS_Android
	axTempStringA	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	__android_log_write(ANDROID_LOG_VERBOSE, "libax", tmp.c_str() );
#else
	axTempStringW	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	std::wcout << tmp.c_str();
#endif
	return 0;
}

inline
axStatus ax_print_ArgList( const char* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
#ifdef axOS_Android
	axTempStringA	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	__android_log_write(ANDROID_LOG_VERBOSE, "libax", tmp.c_str() );
#else
	axTempStringW	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	std::wcout << tmp.c_str();
#endif
	return 0;
}

inline
void ax_dump_hex_mem( const void* buf, axSize len, FILE *stream = stdout ) {
    axSize i;
    const uint8_t *c = (const uint8_t*)buf;

    ax_print("----------- dump hex len={?} ------------", len );

    char	line[ 512 ];
    size_t	k;
    const char* hex = ax_hex_chars();
	
    while( len ) {
		k = 0;
        axSize n = ax_min( len, axSize(16) );
		//print address
        k += sprintf( line+k, "\n%p: ", c );
		//print Hex
        for( i=0; i<16; i++ ) {
            if( i == 8 ) { 
				line[k] = ' ';	k++;
				line[k] = ' ';	k++;
			}
            if( i < n ) {
				line[k] = hex[ c[i] >> 4   ];	k++;
				line[k] = hex[ c[i] & 0x0f ];	k++;
			}else{
				line[k] = 'x';	k++;
				line[k] = 'x';	k++;
            }
			line[k] = ' ';	k++;
        }
		line[k] = ' '; k++;
		//print Character
        for( i=0; i<16; i++ ) {
			if( i < n ) {
				if( c[i] >= 32 && c[i] < 127 ) { // < 32 is ASCII control characters
					line[k] = c[i]; k++;
				}else{
					line[k] = '.';	k++;
				}
			}else{
				line[k] = ' ';	k++;
			}
        }
		line[k] = 0;
		fwrite( line, 1, k, stream );
        c += n;
        len -= n;
    }
	fputs( "\n\n", stream );
	fflush( stream );
}

template< class T > inline
void ax_dump_hex_raw( const T& o, FILE *s = stdout ) { ax_dump_hex_mem( &o, sizeof(o), s ); }

inline void		ax_dump_hex_string( const char*    sz, FILE* file = stdout ) { ax_dump_hex_mem( sz, ax_strlen(sz), file ); }
inline void		ax_dump_hex_string( const wchar_t* sz, FILE* file = stdout ) { ax_dump_hex_mem( sz, ax_strlen(sz)*sizeof(wchar_t), file ); }


//@}

#endif //__ax_print_h__

