//
//  ax_print.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/string/axStringFormat_out_Imp.h>
#include <ax/core/string/ax_print.h>


axExpandArgList1_Imp( axStatus, ax_print,	const char*,	const axStringFormat_Arg&, axStringFormat_ArgList )
axExpandArgList1_Imp( axStatus, ax_print,	const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )


axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat_ArgList &list ) {
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

axStatus ax_print_ArgList( const char* fmt, const axStringFormat_ArgList &list ) {
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

axStatus	ax_convert_dump_hex_string( axIStringA & out, const void* ptr, axSize len ) {
	axStatus st;
	st = out.reserve( len * 5 );		if( !st ) return st;

    axSize i;
    const uint8_t *c = (const uint8_t*)ptr;

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

		st = out.appendWithLength( line, k );		if( !st ) return st;

        c += n;
        len -= n;
    }

	return 0;
}

void ax_dump_hex_mem( const void* buf, axSize len, FILE *stream ) {

	axStringA_<8192>	str;
	axStatus st;
	st = ax_convert_dump_hex_string( str, buf, len );		assert(st);

    ax_print("----------- dump hex len={?} ------------", len );

	fputs( str, stream );
	fputs( "\n\n", stream );
	fflush( stream );
}
