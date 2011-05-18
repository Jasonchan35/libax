#include <ax/ax_base.h>

axStatus test() {
	axStatus st;

	axFile	file;
	st = file.open( "test_file.txt", "wb" );		        if( !st ) return st;
	st = file.format( "writeString test {?}\n", 1234 );		if( !st ) return st;

	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

