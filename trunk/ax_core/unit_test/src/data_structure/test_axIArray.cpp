//
//  test_axIArray.cpp
//  
//
//  Created by Jason on 2012-11-07.
//
//

#include <ax/ax_core.h>
#include <ax/ax_unit_test.h>


axStatus init_test_array( axIArray<int> &a, int n ) {
	axStatus st;
	st = a.resize(n);		if( !st ) return st;
	for( int i=0; i<n; i++ ) {
		a[i] = i;
	}
	return 0;
}

axStatus test_axIArray_remove() {
	axStatus st;
	axArray<int,4>	a;
	axUTestDo( st = init_test_array(a,12) );	if( !st ) return st;

	axUTestDo( st = a.remove( 6, 3 ) );		if( !st ) return st;
	axUTestCheck( a.size() == 9 );

	axUTestCheck( a[5] == 5 );
	axUTestCheck( a[6] == 9 );
	axUTestCheck( a[7] == 10 );


	ax_log_var(a);
	return 0;
}

axStatus test_axIArray_insert() {
	axStatus st;
	axArray<int,4>	a;
	axUTestDo( st = init_test_array(a, 10) );	if( !st ) return st;
	axUTestDo( st = a.insert( 4, 4 ) );			if( !st ) return st;

	ax_log_var( a );
	
	axUTestCheck( a.size() == 10+1 );
	axUTestCheck( a[3] == 3 );
	axUTestCheck( a[4] == 4 );
	axUTestCheck( a[5] == 4 );
	axUTestCheck( a[6] == 5 );

//-----
	axArray<int>	b;
	axUTestDo( st = b.resize(3) );				if( !st ) return st;
	for( size_t i=0; i<b.size(); i++ ) {
		b[i] = i + 100;
	}

	axUTestDo( st = a.insertN( 2, b ) );		if( !st ) return st;
	
	ax_log_var( a );
	ax_log_var( b );
	
	axUTestCheck( a.size() == 10+1+3 );
	axUTestCheck( a[1] == 1 );
	axUTestCheck( a[2] == 100 );
	axUTestCheck( a[3] == 101 );
	axUTestCheck( a[4] == 102 );
	axUTestCheck( a[5] == 2 );
	
	return 0;
}


axStatus test_axIArray() {
	axStatus st;
	axUTestCase( test_axIArray_insert );
	axUTestCase( test_axIArray_remove );

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;
	st = test_axIArray();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}
