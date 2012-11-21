//
//  test_hashTable.cpp
//  ax_core_unit_test
//
//  Created by Jason on 2012-11-21.
//  Copyright (c) 2012 Jason. All rights reserved.
//

#include <ax/ax_core.h>


class Obj : public axHashTableNode< Obj, true > {
public:

	Obj( size_t v ) : value( v ) {}

	uint32_t	hashTableValue() { return (uint32_t) value; }

	axStatus toStringFormat( axStringFormat &f ) const {
		return f.format("{?}", value );
	}

	size_t	value;
};

axStatus do_test_hashTable() {
    axStatus st;
	
	axHashTable< Obj >		tbl;
	
	size_t last_size = 0;
	
	for( size_t i=0; i<1000000; i++ ) {
		tbl.insert( new Obj( i ) );
		
		if( tbl.tableSize() > last_size ) {
			ax_log("{?}  {?}", i, tbl.tableSize() );
			last_size = tbl.tableSize();
		}
	}
	
	
//	axHashTable< Obj >::Iterator	it( tbl );
//	for( ; it; it++ ) {
//		ax_log_var( *it );
//	}
		
//	ax_log("{?}", tbl );
	
	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;
	st = do_test_hashTable();
	
	
    ax_print("==== return {?} ====\n", st );
#if axCOMPILER_VC
	getchar();
#endif
    return 0;
}