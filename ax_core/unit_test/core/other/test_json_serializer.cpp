#include "test_json_serializer.h"
 

axStatus test_json_simple() {
	axStatus st;

	TestClassSimple c;
	c.s1.set( L"this is a man special char: \\\\ | \" | \b | \f | \n | \r | \t |\\u00a3|" );
	c.s2.set( "I am a pen" );

	c.s2._getInternalBufferPtr()[0] = 0x1F;

	axTempStringA json_str;

	axApp::getCurrentDir( json_str );
	ax_log( "axApp::getCurrentDir( {?} )", json_str );
	json_str.clear();

	{
		axJSONSerializer se( json_str ) ;
		st = se.io( c ); if( !st ) return st;

		ax_log("-->\n{?}" ,json_str );
	}

	{

		axJSONDeserializer se( json_str ) ;
		st = se.io( c ); if( !st ) return st;

		ax_log("-->\ns1 {?}\ns2 {?}" , c.s1, c.s2 );
	}

	return 0;
}
 

axStatus test_json_class_read() {
	axStatus st;
	axTempStringA str;
	//st = axFileSystem::loadFile( str, "../../../test/base/other/myclass.json" ); if( !st ) return st;
	st = axFileSystem::loadFile( str, "myclass.json" ); if( !st ) return st;

	
	TestClass c;
	c.setValue2();
	axJSONDeserializer ds( str ) ;
	st = ds.io( c, "myclass" ); if( !st ) return st;

	ax_log("{?}", c );

	return 0;
}


axStatus test_json_class_write() {
    axStatus st;
	
	axTempStringA str;
	TestClass c;
	c.setValue();

	axJSONSerializer se( str ) ;
	st = se.io( c, "myclass" ); if( !st ) return st;

	ax_log("str -->\n{?}" ,str );

	//st = axFileSystem::saveFile( str, "../../../test/base/other/myclass.json" ); if( !st ) return st;
	st = axFileSystem::saveFile( str, "myclass.json" ); if( !st ) return st;



		
	return 0;
}


axStatus test() {
	axStatus st;
	axTempStringA str;
	axArray<int> arr, arr2;

	st = arr.resize( 10 ); if( !st ) return st;


	for( int i=0; i<10; i++ ) {
		arr[i] = i * 9;
	}

	{
		axJSONSerializer se( str ) ;
		st = se.io( arr );		if( !st ) return st;	
		ax_log("str -->\n{?}" ,str );
	}

	{
		axJSONDeserializer se( str ) ;
		st = se.io( arr2 );		if( !st ) return st;	
		ax_log("arr2 -->\n{?}" ,arr2 );
	}
	return 0;
}

int main() {
    axStatus st;
	
#if axOS_MacOSX
	axScope_NSAutoreleasePool	pool;
#endif //axOS_MacOSX

	axStatus a = axStatus_Std::not_enough_memory;
	ax_log("{?}", a);
	

	//========================================

	//st = test_json_simple();
	//st = test_json_class_write();
	//st = test_json_class_read();
	//st = test();

	//========================================

    ax_print("==== return {?} ====\n", st );
#if axCOMPILER_VC
	getchar();
#endif
    return 0;
}

