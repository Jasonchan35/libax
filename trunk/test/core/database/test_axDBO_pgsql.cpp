#include <ax/ax_core.h>

int main() {
	printf("test_axDBO_pgsql");

	axDBO_pgsql	db;
	db.connect( L"192.168.1.36", L"aweweb", L"aweweb", L"1234" );


	getchar();
	return 0;
}