//#include "test_json_serializer.h"

#include <ax/ax_core.h>
#include <ax/ax_unit_test.h>

class TestClass {
public:
	uint32_t	test_uint;
	int32_t		test_int;
	int32_t		test_neg_int;

	template<class S>
	axStatus serialize_io( S & s ) {
		axStatus st;
		ax_io_vary( test_uint );
		ax_io_vary( test_int );
		ax_io_vary( test_neg_int );
		return 0;
	}
	
	bool operator == ( const TestClass & s ) {
		if( test_uint != s.test_uint ) return false;
		if( test_int != s.test_int ) return false;
		if( test_neg_int != s.test_neg_int ) return false;
		return true;
	}
	
	bool operator != ( const TestClass & s ) { return ! operator==(s); }
};

axStatus test_io_vary() {
	axStatus st;

		axByteArray_< 512 > buf;
	
	for( int i=0; i<50; i++ ) {
		TestClass src, dst;
		src.test_uint = i * 1779;
		src.test_int = src.test_uint;
		src.test_neg_int = -src.test_int;


		st = ax_serialize_to_buf( buf, src );		if( !st ) return st;		
		st = ax_serialize_from_buf( buf, dst );		if( !st ) return st;

			ax_log("----");
			ax_log_hex( buf );
			ax_log_var3( src.test_uint, src.test_int, src.test_neg_int );
			ax_log_var3( dst.test_uint, dst.test_int, dst.test_neg_int );
		
		if( src != dst ) {
			ax_log_hex( buf );
			ax_log_var3( src.test_uint, src.test_int, src.test_neg_int );
			ax_log_var3( dst.test_uint, dst.test_int, dst.test_neg_int );
			uint8_t v = src.test_uint;
			uint8_t a = (v << 1) ^ (v >> 7);
			
			uint8_t b;
			
			if( a % 2 ) {
				b = ~(a >> 1);
			}else{
				b = a >> 1;
			}
			
			ax_log("{0:4d} {0:08b}", src.test_int );
			ax_log("{0:4d} {0:08b}", dst.test_int );
			ax_log("----");			
			ax_log("{0:4d} {0:08b}", v << 1 );
			ax_log("{0:4d} {0:08b}", (uint8_t)(v >> 7) );
			ax_log("{0:4d} {0:08b}", a );
			ax_log("----");
			ax_log("{0:4d} {0:08b}", v );
			ax_log("{0:4d} {0:08b}", a );
			ax_log("{0:4d} {0:08b}", a>>1 );
			ax_log("{0:3X} {0:08b}", b );
			assert( false );
		}
	}
	
	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;
	//========================================

	axUTestCase( test_io_vary() );
	//st = test_json_class_write();
	//st = test_json_class_read();
	//st = test();

	//========================================

    ax_print("==== end of program ====" );
#if axCOMPILER_VC
	getchar();
#endif
    return 0;
}

