#include <ax/ax_core.h>

axStatus test() {
	axStatus st;

	ax_print(L"testing {?}\n", __FILE__ );

	axDBO	db;
	st = db.connect( "pgsql", "host=127.0.0.1 dbname=testdb user=testing password=1234" );
	if( !st ) return st;

	axDBO_DataSet	data_set;

//	axDBO_Stmt stmt = db.prepareSQL( "insert into tbl_test ( name, number ) values( 'p0', $1 )", p0 );
//	stmt.exec();

	ax_print("---- test insert ---- \n");

	axStringA str_a;
	str_a.set( "testing" );
	axStringW str_w;
	str_w.set( L"testing" );
	axByteArray	bytea;
	st = bytea.resize( 10 );	assert( st );
	axSize i;
	for( i=0; i<bytea.size(); i++ ) {
		bytea[i] = i*3;
	}

	st = db.execSQL( data_set,
                      "insert into tbl_test ( "
					  " test_char, test_int16, test_int32, test_int64, "
					  " test_char_str, test_wchar_str, test_stringa, test_stringw,"
					  " test_bool, test_bytearray "
					  " ) values( {?}, {?}, {2}, {?}, {?}, {?}, {?}, {?}, {?}, {?} )",
						'X', 12345, 12345678, 123456789123456789LL,
						"testing", L"testing", str_a, str_w,
						false, bytea );
	ax_print( "{?}", data_set );


	ax_print("---- test select ---- \n");
	st = db.execSQL( data_set,
                      "select "
					  " test_char, test_int16, test_int32, test_int64, "
					  " test_char_str, test_wchar_str, test_stringa, test_stringw,"
					  " test_bool, test_bytearray "
					  " from tbl_test order by _id" );
	ax_print( "{?}", data_set );


	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}


