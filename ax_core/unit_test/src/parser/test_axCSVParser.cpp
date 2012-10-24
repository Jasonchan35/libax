#include <ax/ax_core.h>

axStatus test() {
	axStatus st;
	axStringA	a;
	axStringW	w;

	axCSVFileParser	csv;
	csv.open(L"test.csv");

	axStringA_Array	row;
	while( csv.getRow( row ) ) {
		ax_log( "row = {?}\n", row );
	}

	ax_log( "=== end CSV ==\n" );
	return 0;
}

int main() {
    axStatus st = test();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

