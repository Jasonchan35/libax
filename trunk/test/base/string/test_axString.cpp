#include <ax/ax_core.h>

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

	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

