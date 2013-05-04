//#include "test_json_serializer.h"

#include <ax/ax_core.h>
#include <ax/ax_unit_test.h>

class TestClass {
public:
	axStringA	test_strA;
	axDateTime	test_dateTime;
	int			test_int;
	float		test_float;

	axStatus	testData() {
		test_strA.set("this is stringA");
		test_dateTime.setToNow();
		test_int = 12345;
		test_float = 8.9123f;
		return 0;
	}

	template<class S>
	axStatus serialize_io( S & s ) {
		axStatus st;
		ax_io( test_strA );
		ax_io( test_dateTime );
		ax_io( test_int );
		ax_io( test_float );
		return 0;
	}
};


axStatus test_json_simple() {
	axStatus st;

	TestClass data;
	data.testData();

//	c.s2._getInternalBufferPtr()[0] = 0x1F;

	axTempStringA json_str;
	{
		st = ax_to_json( json_str, data, false );
		axUTestCheck(st)
		ax_log("==== Json ====\n{?}\n===== end of Json ====\n" , json_str );
	}

	{
		TestClass	tmp;
		st = ax_from_json( json_str, tmp, false );
		axUTestCheck(st);
		axUTestCheck( tmp.test_strA.equals( data.test_strA ) );

		ax_log_var( data.test_dateTime );
		ax_log_var( data.test_dateTime.second );

		ax_log_var( tmp.test_dateTime );
		ax_log_var( tmp.test_dateTime.second );
		
		ax_log("{?:f}", tmp.test_dateTime.toTimeStamp().second() );
		ax_log_var( tmp.test_dateTime.toTimeStamp().second() );
		
		
		axUTestCheck( tmp.test_dateTime.absDiff( data.test_dateTime ) <= 0.001f );
		axUTestCheck( tmp.test_int 		== data.test_int );
		axUTestCheck( tmp.test_float 	== data.test_float );
	}

	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;
	//========================================

	axUTestCase( test_json_simple() );
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

