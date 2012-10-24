#include <ax/ax_base.h>
#include <ax/ax_core.h>

axStatus    test_time() {
	axDateTime	dt;
	dt.now();

	ax_log( "current time is {?}", dt );

    return 0;
}

axStatus	test_timestamp() {
	axTimeStamp	ts;
	ts.now();

	ts += 60;

	axDateTime	dt(ts);	
	ax_log( "timestamp = {?}, {?}", ts, dt );
	return 0;
}

int main() {
    axStatus st;
//	st = test_time();
	st = test_timestamp();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

