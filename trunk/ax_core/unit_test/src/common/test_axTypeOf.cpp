
#include <ax/ax_core.h>
#include <ax/ax_unit_test.h>


axStatus test_axTypeOf_safe_abs()  {
	axStatus st;

	ax_log("test ax_safe_abs( int8 )");
	for( int i=0; i<=255; i++ ) {
		for( int j=0; j<=255; j++ ) {
			uint8_t	a=i;
			int c = ax_abs(i);
			st = ax_safe_abs( a );
			if( a == c ) {
				axUTestCheck( st );
			}else{
				axUTestCheck( st.code() == axStatus_Std::non_safe_abs );
			}
		}
	}

	return 0;
}

axStatus test_axTypeOf_safe_add()  {
	axStatus st;

	ax_log("test ax_safe_add( int8, int8 )");
	for( int i=0; i<=255; i++ ) {
		for( int j=0; j<=255; j++ ) {
			uint8_t	a=i, b=j;
			int c = i+j;
			st = ax_safe_add( a,b );
			if( a == c ) {
				axUTestCheck( st );
			}else{
				axUTestCheck( st.code() == axStatus_Std::non_safe_add );
			}
		}
	}

	ax_log("test ax_safe_add( uint8, uint8 )");
	for( int i=-128; i<=127; i++ ) {
		for( int j=-128; j<=127; j++ ) {
			int8_t	a=i, b=j;
			int c = i+j;
			st = ax_safe_add( a,b );
			if( a == c ) {
				axUTestCheck( st );
			}else{
				axUTestCheck( st.code() == axStatus_Std::non_safe_add );
			}
		}
	}	
	return 0;
}


axStatus test_axTypeOf_safe_sub()  {
	axStatus st;

	ax_log("test ax_safe_sub( int8, int8 )");
	for( int i=0; i<=255; i++ ) {
		for( int j=0; j<=255; j++ ) {
			uint8_t	a=i, b=j;
			int c = i-j;
			st = ax_safe_sub( a,b );
			if( a == c ) {
				axUTestCheck( st );
			}else{
				axUTestCheck( st.code() == axStatus_Std::non_safe_add );
			}
		}
	}

	ax_log("test ax_safe_sub( uint8, uint8 )");
	for( int i=-128; i<=127; i++ ) {
		for( int j=-128; j<=127; j++ ) {
			int8_t	a=i, b=j;
			int c = i-j;
			st = ax_safe_sub( a,b );
			if( a == c ) {
				axUTestCheck( st );
			}else{
				axUTestCheck( st.code() == axStatus_Std::non_safe_add );
			}
		}
	}	
	return 0;
}

template<class T>
axStatus axStatus_test_axTypeOf_safe_mul_case( T a, T b ) {
	ax_log("test {?:2} x {?:2} = {?:4} ( {?:4} !! )", a, b, (T)(a*b), a*b );
	return ax_safe_mul( a, b );
}

axStatus test_axTypeOf_safe_mul()  {
	axStatus st;

//axStatus_test_axTypeOf_safe_mul_case<int8_t> ( i, j );

	ax_log("test ax_safe_mul( int8, int8 )");
	for( int i=0; i<=255; i++ ) {
		for( int j=0; j<=255; j++ ) {
			uint8_t	a=i, b=j;
			int c = i*j;
			st = ax_safe_mul( a,b );
			if( a == c ) {
				axUTestCheck( st );
			}else{
				axUTestCheck( st.code() == axStatus_Std::non_safe_mul );
			}
		}
	}

	ax_log("test ax_safe_mul( uint8, uint8 ) ");
	for( int i=-128; i<=127; i++ ) {
		for( int j=-128; j<=127; j++ ) {
			int8_t	a=i, b=j;
			int c = i*j;
			st = ax_safe_mul( a,b );
			if( a == c ) {
				axUTestCheck( st );
			}else{
				axUTestCheck( st.code() == axStatus_Std::non_safe_mul );
			}
		}
	}

	return 0;
}


axStatus test_axTypeOf() {
	axStatus st;
	axUTestCase( test_axTypeOf_safe_abs() );

	axUTestCase( test_axTypeOf_safe_add() );
	axUTestCase( test_axTypeOf_safe_sub() );
	axUTestCase( test_axTypeOf_safe_mul() );

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
