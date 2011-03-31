#include <ax/ax_core.h>

int main() {
	axStatus st;
	printf("test_axDBO_pgsql");

	axDBO	db;
	db.connect( L"pgsql", L"192.168.1.36", L"aweweb", L"aweweb", L"1234" );

	st = db.execSQL( L"select * from test" );

	getchar();
	return 0;
}