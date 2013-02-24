#include "test_json_serializer.h"
 

axStatus test_json_simple() {
	axStatus st;

	TestClassSimple c;
	c.s1.set( L"this is a man special char: \\\\ | \" | \b | \f | \n | \r | \t |\\u00a3|" );
	c.s2.set( "I am a pen" );

	c.s2._getInternalBufferPtr()[0] = 0x1F;

	axTempStringA json_str;

	axApp	app;

	app.getCurrentDir( json_str );
	ax_log( "axApp::getCurrentDir( {?} )", json_str );
	json_str.clear();

	{
		st = ax_to_json( json_str, c, false );
		axUTestCheck(st)
		ax_log("-->\n{?}" ,json_str );
	}

	{
		st = ax_from_json( json_str, c, false );
		ax_log("-->\ns1 {?}\ns2 {?}" , c.s1, c.s2 );
		axUTestCheck(st);
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
	axJsonParser ds( str ) ;
	st = ds.io( c, "myclass" ); if( !st ) return st;

	ax_log("{?}", c );

	return 0;
}


axStatus test_json_class_write() {
    axStatus st;
	
	axTempStringA str;
	TestClass c;
	c.setValue();

	axJsonWriter se( str ) ;
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
		axJsonWriter se( str ) ;
		st = se.io( arr, NULL );		if( !st ) return st;
		ax_log("str -->\n{?}" ,str );
	}

	{
		axJsonParser se( str ) ;
		st = se.io( arr2, NULL );		if( !st ) return st;
		ax_log("arr2 -->\n{?}" ,arr2 );
	}
	return 0;
}

int main() {
    axStatus st;
	
#if axOS_MacOSX
	axScope_NSAutoreleasePool	pool;
#endif //axOS_MacOSX

	//========================================

	axUTestCase( test_json_simple() );
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

