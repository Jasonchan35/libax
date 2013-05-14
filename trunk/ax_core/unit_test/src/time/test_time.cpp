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

axStatus	test_loop() {
	axStopWatch	watch;
	axTimeStamp	ts;
	for( int i=0; i<100000; i++ ) {
		ts.now();
		if( i % 1000 == 0 ) {
			ax_log("{?} {?} time = {?}", i, watch.get(), ts );
		}
	}

	return 0;
}

int main() {
    axStatus st;
//	st = test_time();
	st = test_timestamp();
//	st = test_loop();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

