#include <ax/ax_core.h>


axStatus test() {
	axStatus st;
	axStringA	a;
	axStringW	w;

	int value;
	ax_str_to( "123", value );


//=======
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

	ax_log("{1} {0}\n", 'a', 'b' );

//===========

	axStringA_Array	str_array;
	str_array.resize( 16 );
	for( axSize i=0; i<str_array.size(); i++ ) {
		str_array[i].format( "{?}", (char)('A'+ ax_random(26) ) );
	}
	
	ax_log( "str_array = \n{?}\n\n", str_array );
//	str_array.remove( 2, 3 );
	ax_log( "after remove str_array = \n{?}\n\n", str_array );

	str_array.sortNoCase( true );
	ax_log( "after sort ascending  str_array = \n{?}\n\n", str_array );

	str_array.sortNoCase( false );

	for( axSize i=0; i<str_array.size(); i++ ) {
		str_array[i].toUpperCase();
	}
	ax_log( "after sort descending str_array = \n{?}\n\n", str_array );

	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

