#include <ax/ax_core.h>

axStatus test() {
	axStatus st;
	printf("test_axDBO_pgsql");

	axDBO	db;
	st = db.connect( L"pgsql", L"host=127.0.0.1 dbname=lib_test user=testing password=1234" );
	if( !st ) return st;

	st = db.execSQL( L"select * from test" );

	getchar();
	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s====\n", st.code(), st.c_str() );
    return 0;
}


