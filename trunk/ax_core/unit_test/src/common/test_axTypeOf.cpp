
#include <ax/ax_core.h>
#include <ax/ax_unit_test.h>

axStatus test_axTypeOf_safe_add()  {
	axStatus st;
	int8_t		i8 = 0;

	for( size_t i=0; i<20; i++ ) {
		st = ax_safe_add( i8, 16 );
		axUTestCheck( st );
	}

	return 0;
}

axStatus test_axTypeOf() {
	axStatus st;
	axUTestCase( test_axTypeOf_safe_add() );

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;
	st = test_axTypeOf();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}
