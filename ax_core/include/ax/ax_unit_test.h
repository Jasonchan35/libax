#ifndef __ax_unit_test_h__
#define __ax_unit_test_h__

//#define	axUTestAssert( E )	{ if( !(E) ) { printf("FAIL: %s [%s:%u]\n", #E, __FILE__, __LINE__); assert(false); } }

#define	axUTestCheck( E ) { \
	if( E ) { \
		/*printf("PASS: %s\n", #E);*/ \
	}else{ \
		printf("FAIL: %s\n   %s:%u\n", #E, __FILE__, __LINE__ ); \
		assert(false); \
	} \
}\
//-----

#define axUTestDo( E )			{ printf("DO:   %s\n", #E); E; }
#define axUTestCase( F )		{ printf("==== %s ====\n", #F); axStatus st = F(); if( !st ) return st; }


#endif //__ax_unit_test_h__
