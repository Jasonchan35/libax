//
//  test_axIArray.cpp
//  
//
//  Created by Jason on 2012-11-07.
//
//

#include <ax/ax_core.h>

//#define	axTestAssert( E )	{ if( !(E) ) { printf("FAIL: %s [%s:%u]\n", #E, __FILE__, __LINE__); assert(false); } }

#define	axTestAssert( E ) { \
	if( E ) { \
		printf("PASS: %s\n", #E); \
	}else{ \
		printf("FAIL: %s\n   %s:%u\n", #E, __FILE__, __LINE__ ); \
		assert(false); \
	} \
}\
//-----

#define axTestFunc( E )			{ printf("TEST: %s\n", #E); E; }
#define axUnitTest( F )			{ printf("==== %s ====\n", #F); axStatus st = F(); if( !st ) return st; }


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
	axTestFunc( st = init_test_array(a,12) );	if( !st ) return st;

	axTestFunc( st = a.remove( 6, 3 ) );		if( !st ) return st;
	axTestAssert( a.size() == 9 );

	axTestAssert( a[5] == 5 );
	axTestAssert( a[6] == 9 );
	axTestAssert( a[7] == 10 );


	ax_log_var(a);
	return 0;
}

axStatus test_axIArray_insert() {
	axStatus st;
	axArray<int,4>	a;
	axTestFunc( st = init_test_array(a, 10) );	if( !st ) return st;
	axTestFunc( st = a.insert( 4, 4 ) );			if( !st ) return st;

	ax_log_var( a );
	
	axTestAssert( a.size() == 10+1 );
	axTestAssert( a[3] == 3 );
	axTestAssert( a[4] == 4 );
	axTestAssert( a[5] == 4 );
	axTestAssert( a[6] == 5 );

//-----
	axArray<int>	b;
	axTestFunc( st = b.resize(3) );				if( !st ) return st;
	for( int i=0; i<b.size(); i++ ) {
		b[i] = i + 100;
	}

	axTestFunc( st = a.insertN( 2, b ) );		if( !st ) return st;
	
	ax_log_var( a );
	ax_log_var( b );
	
	axTestAssert( a.size() == 10+1+3 );
	axTestAssert( a[1] == 1 );
	axTestAssert( a[2] == 100 );
	axTestAssert( a[3] == 101 );
	axTestAssert( a[4] == 102 );
	axTestAssert( a[5] == 2 );
	
	return 0;
}


axStatus test_axIArray() {
	axStatus st;
	axUnitTest( test_axIArray_insert );
	axUnitTest( test_axIArray_remove );

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;
	st = test_axIArray();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
//	getchar();
    return 0;
}
