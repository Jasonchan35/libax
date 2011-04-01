#include <ax/ax_core.h>

axStatus test() {
	axStatus st;
	ax_print(L"testing {?}\n", __FILE__ );

	axDBO	db;
	st = db.connect( "pgsql", "host=127.0.0.1 dbname=testdb user=testing password=1234" );
	if( !st ) return st;

	axDBO_Result	res;
	res = db.execSQL( "select * from tbl_test" );
	res.print();

	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}


