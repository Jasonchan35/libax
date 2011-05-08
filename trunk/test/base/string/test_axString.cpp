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

	axArray< axStringA_<10>, 2 >	str_array;
	str_array.resize( 5 );
	for( axSize i=0; i<str_array.size(); i++ ) {
		str_array[i].format( "str({?}).........\n", i );
	}

	str_array.resize( 6 );

	ax_print( "str_array = {?}\n", str_array );

	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

