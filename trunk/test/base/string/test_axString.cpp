#include <ax/ax_base.h>

axStatus test() {
	axStatus st;
	axStringA	a;
	axStringW	w;

	st = a.set( "abcd" );	if( !st ) return st;

	if( 0 != ax_strcmp( a, "abcd" ) ) {
		return -1;
	}
	st = a.set( L"abcd" );	if( !st ) return st;
	if( 0 != ax_strcmp( a, "abcd" ) ) {
		return -1;
	}

//========
	st = w.set( "abcd" );	if( !st ) return st;
	if( 0 != ax_strcmp( w, L"abcd" ) ) {
		return -1;
	}
	st = w.set( L"abcd" );	if( !st ) return st;
	if( 0 != ax_strcmp( w, L"abcd" ) ) {
		return -1;
	}

	ax_print("{1} {0}\n", 'a', 'b' );

//===========

	axStringA_Array	str_array;
	str_array.resize( 16 );
	for( axSize i=0; i<str_array.size(); i++ ) {
		if( i % 2 ) {
			str_array[i].format( "{0:c}{0:c}", ('p' - i) );
		}else{
			str_array[i].format( "{0:c}{0:c}", ('P' - i) );
		}
	}

	ax_print( "str_array = \n{?}\n\n", str_array );
//	str_array.remove( 2, 3 );
	ax_print( "after remove str_array = \n{?}\n\n", str_array );

	str_array.sortIgnoreCase( true );
	ax_print( "after sort ascending  str_array = \n{?}\n\n", str_array );

	str_array.sortIgnoreCase( false );

	for( axSize i=0; i<str_array.size(); i++ ) {
		str_array[i].toUpperCase();
	}
	ax_print( "after sort descending str_array = \n{?}\n\n", str_array );

	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

