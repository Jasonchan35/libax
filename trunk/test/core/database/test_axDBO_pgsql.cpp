#include <ax/ax_core.h>

axStatus test() {
	axStatus st;
	ax_print(L"testing {?}\n", __FILE__ );

	axDBO	db;
	st = db.connect( "pgsql", "host=127.0.0.1 dbname=testdb user=testing password=1234" );
	if( !st ) return st;

	axDBO_Result	res;

//	axDBO_Stmt stmt = db.prepareSQL( "insert into tbl_test ( name, number ) values( 'p0', $1 )", p0 );
//	stmt.exec();

	ax_print("---- test insert ---- \n");

	int p0 = 2345;
	res = db.execSQL( "insert into tbl_test ( name, number ) values( 'p0', $1 )", 12345 );
	res.print();


	ax_print("---- test select ---- \n");
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


