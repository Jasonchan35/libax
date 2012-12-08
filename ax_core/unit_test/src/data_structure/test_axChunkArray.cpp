//
//  test_hashTable.cpp
//  ax_core_unit_test
//
//  Created by Jason on 2012-11-21.
//  Copyright (c) 2012 Jason. All rights reserved.
//

#include <ax/ax_core.h>


class Element {
public:

	Element() {
		v = 999999;
	}
	axSize v;
	
	
	axStatus onTake( Element &src ) {
		v = src.v;
		return 0;
	}
};


axStatus do_test_trunk() {
    axStatus st;
	axSize n = 100;

	//axChunkArray<Element,16,4>	arr;
	/*
	arr.resize( 100 );
	arr.last().v = 100;
	
	for( axSize i=0; i<n; i++ ) {
		st = arr.incSize( 1 ); if( !st ) return st;
		arr.last().v = i;
	}
	
	for( axSize i=0; i<100; i++ ) {
		ax_log_var( arr[i].v );
	}
	*/
	axChunkArray< int, 16, 4 >	arr;
	for( size_t i=0; i<n; i++ ) {
		arr.append( (int)i );
		ax_log_var( arr );
	}
	
	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;
	st = do_test_trunk();
	
	
    ax_print("==== return {?} ====\n", st );
#if axCOMPILER_VC
	getchar();
#endif
    return 0;
}